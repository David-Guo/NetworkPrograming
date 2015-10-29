# 一个简单的 tcp echo servie 框架


## Serveric 

The programs do the following:

1. The client reads a line from its standard input and writes the line to the server.
2. The server reads a line from its network input and ehcoes the line added with "from the server" back to the client
3. Tht client reads the echoed line and prints it on its standard output.

## Introduction

server 同过ip地址唯一指定机器，通服务端口号，唯一指定进程。
只要client 知道server ip地址和端口号，就能正常访问。

`inet.h` 头文件中指定了 server 的 ip 地址与服务端口，

## Bug

目前 server 端没有问题，可以通过指令:

    `telnet 127.0.0.1 12345`

正常访问服务器。

clinet 端依旧存在无法 connect to server 的问题。


## Reference

[TCP/IP socket 范例代码](http://puremonkey2010.blogspot.tw/2012/07/networkprg-tcp-clientserver.html)
这篇文章的评论部分介绍了防火墙部分
