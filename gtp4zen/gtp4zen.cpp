﻿// gtp4zen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "compile_time.h"
#include "zen6gtp.h"
#include "zen7gtp.h"

using namespace boost::program_options;
static void _play_zen(CGtp *pgtp);

int g_zenver = 7;
int g_threads = 1;
int g_maxtime = 180;			// default 180 seconds
int g_strength = 25000;			// default 25000 strength
bool g_logfilenametime = false;
bool g_debug = false;
int g_think_interval = 200;
int g_think_level_0 = 3;
float g_think_level_1 = 1.0;
float g_think_level_2 = 0.75;
int g_resign = 20;
bool g_chinese_rule = true;

#ifdef _DEBUG
string g_logfile = "gtp4zen_log.txt";
#else
string g_logfile = "";
#endif

// 获取当前路径
inline std::string GetModuleFilePath()
{
	TCHAR buffer[MAX_PATH] = L"";
	GetModuleFileName(NULL, buffer, MAX_PATH);
	LPTSTR lpPos = _tcsrchr(buffer, _T('\\'));
	if (lpPos)
	{
		*(lpPos + 1) = _T('\0');
	}

	std::string result = CWtoA(buffer);
	return result;
	//return (LPSTR)CWtoA(buffer);
}

int _tmain(int argc, _TCHAR* argv[])
{
	SYSTEM_INFO  sysInfo;
	GetSystemInfo(&sysInfo);
	g_threads = sysInfo.dwNumberOfProcessors;

#if 1
	// 命令行解析(使用boost)
	options_description opts("Options");
	opts.add_options()
		("help,h", "Show all allowed options.")
		("zenverion,z",value<int>(),    "Version of zen.dll, must be 6 or 7. (default 7)")
		("threads,t",  value<int>(),    "Set the number of threads to use. (default CPU_CORES)")
		("maxtime,T",  value<int>(),    "Set the max time for one move. (default 180 seconds)")
		("strength,s", value<int>(),    "Set the playing strength. (default 25000)")
		("ithink,i", value<int>(),      "thinking interval, only set 100 when play cgos. (default 100)")
		("ilevel0,n", value<int>(),     "factor0. (default 3)")
		("ilevel1,o", value<float>(),   "factor1. (default 1.0)")
		("ilevel2,p", value<float>(),   "factor2. (default 0.75)")
		("resign,r", value<int>(),      "resign. (default 10)")
		("logfile,l", value<string>(),  "Enable logging and set the log file. (default none)")
		("logfilenametime,L",           "Add timestamp after log filename. (default off)")
		("debug,d",                     "Enable debug output to gtp shell. (default off)")
		("jpn_rules",                   "Set to Japanese rules. (default Chinese rules)")
		;

	try {
		bool error_options = false;;
		variables_map vm;
		store(parse_command_line(argc, argv, opts), vm);
		if (vm.count("help")) {
			cout << opts << endl;
			return 0;
		}
		if (vm.count("logfilenametime")) {
			g_logfilenametime = true;
		}
		if (vm.count("debug")) {
			g_debug = true;
		}
		if (vm.count("zenverion")) {
			// zen.dll版本
			g_zenver = vm["zenverion"].as<int>();
			if (6 != g_zenver && 7 != g_zenver) {
				error_options = true;
			}
		}
		if (vm.count("threads")) {
			g_threads = vm["threads"].as<int>();
			if (g_threads <= 0) {
				error_options = true;
			}
		}
		if (vm.count("maxtime")) {
			g_maxtime = vm["maxtime"].as<int>();
			if (g_maxtime <= 0) {
				error_options = true;
			}
		}
		if (vm.count("strength")) {
			g_strength = vm["strength"].as<int>();
			if (g_strength <= 0) {
				error_options = true;
			}
		}
		if (vm.count("ithink")) {
			g_think_interval = vm["ithink"].as<int>();
			if (g_think_interval <= 0) {
				g_think_interval = 200;
			}
		}
		if (vm.count("ilevel0")) {
			g_think_level_0 = vm["ilevel0"].as<int>();
		}
		if (vm.count("ilevel1")) {
			g_think_level_1 = vm["ilevel1"].as<float>();
		}
		if (vm.count("ilevel2")) {
			g_think_level_2 = vm["ilevel2"].as<float>();
		}
		if (vm.count("resign")) {
			g_resign = vm["resign"].as<int>();
		}
		if (vm.count("logfile")) {
			g_logfile = vm["logfile"].as<string>();
		}
		if (vm.count("jpn_rules")) {
			g_chinese_rule = false;
		}

		if (error_options) {
			cerr << "ERROR: unknown, check by gtp4zen --help" << endl;
			return 0;
		}
	} catch (exception &e) {
		cerr << "ERROR: " << e.what() << endl;
		return 0;
	} catch (...) {
		cerr << "ERROR: unknown, check by gtp4zen --help" << endl;
		return 0;
	}
#endif

	// 文件名是否加时间
	string filename = g_logfile;
	if (g_logfilenametime) {
		const char *p1 = strrchr(filename.c_str(), '/');
		const char *p2 = strrchr(filename.c_str(), '\\');
		const char *p3 = strrchr(filename.c_str(), '.');

		SYSTEMTIME sys;
		GetLocalTime(&sys);
		char timestr[64] = "";
		sprintf_s(timestr
			, 64
			, "_%d-%02d-%02d_%02d-%02d-%02d"
			, sys.wYear, sys.wMonth, sys.wDay
			, sys.wHour, sys.wMinute, sys.wSecond
			);

		bool add_tail = false;
		if (p1 && p3) {
			if (p3 > p1)
				add_tail = false;
			else
				add_tail = true;
		} else if (p2 && p3) {
			if (p3 > p2)
				add_tail = false;
			else
				add_tail = true;
		} else {
			if (p3)
				add_tail = false;
			else
				add_tail = true;
		}

		if (add_tail) {
			filename += timestr;
		} else {
			boost::replace_last(filename, ".", timestr + std::string("."));
		}
		g_logfile = filename;
	}

	logprintf(L"%s", L"========程序gtp4zen启动========");
	logprintf(L"CPU核数：%d", sysInfo.dwNumberOfProcessors);
	logprintf(L"ZEN版本：%d", g_zenver);
	logprintf(L"线程数量：%d", g_threads);
	logprintf(L"每手最大思考时间（秒）：%d", g_maxtime);
	logprintf(L"每手最大模拟步数（步）：%d", g_strength);

	std::string zen_dll_path = GetModuleFilePath();
	std::string lua_engine_path = GetModuleFilePath() + "gtp4zen.lua";
	if (7 == g_zenver && boost::filesystem::is_regular_file((zen_dll_path + "7\\zen.dll").c_str())) {
	}
	else if (7 == g_zenver && boost::filesystem::is_regular_file((zen_dll_path + "6\\zen.dll").c_str())) {
		g_zenver = 6;
	}
	else if (6 == g_zenver && boost::filesystem::is_regular_file((zen_dll_path + "6\\zen.dll").c_str())) {
	}
	else if (boost::filesystem::is_regular_file((zen_dll_path + "zen.dll").c_str())) {
	}
	else {
		fprintf(stderr, "ERROR: zen.dll not exist?\n");
		logprintf(L"%s", L"ERROR: zen.dll not exist?");
		//system("pause");
		return 1;
	}

	if (6 == g_zenver) {
		CZen6Gtp gtp6;
		CGtp *pgtp = &gtp6;
		if (pgtp->load((zen_dll_path + "6\\zen.dll").c_str(), lua_engine_path.c_str()))
			_play_zen(pgtp);
		else if (pgtp->load((zen_dll_path + "zen.dll").c_str(), lua_engine_path.c_str()))
			_play_zen(pgtp);
		pgtp->unload();
	} else if (7 == g_zenver) {
		CZen7Gtp gtp7;
		CGtp *pgtp = &gtp7;
		if (pgtp->load((zen_dll_path + "7\\zen.dll").c_str(), lua_engine_path.c_str()))
			_play_zen(pgtp);
		else if (pgtp->load((zen_dll_path + "zen.dll").c_str(), lua_engine_path.c_str()))
			_play_zen(pgtp);
		pgtp->unload();
	}

	return 0;
}

