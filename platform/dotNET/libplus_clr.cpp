// This is the main DLL file.

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
