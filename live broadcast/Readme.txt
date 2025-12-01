由于本程序是第三方

编译：

需要替换到rv1106b_rv1103b_linux_ipc_v1.0.0_20241016/media/samples/simple_test/simple_adec_bind_ao.c
simple_test文件夹下make命令编译


录音程序：
修改rv1106b_rv1103b_linux_ipc_v1.0.0_20241016/media/samples/simple_test/simple_adec_bind_ao.c可以影响rk_mpi_ai_test
rv1106b_rv1103b_linux_ipc_v1.0.0_20241016/media/rockit/build/mpi文件夹下sudo make就能编译出来
编译出来的程序存储在example/mod下


使用：

首先要在电脑端https://ossrs.net/lts/zh-cn/docs/v5/doc/windows
启动http://localhost:8080/live/livestream.flv
相当于把接收端打开

然后眼镜端启动程序前先确定你的ffmpeg可以启动，WiFi已经连好
simple_vi_bind_venc -u rtmp://192.168.2.7/live/livestream -w 640 -h 480
上面的IP地址换成你的主机IP