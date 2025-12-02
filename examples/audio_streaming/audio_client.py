import socket
import pyaudio
import threading
import signal
import sys

class AudioClient:
    def __init__(self, server_host, server_port=8888):
        self.server_host = server_host
        self.server_port = server_port
        self.socket = None
        self.audio = pyaudio.PyAudio()
        self.stream = None
        self.is_recording = False
        
        # 音频参数 - 与服务端匹配
        self.format = pyaudio.paInt16
        self.channels = 2  # 改为2声道
        self.rate = 16000
        self.chunk = 1024  # 每次读取的帧数
        
    def connect(self):
        """连接到服务端"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.server_host, self.server_port))
            print(f"已连接到服务端 {self.server_host}:{self.server_port}")
            print(f"音频参数: {self.rate}Hz, {self.channels}声道, 16位")
            return True
        except Exception as e:
            print(f"连接失败: {e}")
            return False
    
    def start_recording(self):
        """开始录制并发送音频数据"""
        if not self.socket:
            print("未连接到服务端")
            return
            
        self.is_recording = True
        
        try:
            # 打开音频输入流
            self.stream = self.audio.open(
                format=self.format,
                channels=self.channels,
                rate=self.rate,
                input=True,
                frames_per_buffer=self.chunk,
                stream_callback=self.audio_callback if hasattr(self, 'audio_callback') else None
            )
            
            print("开始录音... 按 Ctrl+C 停止")
            self.stream.start_stream()
            
            # 保持主线程运行
            while self.is_recording and self.stream.is_active():
                import time
                time.sleep(0.1)
                
        except KeyboardInterrupt:
            print("\n停止录音")
        except Exception as e:
            print(f"录音错误: {e}")
        finally:
            self.stop_recording()
    
    def audio_callback(self, in_data, frame_count, time_info, status):
        """音频回调函数（可选）"""
        if self.socket and self.is_recording:
            try:
                self.socket.sendall(in_data)
            except Exception as e:
                print(f"发送数据错误: {e}")
                self.is_recording = False
        return (in_data, pyaudio.paContinue)
    
    def stop_recording(self):
        """停止录制"""
        self.is_recording = False
        if self.stream:
            self.stream.stop_stream()
            self.stream.close()
        if self.socket:
            self.socket.close()
        if self.audio:
            self.audio.terminate()
        print("音频客户端已关闭")

def signal_handler(sig, frame):
    """处理Ctrl+C信号"""
    print('\n正在关闭...')
    if 'client' in globals():
        client.stop_recording()
    sys.exit(0)

if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)
    
    # 服务端IP地址
    SERVER_IP = "192.168.113.246"  # 替换为实际IP
    
    client = AudioClient(SERVER_IP)
    
    if client.connect():
        # 使用简单方式直接发送数据
        client.start_recording()
