/*
* libplus_clr.cxx
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

#pragma once

#include "libplus_clr.h"

///////////////////////////////////////////////////////////////////////

#pragma unmanaged

#define libNETEVT(name) \
case PLUSdevice::e_##name: name##_cb(); break;

#define libNETEVT1(name) \
case PLUSdevice::e_##name: name##_cb(p1); break;

void CLR_PLUSdevice::Event(int evtID, const char * p1, const char * p2, const char * p3, const char * p4) 
{  

    switch (evtID) {
        libNETEVT1(status)
        libNETEVT1(isinitialised)
        libNETEVT1(videoframe)
        libNETEVT1(callerid)
        libNETEVT1(incomingcall)
        libNETEVT1(incall)
        libNETEVT1(encryption) 
        libNETEVT1(farEndCameraInstruct)
        libNETEVT1(realTimeTextInstruct)
        libNETEVT1(farEndMotoisedInstruct)
        libNETEVT1(PhoneNumberEvent)
        libNETEVT1(URIaddressEvent)
        libNETEVT1(presenceSupport)
        libNETEVT(duplicate)
        libNETEVT1(forwardCall)
        libNETEVT1(dhGenerate);
        // IMPL: Event Name here
        default: break;
    }
};

///////////////////////////////////////////////////////////////////////

#pragma managed

namespace libplus {

    libPLUS::libPLUS()
        : m_Impl(new CLR_PLUSdevice)
    {
        initialize();
    }

    libPLUS::~libPLUS() 
    {
        delete m_Impl;
    }

    libPLUS::!libPLUS() 
    {
        delete m_Impl;
    }

}
