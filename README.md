# 为什么这样做

大学的时候用过 TI 的 ZigBee 和 BLE，里面都有一个 轮询式的操作系统 OSAL，小巧灵活，并且适合学习。在单片机上开发大家都了解，一般的开发流程是，修改代码 -> 编译(速度慢) -> 下载 -> 复位单片机 -> 看现象。从改程序到看到现象步骤繁多，容易让初学者在繁杂的步骤中失去信心。当时为了克服这个学习困难把 OSAL 抽离了出来，放在了 VS 环境下。编译速度和学习的方便程度都有很大提高 。最近在整理东西，再次把它拿出来，希望对大家有所帮助。

注 : 因为OSAL组件的不同，这次移植参考了 CC2541的 协议栈源码，CC2538的协议栈源码，和CC2630的协议栈源码, 争取在剥离出来的时候做到对官方源码的最少的改动，对官方代码模块的最大包含。

# 移植特点

保留了原来 TI 代码的结构不变，包括了 任务管理、NV 系统、内存管理、电源管理、定时器、回调定时器、时钟管理等。模拟器部分调用了 Windows API 函数为 OSAL 提供 320us 计数器。这一部分采用 Windows 精确计数器，精度达到 MS 级，这一部分的原理请参考我主页的另外[一篇文章](<https://zhuanlan.zhihu.com/p/70258432>)。

电源管理部分在 OSAL 空闲的时候调用 Sleep 函数用来降低CPU占用。

# 如何开始
点[这里](https://github.com/songwenshuai/OSAL)下载源码。

> Visual Studio 用户，解压打开 OSAL.vcxproj，点击全部保存，提示保存解决方案 .sln。我的 visual studio 版本是 2019，其他的版本应该也能轻松编译。

# 文件列表
```c
│  .gitignore
│  LICENSE.md
│  README.md
│
└─Source
    │  OSAL.vcxproj
    │  OSAL.vcxproj.filters
    │
    ├─App
    │      GenericApp.c
    │      GenericApp.h
    │      OSAL_GenericApp.c
    │      OSAL_Main.c
    │
    ├─Core
    │  ├─Inc
    │  │      OSAL.h
    │  │      OSAL_Bufmgr.h
    │  │      OSAL_Cbtimer.h
    │  │      OSAL_Clock.h
    │  │      OSAL_Comdef.h
    │  │      OSAL_Debug.h
    │  │      OSAL_List.h
    │  │      OSAL_Memory.h
    │  │      OSAL_Nv.h
    │  │      OSAL_Printf.h
    │  │      OSAL_PwrMgr.h
    │  │      OSAL_Queue.h
    │  │      OSAL_Ring.h
    │  │      OSAL_Tasks.h
    │  │      OSAL_Timers.h
    │  │
    │  └─Src
    │          OSAL.c
    │          OSAL_Bufmgr.c
    │          OSAL_Cbtimer.c
    │          OSAL_Clock.c
    │          OSAL_List.c
    │          OSAL_Memory.c
    │          OSAL_Nv.c
    │          OSAL_Port.c
    │          OSAL_Printf.c
    │          OSAL_PwrMgr.c
    │          OSAL_Queue.c
    │          OSAL_Ring.c
    │          OSAL_Timers.c
    │
    └─Doc
            OSAL API.pdf
            Z-Stackapi函数.pdf
            z-stack代码分析--osalInitTasks函数.pdf
            Z-Stack操作系统抽象层应用程序编程接口(中).pdf
            ZigBee操作系统抽象层应用程序编程接口.pdf
            ZStack-OSAL中文说明.pdf
            ZStack-OSAL中的系统定时任务处理.pdf
            Zstack中如何实现自己的任务.pdf
            zstack系统框架.pdf
            一图读懂ZStack控制核心OSAL的基本架构和工作机理.pdf
            堆内存管理（中）.pdf
            自己理解zigbee有关的数据的发送和接收.pdf
```

如何得到目录树

```bash
 tree OSAL /F
```

> Core 目录下是 OSAL 轮询操作系统源码和一些通用代码。

> App 目录下是一个示例任务。

# 有什么现象
编译运行之后在控制台每一秒打印一次 信息

```
deviation = 961us
weite_nv  = 0001
reads_nv  = 0001
ltoa_num  = 2147483648
ltoa_num  = 0x80000000
rand      = 1012484
messages  = 0001
rcv       = message
```

deviation 是 OSAL 轮询误差值 ( 单位 us )，在定时任务周期设置是 1000 MS的情况下 。(deviation  = 这次事件发生时间 (系统时间) - 上次事件发生时间 (系统时间) 的绝对值，代表 OSAL 模拟器轮询的精度)

weite_nv 、reads_nv 为 NV 系统读写测试打印。

messages 消息传递测试。

cb timer Test 5s 一次的回调定时器任务。

# 我想让它跑在单片机上
> 准备工作：你需要调通串口，让它可以打印字符。
>

> 设置一个定时器 1ms 中断一次，在中断函数中调用 osalAdjustTimer();

> 或者设置一个计数器 每 320us 计数一次, 在主循环中调用 osalTimeUpdate()(模拟器采用的是这种方法)。

> 然后完成两个函数 SysTickIntEnable 和 SysTickIntDisable。

> 完成 _putchar 函数

# 我该怎么使用OSAL
请参阅 TI [官方文档](<https://github.com/songwenshuai/OSAL/blob/master/OSAL API.pdf>)。

# 如何联系我

- 邮箱：[songwenshuai@sina.com](mailto:songwenshuai@sina.com)
- 主页：[songwenshuai](<https://github.com/songwenshuai>)
- 仓库：[Github](<https://github.com/songwenshuai>)

- 专栏：[zhuanlan](<https://zhuanlan.zhihu.com/songwenshuai>)

