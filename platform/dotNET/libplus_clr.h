/*
* libplus_clr.h
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

#pragma once

#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;
using namespace std;

#pragma unmanaged

#include <string>
#include "libplus.h"

///////////////////////////////////////////////////////////////////////////////////////////////

#define libNETCLR(name) \
typedef int(__stdcall* name##_ANSWERCB)(); \
name##_ANSWERCB name##_cb;

#define libNETCLR1(name) \
typedef int(__stdcall* name##_ANSWERCB)(const char *); \
name##_ANSWERCB name##_cb;

#define libNETCLR2(name) \
typedef int(__stdcall* name##_ANSWERCB)(const char *, const char *); \
name##_ANSWERCB name##_cb;

#define libNETCLR3(name) \
typedef int(__stdcall* name##_ANSWERCB)(const char *, const char *, const char *); \
name##_ANSWERCB name##_cb;

#define libNETCLR4(name) \
typedef int(__stdcall* name##_ANSWERCB)(const char *, const char *, const char *, const char *); \
name##_ANSWERCB name##_cb;

class CLR_PLUSdevice  : public PLUSdevice
{
    //and somewhere in a class that should be able to notify its progress declare a field of this type:
public:

    libNETCLR2(status)
    libNETCLR1(isinitialised)
    //libNETCLR1(notused1)
    libNETCLR1(callerid)
    libNETCLR1(incomingcall)
    libNETCLR1(incall)
    libNETCLR1(encryption)
    libNETCLR1(farEndCameraInstruct)
    libNETCLR1(realTimeTextInstruct)
    libNETCLR1(farEndMotoisedInstruct)
    libNETCLR1(PhoneNumberEvent)
    libNETCLR1(URIaddressEvent)
    libNETCLR1(presenceSupport)
    libNETCLR(duplicate)
    libNETCLR1(forwardCall)
    libNETCLR1(dhGenerate)
    libNETCLR2(mediastart)
    // IMPL: Event Names here

protected:

    virtual void Event(
        int evtID,                // ID from Event enum
        const char * p1 = "",     // parameter 1
        const char * p2 = "",     // parameter 2
        const char * p3 = "",     // parameter 3
        const char * p4 = ""      // parameter 4
        );

};

///////////////////////////////////////////////////////////////////////////////////////////////
// Convert System::String to std::string

void UnManaged_SetValue(CLR_PLUSdevice * device, int settingID, const char* str)
{
    device->Set_Value(settingID, str);
}

void UnManaged_Call(CLR_PLUSdevice * device, int settingID, const char* str1, const char* str2, const char* str3, const char* str4)
{
    device->Call(settingID, str1, str2, str3, str4);
}

#pragma managed

void Managed_SetValue(CLR_PLUSdevice * device, int settingID, String ^ value)
{
    marshal_context^ context = gcnew marshal_context();
    UnManaged_SetValue(device, settingID, context->marshal_as<const char*>(value));
    delete context;
}

void Managed_Call(CLR_PLUSdevice * device, int settingID, String ^ p1 = "", String ^ p2 = "", String ^ p3 = "", String ^ p4 = "")
{
    marshal_context^ context = gcnew marshal_context();
    UnManaged_Call(device, settingID, context->marshal_as<const char*>(p1), 
                    context->marshal_as<const char*>(p2), context->marshal_as<const char*>(p3), context->marshal_as<const char*>(p4));
    delete context;
} 

#define libNETSetting(name) \
property String^ name { \
    String^ get() { return  gcnew String(m_Impl->Get_Value(PLUSdevice::e_##name)); } \
    void set(String^ value) { Managed_SetValue(m_Impl, PLUSdevice::e_##name, value); } \
}

#define libNETMethod0(name) \
void name##() { Managed_Call(m_Impl, PLUSdevice::e_##name); }

#define libNETMethod1(name) \
void name##(String^ str1) { Managed_Call(m_Impl, PLUSdevice::e_##name, str1); }

#define libNETMethod2(name) \
void name##(String^ str1, String^ str2) { Managed_Call(m_Impl, PLUSdevice::e_##name, str1, str2); }

#define libNETMethod3(name) \
void name##(String^ str1, String^ str2, String^ str3) { Managed_Call(m_Impl, PLUSdevice::e_##name, str1, str2, str3); }

#define libNETMethod4(name) \
void name##(String^ str1, String^ str2, String^ str3, String^ str4) { Managed_Call(m_Impl, PLUSdevice::e_##name, str1, str2, str3, str4); }

#define libNETEventDel1(name) \
public delegate void name##_del(String^ p1);

#define libNETEventDel2(name) \
public delegate void name##_del(String^ p1, String^ p2);

#define libNETEventDel3(name) \
public delegate void name##_del(String^ p1, String^ p2, String^ p3);

#define libNETEventDel4(name) \
public delegate void name##_del(String^ p1, String^ p2, String^ p3, String^ p4);

#define libNETEvent1(name) \
public: \
event name##_del^ name; \
private: \
void raise##name(String^ p1) { name##(p1); }

#define libNETEvent2(name) \
public: \
event name##_del^ name; \
private: \
void raise##name(String^ p1, String^ p2) { name##(p1,p2); }

#define libNETEvent3(name) \
public: \
event name##_del^ name; \
private: \
void raise##name(String^ p1, String^ p2, String^ p3) { name##(p1,p2,p3); }

#define libNETEvent4(name) \
public: \
event name##_del^ name; \
private: \
void raise##name(String^ p1, String^ p2, String^ p3, String^ p4) { name##(p1,p2,p3,p4); }



#define libNETEventDel(name) \
public delegate void name##_del();

#define libNETEvent(name) \
public: \
event name##_del^ name; \
private: \
void raise##name() { name##(); }


#define libNETInitial(name) \
name##_del^ name##_prDel = gcnew name##_del(this, &libPLUS::raise##name); \
GCHandle name##_gch = GCHandle::Alloc(name##_prDel); \
IntPtr name##_ip = Marshal::GetFunctionPointerForDelegate(name##_prDel); \
m_Impl->name##_cb = static_cast<CLR_PLUSdevice::name##_ANSWERCB>(name##_ip.ToPointer());


namespace libplus {

    libNETEventDel2(status)
    libNETEventDel1(isinitialised)
    //libNETEventDel1(notused1)
    libNETEventDel1(callerid)
    libNETEventDel1(incomingcall)
    libNETEventDel1(incall)
    libNETEventDel1(encryption)
    libNETEventDel1(farEndCameraInstruct)
    libNETEventDel1(realTimeTextInstruct)
    libNETEventDel1(farEndMotoisedInstruct)
    libNETEventDel1(PhoneNumberEvent)
    libNETEventDel1(URIaddressEvent)
    libNETEventDel1(presenceSupport)
    libNETEventDel(duplicate)
    libNETEventDel1(forwardCall)
    libNETEventDel1(dhGenerate)
    libNETEventDel2(mediastart)
    // IMPL: Event Names here

	public ref class libPLUS
	{
        public:
            // Allocate the native object on the C++ Heap via a constructor
            libPLUS();

            // Deallocate the native object on a destructor
            ~libPLUS();

            void Load();

            void Unload();

            enum class MediaStream { audioIn, audioOut, videoIn, videoOut, extVideoIn, extVideoOut, localVideoOut };

            // Settings
            libNETSetting(version)
            libNETSetting(tracing)
            libNETSetting(username)
            libNETSetting(password)
            libNETSetting(server)
            libNETSetting(quality)
            libNETSetting(accessability)
            libNETSetting(content)
            libNETSetting(autoanswer)

            libNETSetting(drvvideoplay)
            libNETSetting(drvvideorec)
            libNETSetting(drvaudioplay)
            libNETSetting(drvaudiorec)
            libNETSetting(curdrvvideoplay)
            libNETSetting(curdrvvideorec)
            libNETSetting(curdrvaudioplay)
            libNETSetting(curdrvaudiorec)
            libNETSetting(devvideoplay)
            libNETSetting(devvideorec)
            libNETSetting(devaudioplay)
            libNETSetting(devaudiorec)
            libNETSetting(audioplay)
            libNETSetting(audiorec)
            libNETSetting(videoplay)
            libNETSetting(videorec)

            libNETSetting(call)
            libNETSetting(audiomute)
            libNETSetting(videomute)
            //libNETSetting(framewidth)
            //libNETSetting(frameheight)
            libNETSetting(callstate)
            libNETSetting(farEndCameraSupport)
            libNETSetting(realTimeTextSupport)
            libNETSetting(farEndMotorizedSupport)
            libNETSetting(farEndCameraInCall)
            libNETSetting(realTimeTextInCall)
            libNETSetting(farEndMotorizedInCall)
            libNETSetting(userPhoneNumber)
            libNETSetting(userURIAddress)
            libNETSetting(showlocalvideo)
            //libNETSetting(instanceid)  - Not required 
            libNETSetting(initialised)
            libNETSetting(language)
            libNETSetting(listenport)
            libNETSetting(videoformats)
            libNETSetting(videoinformat)
            libNETSetting(videooutformat)
            libNETSetting(secondvideo)

            libNETSetting(encryptSignal)
            libNETSetting(encryptMedia)
            libNETSetting(encryptMediaHigh)
            // IMPL: Setting Names Here


            // Methods
            libNETMethod1(placeCall)
            libNETMethod0(hangupCall)
            libNETMethod0(answerCall)
            libNETMethod1(sendDTMF)
            libNETMethod3(farEndCamera)
            libNETMethod0(start)
            libNETMethod3(farEndMotorized)
            libNETMethod1(realTimeTextInput)
            libNETMethod0(realTimeTextNewLine)
            libNETMethod1(secondCall)
            libNETMethod0(stop)
            libNETMethod0(dhParameters)
            libNETMethod3(videosize)
            libNETMethod4(audioformat)
            // IMPL: Method Names here

            // Events
            libNETEvent2(status)
            libNETEvent1(isinitialised)
            //libNETEvent1(notused1)
            libNETEvent1(callerid)
            libNETEvent1(incomingcall)
            libNETEvent1(incall)
            libNETEvent1(encryption)
            libNETEvent1(farEndCameraInstruct)
            libNETEvent1(realTimeTextInstruct)
            libNETEvent1(farEndMotoisedInstruct)
            libNETEvent1(PhoneNumberEvent)
            libNETEvent1(URIaddressEvent)
            libNETEvent1(presenceSupport)
            libNETEvent(duplicate)
            libNETEvent1(forwardCall)
            libNETEvent1(dhGenerate)
            libNETEvent2(mediastart)
            // IMPL: Event Names here

        protected:
            // Deallocate the native object on the finalizer just in case no destructor is called
            !libPLUS();

        private:
            void initialize()
            {
                libNETInitial(status)
                libNETInitial(isinitialised)
                //libNETInitial(notused1)
                libNETInitial(callerid)
                libNETInitial(incomingcall)
                libNETInitial(incall)
                libNETInitial(encryption)
                libNETInitial(farEndCameraInstruct)
                libNETInitial(realTimeTextInstruct)
                libNETInitial(farEndMotoisedInstruct)
                libNETInitial(PhoneNumberEvent)
                libNETInitial(URIaddressEvent)
                libNETInitial(presenceSupport)
                libNETInitial(duplicate)
                libNETInitial(forwardCall)
                libNETInitial(dhGenerate)
                libNETInitial(mediastart)
                // IMPL: Event Names here
            }

            CLR_PLUSdevice * m_Impl;

	};
}
