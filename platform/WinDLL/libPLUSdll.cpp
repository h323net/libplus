
/*
 * libPLUSdll.cpp
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
#include "libPLUSdll.h"


////////////////////////////////////////////////////////////////////////////////

#define libDLLEvtOpt(name) \
case PLUSdevice::e_##name: m_dev.on##name(); break;

#define libDLLEvtOpt1(name) \
case PLUSdevice::e_##name: m_dev.on##name(p1); break;

#define libDLLEvtOpt2(name) \
case PLUSdevice::e_##name: m_dev.on##name(p1,p2); break;

#define libDLLEvtOpt3(name) \
case PLUSdevice::e_##name: m_dev.on##name(p1,p2,p3); break;

#define libDLLEvtOpt4(name) \
case PLUSdevice::e_##name: m_dev.on##name(p1,p2,p3,p4); break;

#define libDLLMediaDev(name) \
virtual bool out##name(const void * data, int size, int width=0, int height=0) {  return m_dev.out##name(data, size, width, height);  }


class DLL_PLUSdevice : public PLUSdevice
{

public:

    DLL_PLUSdevice(libPLUS & dev)
        : m_dev(dev) {}


    // Events
    virtual void Event(int evtID, const char * p1 = "", const char * p2 = "", const char * p3 = "", const char * p4 = "")
    {
        if (m_dev.HandleEvent(evtID, p1, p2, p3, p4))
            return;

        switch (evtID) {
            libDLLEvtOpt2(status)
            libDLLEvtOpt1(isinitialised)
            //libDLLEvtOpt1(notused1)
            libDLLEvtOpt1(callerid)
            libDLLEvtOpt1(incomingcall)
            libDLLEvtOpt1(incall)
            libDLLEvtOpt1(encryption)
            libDLLEvtOpt1(farEndCameraInstruct)
            libDLLEvtOpt1(realTimeTextInstruct)
            libDLLEvtOpt1(farEndMotoisedInstruct)
            libDLLEvtOpt1(PhoneNumberEvent)
            libDLLEvtOpt1(URIaddressEvent)
            libDLLEvtOpt1(presenceSupport)
            libDLLEvtOpt(duplicate)
            libDLLEvtOpt1(forwardCall)
            libDLLEvtOpt1(dhGenerate);
            // IMPL: Event Names here
            default: break;
        }
    }

    // Media
    libDLLMediaDev(audio)
    libDLLMediaDev(video)
    libDLLMediaDev(content)


private:
    libPLUS & m_dev;

};

////////////////////////////////////////////////////////////////////////////////

#define libDLLSetBody(name) \
const char * libPLUS::get##name() \
     { if (m_Impl) return m_Impl->Get_Value(PLUSdevice::e_##name); \
       else return "NotLoaded!"; } \
void libPLUS::set##name(const char * value) \
    {  if (m_Impl) m_Impl->Set_Value(PLUSdevice::e_##name, value); }


#define libDLLMethBody0(name) \
void libPLUS::do##name() { if (m_Impl) m_Impl->Call(PLUSdevice::e_##name);  }

#define libDLLMethBody1(name) \
void libPLUS::do##name(const char * str1) { if (m_Impl) m_Impl->Call(PLUSdevice::e_##name, str1); }

#define libDLLMethBody2(name) \
void libPLUS::do##name(const char * str1, const char * str2) { if (m_Impl) m_Impl->Call(PLUSdevice::e_##name, str1, str2); }

#define libDLLMethBody3(name) \
void libPLUS::do##name(const char * str1, const char * str2, const char * str3) { if (m_Impl) m_Impl->Call(PLUSdevice::e_##name, str1, str2, str3); }

#define libDLLMethBody4(name) \
void libPLUS::do##name(const char * str1, const char * str2, const char * str3, const char * str4) { if (m_Impl) m_Impl->Call(PLUSdevice::e_##name, str1, str2, str3, str4); }

#define libDLLEvtBody(name) \
void libPLUS::on##name() {}

#define libDLLEvtBody1(name) \
void libPLUS::on##name(const char * str1) {}

#define libDLLEvtBody2(name) \
void libPLUS::on##name(const char * str1, const char * str2) {}

#define libDLLEvtBody3(name) \
void libPLUS::on##name(const char * str1, const char * str2, const char * str3) {}

#define libDLLEvtBody4(name) \
void libPLUS::on##name(const char * str1, const char * str2, const char * str3, const char * str4) {}

#define libDLLMediaBody(name) \
bool libPLUS::in##name(void * data, int size, int width, int height) {  return m_Impl->in##name(data, size, width, height); } \
bool libPLUS::out##name(const void * data, int size, int width, int height) {  return false;  }


libPLUS::libPLUS() : m_Impl(NULL) {  }

libPLUS::~libPLUS() { Unload(); }


void libPLUS::Load()
{
    m_Impl = new DLL_PLUSdevice(*this);

    do {
        Sleep(100);
    } while (m_Impl->IsLoading());
}


void libPLUS::Unload()
{
    if (m_Impl) {
        delete m_Impl;
        m_Impl = NULL;
    }
}

// Settings
libDLLSetBody(version)
libDLLSetBody(tracing)
libDLLSetBody(username)
libDLLSetBody(password)
libDLLSetBody(server)
libDLLSetBody(quality)
libDLLSetBody(accessability)
libDLLSetBody(content)
libDLLSetBody(autoanswer)

libDLLSetBody(drvvideoplay)
libDLLSetBody(drvvideorec)
libDLLSetBody(drvaudioplay)
libDLLSetBody(drvaudiorec)
libDLLSetBody(curdrvvideoplay)
libDLLSetBody(curdrvvideorec)
libDLLSetBody(curdrvaudioplay)
libDLLSetBody(curdrvaudiorec)
libDLLSetBody(devvideoplay)
libDLLSetBody(devvideorec)
libDLLSetBody(devaudioplay)
libDLLSetBody(devaudiorec)
libDLLSetBody(audioplay)
libDLLSetBody(audiorec)
libDLLSetBody(videoplay)
libDLLSetBody(videorec)

libDLLSetBody(call)
libDLLSetBody(audiomute)
libDLLSetBody(videomute)
//libDLLSetBody(framewidth)
//libDLLSetBody(frameheight)
libDLLSetBody(callstate)
libDLLSetBody(farEndCameraSupport)
libDLLSetBody(realTimeTextSupport)
libDLLSetBody(farEndMotorizedSupport)
libDLLSetBody(farEndCameraInCall)
libDLLSetBody(realTimeTextInCall)
libDLLSetBody(farEndMotorizedInCall)
libDLLSetBody(userPhoneNumber)
libDLLSetBody(userURIAddress)
libDLLSetBody(showlocalvideo)
//libDLLSetBody(instanceid)  - Not required 
libDLLSetBody(initialised)
libDLLSetBody(language)
libDLLSetBody(listenport)
libDLLSetBody(videoformats)
libDLLSetBody(videoinformat)
libDLLSetBody(videooutformat)
libDLLSetBody(secondvideo)

libDLLSetBody(encryptSignal)
libDLLSetBody(encryptMedia)
libDLLSetBody(encryptMediaHigh)
// IMPL: Setting Names Here


// Methods
libDLLMethBody1(placeCall)
libDLLMethBody0(hangupCall)
libDLLMethBody0(answerCall)
libDLLMethBody1(sendDTMF)
libDLLMethBody3(farEndCamera)
libDLLMethBody0(start)
libDLLMethBody3(farEndMotorized)
libDLLMethBody1(realTimeTextInput)
libDLLMethBody0(realTimeTextNewLine)
libDLLMethBody1(secondCall)
libDLLMethBody0(stop)
libDLLMethBody3(videosize)
libDLLMethBody4(audioformat)
libDLLMethBody0(dhParameters)
// IMPL: Method Names here

// Events
bool libPLUS::HandleEvent(int id, const char * /*str1*/, 
                         const char * /*str2*/, const char * /*str3*/, const char * /*str4*/)
{
    return false;
}

libDLLEvtBody2(status)
libDLLEvtBody1(isinitialised)
//libDLLEvtBody1(notused1)
libDLLEvtBody1(callerid)
libDLLEvtBody1(incomingcall)
libDLLEvtBody1(incall)
libDLLEvtBody1(encryption)
libDLLEvtBody1(farEndCameraInstruct)
libDLLEvtBody1(realTimeTextInstruct)
libDLLEvtBody1(farEndMotoisedInstruct)
libDLLEvtBody1(PhoneNumberEvent)
libDLLEvtBody1(URIaddressEvent)
libDLLEvtBody1(presenceSupport)
libDLLEvtBody(duplicate)
libDLLEvtBody1(forwardCall)
libDLLEvtBody1(dhGenerate);
// IMPL: Event Names here


// Media
libDLLMediaBody(audio)
libDLLMediaBody(video)
libDLLMediaBody(content)

