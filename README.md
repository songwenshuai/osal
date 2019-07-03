# 为什么这样做

大学的时候用过 TI 的 ZigBee 和 BLE，里面都有一个 轮询式的操作系统 OSAL，小巧灵活，并且适合学习。在单片机上开发大家都了解，一般的开发流程是，修改代码 -> 编译(速度慢) -> 下载 -> 复位单片机 -> 看现象。从改程序到看到现象步骤繁多，容易让初学者在繁杂的步骤中失去信心。当时为了克服这个学习困难把 OSAL 抽离了出来，放在了 VS 环境下。编译速度和学习的方便程度都有很大提高 。最近在整理东西，再次把它拿出来，希望对大家有所帮助。

注 : 因为OSAL组件的不同，这次移植参考了 CC2541的 协议栈源码，CC2538的协议栈源码，和CC2630的协议栈源码, 争取在剥离出来的时候做到对官方源码的最少的改动，对官方代码模块的最大包含。

# 移植特点

保留了原来 TI 代码的结构不变，包括了 任务管理、NV 系统、内存管理、电源管理、定时器、回调定时器、时钟管理等。模拟器部分调用了 Windows API 函数为 OSAL 提供 320us 计数器。这一部分采用 Windows 精确计数器，精度达到 MS 级，这一部分的原理请参考我主页的另外[一篇文章](<https://zhuanlan.zhihu.com/p/70258432>)。

电源管理效果在模拟器上不容易体现，这里在 OSAL 空闲的时候调用 Sleep 用来降低CPU占用。

# 如何开始
点[这里](https://github.com/songwenshuai/OSAL)下载源码。

>  DEV C++ 用户，解压打开 OSAL.dev，我的 DEV C++ 版本是 5.11，编译环境是 TDM-GCC 4.9.2 32-bit Release

> Visual Studio 用户，解压打开 OSAL.vcxproj，点击全部保存，提示保存解决方案 .sln，请保存在 OSAL.vcxproj 的上一级目录。我的 visual studio 版本是 2017，其他的版本应该也能轻松编译。

# 文件列表
```c
│  .gitignore
│  LICENSE
│  OSAL API.pdf
│  README.md
│
└─OSAL
    │  OSAL.vcxproj
    │  OSAL.vcxproj.filters
    │
    └─Source
        ├─App
        │      GenericApp.c
        │      GenericApp.h
        │      OSAL_GenericApp.c
        │
        ├─Core
        │  ├─Inc
        │  │      OSAL.h
        │  │      OSAL_Bufmgr.h
        │  │      OSAL_Cbtimer.h
        │  │      OSAL_Clock.h
        │  │      OSAL_Comdef.h
        │  │      OSAL_Debug.h
        │  │      OSAL_Hal.h
        │  │      OSAL_List.h
        │  │      OSAL_Macro.h
        │  │      OSAL_Memory.h
        │  │      OSAL_Nv.h
        │  │      OSAL_Printf.h
        │  │      OSAL_PwrMgr.h
        │  │      OSAL_Queue.h
        │  │      OSAL_Ring.h
        │  │      OSAL_Tasks.h
        │  │      OSAL_Timers.h
        │  │      OSAL_Types.h
        │  │
        │  └─Src
        │          OSAL.c
        │          OSAL_Bufmgr.c
        │          OSAL_Cbtimer.c
        │          OSAL_Clock.c
        │          OSAL_Hal.c
        │          OSAL_List.c
        │          OSAL_Memory.c
        │          OSAL_Nv.c
        │          OSAL_Printf.c
        │          OSAL_PwrMgr.c
        │          OSAL_Queue.c
        │          OSAL_Ring.c
        │          OSAL_Timers.c
        │
        └─Simulation
                main.c
```

如何得到目录树

```bash
 tree OSAL /F
```

> Core 目录下是 OSAL 轮询操作系统源码和一些通用代码。

> App 目录下是一个示例任务。

> Simulation 目录下放着仿真相关代码。

# 有什么现象
编译运行之后在控制台每一秒打印一次 信息

```
deviation = 0
weite_nv  = 0007
reads_nv  = 0007
messages  = 0007
status    = 0007

deviation = 0
weite_nv  = 0008
reads_nv  = 0008
messages  = 0008
status    = 0008

deviation = 0
weite_nv  = 0009
reads_nv  = 0009
messages  = 0009
status    = 0009

deviation = 0
weite_nv  = 0010
reads_nv  = 0010
messages  = 0010
status    = 0010

cb timer Test
deviation = 0
weite_nv  = 0011
reads_nv  = 0011
messages  = 0011
status    = 0011
```

deviation 是 OSAL 轮询误差值 ( 单位 MS )，在定时任务周期设置是 1000 MS的情况下 。(deviation  = 这次事件发生时间 (系统时间) - 上次事件发生时间 (系统时间)，代表 OSAL 模拟器轮询的精度)

weite_nv 、reads_nv 为 NV 系统读写测试打印。

messages、status 消息传递测试。

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

