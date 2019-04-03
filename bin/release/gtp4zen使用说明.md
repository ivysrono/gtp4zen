0. 依赖 [VC++ 2015-2019 x86](https://aka.ms/vs/16/release/vc_redist.x86.exe)。

1. 把 Zen7 的 Zen.dll（文件名大小写不敏感，下同）拷贝到 gtp4zen.exe 所在的文件夹，即可运行，必需的只有这 2 个文件。

请参考 [GTP 指令手册](https://www.lysator.liu.se/~gunnar/gtp/)或者使用 [Sabaki](https://github.com/SabakiHQ/Sabaki/releases) 等图形界面工具。

2. gtp4zen.exe Options:
   -h [ --help ] Show all allowed options.
   -z [ --zenverion ] arg Version of Zen.dll, must be 6 or 7. (default 7)
   -t [ --threads ] arg Set the number of threads to use. (default CPU_CORES)
   -T [ --maxtime ] arg Set the max time for one move. (default 180 seconds)
   -s [ --strength ] arg Set the playing strength. (default 250000)
   -i [ --ithink ] arg thinking interval, only set 100 when play cgos. (default 1000)
   -n [ --ilevel0 ] arg factor0. (default 3)
   -o [ --ilevel1 ] arg factor1. (default 1.0)
   -p [ --ilevel2 ] arg factor2. (default 0.75)
   -r [ --resign ] arg resign. (default 20)
   -l [ --logfile ] arg Enable logging and set the log file. (default none)
   -L [ --logfilenametime ] Add timestamp after log filename. (default off)
   -d [ --debug ] Enable debug information output. (default off)

`-h` 输出帮助，`-t` 是线程数（默认与核芯数量相同），`-T` 最大思考时间，`-s` 最大计算步数，`-l` 指定记录文件，`-d` 输出更多信息。

如果使用 `Zen6` 的 `Zen.dll`，需要设置参数 `-z6`。如果需要同时使用 `Zen6` 和 `Zen7`，请在 gtp4zen.exe 所在的文件夹下再分别相应建立名为 `6` 和 `7` 的子目录，并放置相应的 `Zen.dll` 。可同时将第三方如奋奋狮子的 `Zen6GTP` 和 `Zen7GTP` 程序置于相应文件夹中，实现最高效利用。

`n`，`o`，`p` 三个参数的含义由使用的是 Zen6 还是 Zen7 决定，具体可参考 [GoAIRatings](https://github.com/breakwa11/GoAIRatings/blob/master/AIcmds.md#zen6-gtp4zen)，默认设置为 [Zen7 9D+](https://github.com/breakwa11/GoAIRatings/blob/master/AIcmds.md#zen7-gtp4zen)，即 `-n3 -o1 -p0.75`。

3. gtp4zen.exe 所在的文件夹如果存在 gtp4zen.lua，命令行 -T 参数自动失效并使用 gtp4zen.lua 的时间控制。

这个脚本用于精细化时间控制，仅供有编程能力的棋友使用。如果确实明白用途，可以将文件改名并自行修改，默认规则可能导致落子时间恒定为 15s 左右一步。

4. cgos 遛狗建议参数：`-T5 i100`，或者使用 gtp4zen.lua 脚本更精细地控制时间。
