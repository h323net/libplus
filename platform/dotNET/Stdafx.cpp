// stdafx.cpp : source file that includes just the standard includes
// libplus.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _WIN32
    #ifdef P_64BIT
        #ifndef _DEBUG
        #pragma comment(lib,"ptlib64s.lib")
        #pragma comment(lib,"h323plus.lib")
        #else
        #pragma comment(lib,"ptlib64sd.lib")
        #pragma comment(lib,"h323plusd.lib")
        #endif
    #else
        #ifndef _DEBUG
        #pragma comment(lib,"ptlibs.lib")
        #pragma comment(lib,"h323plus.lib")
        #pragma comment(lib,"libPLUS.lib")
        #else
        #pragma comment(lib,"ptlibsd.lib")
        #pragma comment(lib,"h323plusd.lib")
        #pragma comment(lib,"libPLUSd.lib")
        #endif
    #endif
#endif
