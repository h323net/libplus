/*
* plussettings.h
*
* Copyright (c) 2015 Spranto International Pte Ltd. All Rights Reserved.
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, you can obtain one at http://mozilla.org/MPL/2.0/
*
* Software distributed under the License is distributed on an "AS IS"
* basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
* the License for the specific language governing rights and limitations
* under the License.
*
* Contributor(s): ______________________________________.
*
* $Id$
*
*/

#pragma once


#define LIBPLUS_MAKER  "h323plus.org"
#define LIBPLUS_PRODUCT "libPLUS"
#define LIBPLUS_VERSION  H323PLUS_VER

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#if defined(_MSC_VER)     // Windows
    #define LIB_COMPILER STR(_MSC_VER)
    #if defined(_WIN64)
    #define LIB_OS  "WIN64_" 
    #elif defined(_WIN32)
    #define LIB_OS  "WIN32_"
    #elif defined(_ARM)
    #define LIB_OS  "WINARM_" 
    #endif
#else
#if defined(P_ANDROID)
    #define LIB_COMPILER STR(__ANDROID_API__)
    #define LIB_OS  "ANDROID_" 
#endif
#endif  

#define LIBPLUS_OS  LIB_OS LIB_COMPILER

// Port Settings
#define defListenport   1720
#define lPortBase       50000
#define lPortMax        50100
#define tPortBase       2775
#define tPortMax        2780
#define uPortBase       2775
#define uPortMax        2778
#define mPortBase       2776

// Network Settings
#define defSTUNsrv    "spranto.net"
#define defGWsrv      "spranto.net"

#if _WIN32
#define pathExcept   "\\except.ini"
#else
#define pathExcept   "/except.ini"
#endif

// Media Rendering
#define defDisplayWidth  352
#define defDisplayHeight 288
#define defVideoFormat   "YUV420P"
#define defBufferSize   2048000

#define defAudioDelay 150

#define defTraceFile "libplus_trace.dat"
#define defUserName  "default"
#ifdef _DEBUG
#define defTraceLevel 6
#else
#define defTraceLevel 0
#endif

#define defLanguage "EN-US"


static PString defDBName        = "libplus.db";
static PString defDBPass        = "QeHvR96oFYNmLXaMqhSKJNEZjK63ftcQm4RNzYkzTVn";
static PString defDBTable       = "defaults";
static PString defDBId          = "default";
static PString defDBSection     = "defaultuser";
static PString defDBKey         = "user";
static PString defDBSetTable    = "settings";
static PString defDBSectVar     = "variables";


// Operating system default settings
#if _WIN32  
#define defdrvvideorec        "DirectShow"
#define defdrvvideoplay       "Window"
#define defdrvaudiorec        "WindowsMultimedia" //"DirectSound"  "WASAPI"
#define defdrvaudioplay       "WindowsMultimedia" //"DirectSound"  "WASAPI"
#define defApplicationDriver  "Application"
#else
#define defdrvvideorec        ""
#define defdrvvideoplay       ""
#define defdrvaudiorec        "" 
#define defdrvaudioplay       "" 
#define defApplicationDriver  ""
#endif




