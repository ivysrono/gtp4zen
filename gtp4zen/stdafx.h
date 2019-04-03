// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::gregorian;
using namespace boost::posix_time;

#include "win32xx.h"
#include "lua/lua.hpp"

extern int g_zenver;
extern int g_threads;
extern int g_maxtime;
extern int g_strength;
extern string g_logfile;
extern bool g_debug;
extern int g_think_interval;
extern float g_think_level_1;
extern float g_think_level_2;
extern int g_think_level_0;
extern int g_resign;
extern bool g_chinese_rule;

extern int __ansi2num(char ch);
extern std::string __num2ansi(int x, int y, int boardsize);
extern void logprintf(const TCHAR *_Format, ...);

#define GTP4ZEN_VERSION			"0.3.5"
#define GTP4ZEN_COLOR_WHITE		1
#define GTP4ZEN_COLOR_BLACK		2
