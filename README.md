# FTP文件管理项目 
## 具体功能
     编写服务器端,服务器端启动,然后启动客户端,通过客户端可以输入以下命令进
     行服务器上的文件查看:
```
 cd 进入对应目录
 ls 列出相应目录文件
 upload 将本地文件上传至服务器
 download 文件名 下载服务器文件到本地
 remove 删除服务器上文件
 pwd 显示目前所在路径
 其他命令不响应
```
线程池一般由以下四部分组成
 线程管理器(Thread_pool)
 工作线程(Work_thread)
 任务接口(Task)
 任务队列(Task_queue)

## 实现
客户端读取命令后
