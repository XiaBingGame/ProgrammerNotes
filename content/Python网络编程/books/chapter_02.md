# 第二章 UDP
* 端口分类
    - 知名端口 0-1023
    - 注册端口 1024-49151
    - 其余端口靠 49152-65535
* 查询域名服务的端口号
```
import socket
socket.getservbyname('domain')
```
* socket.socket 对象表示套接字
