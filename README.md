将zen6或者zen7转成命令行gtp。代码依赖boost（附boost props），开发环境vs2013update5。

使用了lua脚本可以自定义时间，代码写的很烂，没有版权，随便使用。

使用说明：

1. 把zen7的zen.dll拷贝到gtp4zen.exe所在的文件夹，运行即可，只需要gtp4zen.exe和zen.dll这2个文件
请参考gtp指令手册(http://www.lysator.liu.se/~gunnar/gtp/)或者使用sabaki等图形界面工具  
2. 如果使用zen6的zen.dll，需要设置参数 gtp4zen.exe -z6
3. gtp4zen.exe所在的文件夹如果存在gtp4zen.lua，命令行-T参数自动失效并使用gtp4zen.lua的时间控制，
这个脚本用于精细化时间控制，如果不明白用途可以删除gtp4zen.lua，文件存在即表示生效  
4. gtp4zen.exe --help输出帮助,-t是线程数（默认和cpu核数量相同），-T最大思考时间，-s最大计算步数，-l指定log文件，-d会输出调试信息到gtpshell，帮助信息如下：

C:\Users\clock\Desktop>gtp4zen.exe --help

    Options:
      -h [ --help ]             Show all allowed options.
      -z [ --zenverion ] arg    Version of zen.dll, must be 6 or 7. (default 7)
      -t [ --threads ] arg      Set the number of threads to use. (default
                                CPU_CORES)
      -T [ --maxtime ] arg      Set the max time for one move. (default 10 seconds)
      -s [ --strength ] arg     Set the max playout. (default 10000)
      -i [ --ithink ] arg       thinking interval, only set 100 when play cgos.
                                (default 100)
      -n [ --ilevel0 ] arg      factor0. (default 1)
      -o [ --ilevel1 ] arg      factor1. (default 1)
      -p [ --ilevel2 ] arg      factor2. (default 1)
      -r [ --resign ] arg       resign. (default 10)
      -l [ --logfile ] arg      Enable logging and set the log file. (default none)
      -L [ --logfilenametime ]  Add timestamp after log filename. (default off)
      -d [ --debug ]            Enable debug output to gtp shell. (default off)

其中 n,o,p 三个参数由你使用zen6还是zen7决定其含义，具体可参考Github项目GoAIRating  
以上3个参数如不指定，棋力将达不到预想值。  
如果你单纯需要最强的棋力，如果是zen6，设置-n1 -o1 -p1（即默认值），如果是zen7，设置-n3 -o1 -p0.75  
参数 r 设置胜率低于多少时认输


作者：拥剑：qq80101277