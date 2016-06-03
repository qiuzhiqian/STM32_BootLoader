# STM32_BootLoader
通过串口来实现bootloader功能，改程序以`stm32F103`为例,通过串口1完成，功能是当上点时，启动bootloader程序，同时开始5s倒数计时。
如果在计时时间内输入正确的密码，则进入bootloader，否则进入按品牌程序。当然，如果在进入app的过程中发现app不存在，仍然会进入bootloader。
