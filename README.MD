###计划写一个网络库，主要包含一下处理

-----------------------

1. 基本网络io封装，read，write，epoll connect accept 等
2. 多线程模式
3. 包含定时器
4. 针对日志的处理

-----------------------

#### 已经完成

+ 实现日志
>具体类
>+ LogStream
>+ Logger

+ 实现对socket中 一些常见选项的封装，包括
> + accept accept4
> + bind
> + listen
> + socket
> + read
> + write
> + close
> + shutdownWrite
> + socket error
> + 设置非阻塞
> + 设置NoDelay
> + 复用地址
> + 复用端口
> + KeepAlive

+ 网络地址的处理
> + 获取对端地址
> + 获取本端地址
> + 获取ip
> + 获取port

+ mutex cond 封装
+ 完成loop，channel，tcpconnection
+ 实现时间日期相关处理

+ 实现定时器

-----------------------

#### 下一步工作计划：

+ connecter server类

-----------------------

#### 计划时间：

+ 完成雏形，可以投入使用：编写一个http服务器
+ 添加单元测试
+ 经过测试，并且达到一定的稳定性


#### 准备添加的组件
+ 更灵活的日志库
+ 简单 encode buf 的处理，实时上这一部分是应该属于序列化，
+ 线程安全队列（要求有性能测试）