static void _play_zen(CGtp *pgtp)
{
	std::string result;
	std::string info_result;
	while (true) {
		std::string line;
		std::getline(cin, line);

		boost::trim_if(line, boost::is_any_of("\r\n\t "));
		std::vector<std::string> list;
		boost::split(list, line, boost::is_any_of("\r\n\t "), boost::token_compress_on);
		if (line.size() == 0)
			continue;
		result = "";
		info_result = "";
		if (0 == list.size()) {
			continue;
		} else if ("list_commands" == list[0]) {
			result = pgtp->list_commands();
		} else if ("name" == list[0]) {
			result = pgtp->name();
		} else if ("version" == list[0]) {
			result = pgtp->version();
		} else if ("protocol_version" == list[0]) {
			result = pgtp->protocol_version();
		} else if ("boardsize" == list[0] && list.size() >= 2) {
			result = pgtp->boardsize(atoi(list[1].c_str()));
		} else if ("quit" == list[0]) {
			fprintf(stdout, "=\n\n");
			fflush(stdout);
			break;
		} else if ("clear_board" == list[0]) {
			result = pgtp->clear_board();
		} else if ("komi" == list[0] && list.size() >= 2) {
			result = pgtp->komi((float)std::atof(list[1].c_str()));
		} else if ("play" == list[0] && list.size() >= 3) {		// play w Q17
			result = pgtp->play(list[1].c_str(), list[2].c_str());
		} else if ("genmove" == list[0] && list.size() >= 2) {		// genmove b
			result = pgtp->genmove(list[1].c_str());
			float winrate = pgtp->get_winrate();
			if (winrate > -1 && winrate < 2 && result != "pass") {
				char buff[1024];
				if (list[1] == "W" || list[1] == "w")
					winrate = 1 - winrate;
				sprintf_s(buff, "Black winrate: %.2f", winrate * 100);
				info_result = buff;
			}
		} else if ("place_free_handicap" == list[0] && list.size() >= 2) {
			std::vector<std::string> posarray;
			for (auto it = list.begin() + 1; it != list.end(); ++it) {
				posarray.push_back(*it);
			}
			result = pgtp->free_handicap(posarray);
		} else if ("set_free_handicap" == list[0] && list.size() >= 2) {
			std::vector<std::string> posarray;
			for (auto it = list.begin() + 1; it != list.end(); ++it) {
				posarray.push_back(*it);
			}
			result = pgtp->free_handicap(posarray);
		} else if ("winrate" == list[0]) {
			result = pgtp->winrate();
		} else if ("undo" == list[0]) {
			result = pgtp->undo();
		} else if ("time_settings" == list[0] && list.size() >= 4) {
			// 设置总时间
			result = pgtp->time_settings(atoi(list[1].c_str())
				, atoi(list[2].c_str())
				, atoi(list[3].c_str())
				);
		} else if ("time_left" == list[0] && list.size() >= 4) {
			// 每次对方或自己落子就更新自己的时间
			result = pgtp->time_left(list[1].c_str()
				, atoi(list[2].c_str())
				, atoi(list[3].c_str())
				);
		} else if ("final_score" == list[0]) {
			result = pgtp->score();
		} else {
			result = "? unknown command [" + line + "]\n";
		}
		fprintf(stdout, "%s\n", result.c_str());
		//logprintf(L"command: %s", line.c_str());
		//logprintf(L"result: %s", result.c_str());
		fflush(stdout);
		if (info_result.size() > 0)
		{
			int ret = fprintf(stderr, "%s\n", info_result.c_str());
			if (ret > 0)
				fflush(stderr);
		}
	}
}

