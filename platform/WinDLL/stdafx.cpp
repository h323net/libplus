
/*
 * stdafx.cpp : source file that includes just the standard includes
 *
 * Copyright (c) 2016 ISVO (Asia) Pte Ltd. All Rights Reserved.
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
 *
 */


#include "stdafx.h"

#ifdef _WIN32
    #ifdef _WIN64
        #ifndef _DEBUG
            #pragma comment(lib,"ptlib64s.lib")
            #pragma comment(lib,"h323plus.lib")
            #pragma comment(lib,"libPLUSs.lib")
        #else
            #pragma comment(lib,"ptlib64sd.lib")
            #pragma comment(lib,"h323plusd.lib")
            #pragma comment(lib,"libPLUSd.lib")
        #endif
    #else
        #ifndef _DEBUG
            #pragma comment(lib,"ptlibs.lib")
            #pragma comment(lib,"h323plus.lib")
            #pragma comment(lib,"libPLUSs.lib")
        #else
            #pragma comment(lib,"ptlibsd.lib")
            #pragma comment(lib,"h323plusd.lib")
            #pragma comment(lib,"libPLUSd.lib")
        #endif
    #endif
#endif
