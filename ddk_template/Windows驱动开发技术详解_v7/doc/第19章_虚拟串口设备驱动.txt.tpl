
#2011_09_07   星期三   add by greshem

第19章 虚拟串口设备驱动
　　	本章介绍了串口开发的框架模型，在串口的AddDevice例程中需要暴露出一个串口的符号连接，另外在相应的注册表中需要进行设置。在串口与应用程序的通信中，主要是一组DDK定义的IO控制码，这些IO控制码负责由应用程序向驱动发出请求。
########################################################################
