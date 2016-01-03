// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <stdio.h>

// vs2015 bug with _iob_func and OpenSSL
#if _MSC_VER == 1900  
extern "C" {
    FILE *__iob_func() { return NULL; }
}
#endif







