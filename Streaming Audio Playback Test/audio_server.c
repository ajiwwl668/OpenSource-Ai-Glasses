#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <alsa/asoundlib.h>

#define PORT 8888
#define SAMPLE_RATE 16000
#define CHANNELS 2  // 根据您的测试，使用2声道
#define BUFFER_SIZE 4096
#define FORMAT SND_PCM_FORMAT_S16_LE

typedef struct {
    int client_socket;
} client_data_t;

// 音频设备初始化
int init_audio(snd_pcm_t **handle) {
    int rc;
    snd_pcm_hw_params_t *hw_params;
    unsigned int sample_rate = SAMPLE_RATE;
    snd_pcm_uframes_t period_size = 1024;
    
    printf("初始化音频设备: %d Hz, %d 声道, S16_LE格式\n", SAMPLE_RATE, CHANNELS);
    
    // 打开PCM设备
    rc = snd_pcm_open(handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr, "无法打开PCM设备: %s\n", snd_strerror(rc));
        return -1;
    }
    
    // 分配和初始化硬件参数
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(*handle, hw_params);
    
    // 设置参数
    snd_pcm_hw_params_set_access(*handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(*handle, hw_params, FORMAT);
    snd_pcm_hw_params_set_channels(*handle, hw_params, CHANNELS);
    snd_pcm_hw_params_set_rate_near(*handle, hw_params, &sample_rate, 0);
    
    // 设置缓冲区参数
    snd_pcm_hw_params_set_period_size_near(*handle, hw_params, &period_size, 0);
    
    // 应用参数
    rc = snd_pcm_hw_params(*handle, hw_params);
    if (rc < 0) {
        fprintf(stderr, "无法设置硬件参数: %s\n", snd_strerror(rc));
        snd_pcm_close(*handle);
        return -1;
    }
    
    printf("音频设备初始化成功\n");
    return 0;
}

// 处理客户端连接
void* handle_client(void* arg) {
    client_data_t *data = (client_data_t*)arg;
    int client_socket = data->client_socket;
    snd_pcm_t *playback_handle;
    int rc;
    
    // 初始化音频设备
    if (init_audio(&playback_handle) != 0) {
        fprintf(stderr, "音频初始化失败，关闭客户端连接\n");
        close(client_socket);
        free(data);
        return NULL;
    }
    
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    
    printf("开始接收音频数据流...\n");
    
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        // 计算帧数 (S16_LE格式，每个样本2字节)
        snd_pcm_uframes_t frames = bytes_received / (CHANNELS * 2);
        
        // 写入音频数据
        rc = snd_pcm_writei(playback_handle, buffer, frames);
        
        if (rc == -EPIPE) {
            // 欠载运行
            fprintf(stderr, "音频欠载，重新准备设备\n");
            snd_pcm_prepare(playback_handle);
        } else if (rc < 0) {
            fprintf(stderr, "写入错误: %s\n", snd_strerror(rc));
            break;
        } else if (rc != frames) {
            fprintf(stderr, "写入帧数不匹配: %d != %lu\n", rc, frames);
        }
    }
    
    if (bytes_received == 0) {
        printf("客户端主动断开连接\n");
    } else if (bytes_received < 0) {
        perror("接收数据错误");
    }
    
    // 清理资源
    snd_pcm_drain(playback_handle);  // 播放剩余数据
    snd_pcm_close(playback_handle);
    close(client_socket);
    free(data);
    
    printf("客户端处理完成\n");
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;
    
    printf("启动音频流服务端 (端口: %d)\n", PORT);
    printf("配置: %d Hz, %d 声道, S16_LE格式\n", SAMPLE_RATE, CHANNELS);
    
    // 创建服务器socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("创建socket失败");
        return -1;
    }
    
    // 设置socket选项
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("设置socket选项失败");
        close(server_socket);
        return -1;
    }
    
    // 绑定地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("绑定地址失败");
        close(server_socket);
        return -1;
    }
    
    // 开始监听
    if (listen(server_socket, 5) < 0) {
        perror("监听失败");
        close(server_socket);
        return -1;
    }
    
    printf("服务端准备就绪，等待客户端连接...\n");
    
    // 主循环
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("接受连接失败");
            continue;
        }
        
        printf("客户端连接: %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        // 创建客户端数据结构
        client_data_t *client_data = malloc(sizeof(client_data_t));
        client_data->client_socket = client_socket;
        
        // 创建处理线程
        if (pthread_create(&thread_id, NULL, handle_client, client_data) != 0) {
            perror("创建线程失败");
            close(client_socket);
            free(client_data);
        } else {
            pthread_detach(thread_id);  // 分离线程，自动回收资源
        }
    }
    
    close(server_socket);
    return 0;
}
