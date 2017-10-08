#2011-09-06__星期二 add by huanghaibo
1.在命令行运行cf.pl ddk
2.如果该目录下没有source和makefile文件则运行c:\bin\gen_ddk_build_files.pl再运行
c:\bin\ddk_2600_cmdline.pl否则直接运行c:\bin\ddk_2600_cmdline.pl
3.选择末行C:\WINDOWS\system32\cmd.exe /k D:\WINDDK\2600\bin\setenv.bat D:\WINDDK\2600 fre
4.输入工程所在的盘符如(e:),运行build命令
5.在工程中会生成.sys文件