int __ansi2num(char ch)
{
	const char *axes = "ABCDEFGHJKLMNOPQRSTUVWXYZ";
	const char *find = strchr(axes, ch);
	if (!find)
		return -1;
	else
		return find - axes;
}

std::string __num2ansi(int x, int y, int boardsize)
{
	if (x < 0 || y < 0 || boardsize <= 0 || boardsize > 25)
		return "";

	const char array1[] = "ABCDEFGHJKLMNOPQRSTUVWXYZ";
	char buffer[32] = "";
	sprintf_s(buffer, 32, "%c%d", array1[x], boardsize - y);
	return buffer;
}

void logprintf(const TCHAR *_Format, ...)
{
	// 可变参数
	TCHAR pszBuffer[1024] = L"";
	va_list ap;
	va_start(ap, _Format);
	vswprintf_s(pszBuffer, 1024, _Format, ap);
	va_end(ap);

	// 写文件
	ofstream out;
	if (g_logfile.size() > 0) {
		out.open(g_logfile.c_str(), ios::app | ios::out | ios::binary);
		if (out.is_open()) {
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			TCHAR timestr[128] = L"";
			wsprintf(timestr
				, L"%d-%02d-%02d %02d:%02d:%02d.%04d | "
				, sys.wYear, sys.wMonth, sys.wDay
				, sys.wHour, sys.wMinute, sys.wSecond
				, sys.wMilliseconds
				);
			out << CWtoA(timestr);
			out << CWtoA(pszBuffer);
			out << "\r\n";
			out.close();
		}
	}
}

