
/*
 * libPLUSdll.h
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


#ifdef LIBPLUSDLL_EXPORTS
#define LIBPLUSDLL_API __declspec(dllexport)
#else
#define LIBPLUSDLL_API __declspec(dllimport)
#endif

#define libDLLSetting(name) \
  const char * get##name(); \
  void set##name(const char * value);

#define libDLLMethod0(name) \
void do##name();

#define libDLLMethod1(name) \
void do##name(const char * str1);

#define libDLLMethod2(name) \
void do##name(const char * str1, const char * str2);

#define libDLLMethod3(name) \
void do##name(const char * str1, const char * str2, const char * str3);


// This class is exported from the libPLUS.dll
class PLUSdevice;
class LIBPLUSDLL_API libPLUS {
public:
	libPLUS(void);

    virtual ~libPLUS();

    void Load();
    void Unload();

    // Settings
    libDLLSetting(version)
    libDLLSetting(tracing)
    libDLLSetting(username)
    libDLLSetting(password)
    libDLLSetting(server)
    libDLLSetting(quality)
    libDLLSetting(accessability)
    libDLLSetting(content)
    libDLLSetting(autoanswer)

    libDLLSetting(drvvideoplay)
    libDLLSetting(drvvideorec)
    libDLLSetting(drvaudioplay)
    libDLLSetting(drvaudiorec)
    libDLLSetting(curdrvvideoplay)
    libDLLSetting(curdrvvideorec)
    libDLLSetting(curdrvaudioplay)
    libDLLSetting(curdrvaudiorec)
    libDLLSetting(devvideoplay)
    libDLLSetting(devvideorec)
    libDLLSetting(devaudioplay)
    libDLLSetting(devaudiorec)
    libDLLSetting(audioplay)
    libDLLSetting(audiorec)
    libDLLSetting(videoplay)
    libDLLSetting(videorec)

    libDLLSetting(call)
    libDLLSetting(audiomute)
    libDLLSetting(videomute)
    //libDLLSetting(framewidth)
    //libDLLSetting(frameheight)
    libDLLSetting(callstate)
    libDLLSetting(farEndCameraSupport)
    libDLLSetting(realTimeTextSupport)
    libDLLSetting(farEndMotorizedSupport)
    libDLLSetting(farEndCameraInCall)
    libDLLSetting(realTimeTextInCall)
    libDLLSetting(farEndMotorizedInCall)
    libDLLSetting(userPhoneNumber)
    libDLLSetting(userURIAddress)
    libDLLSetting(showlocalvideo)
    //libDLLSetting(instanceid)  - Not required 
    libDLLSetting(initialised)
    libDLLSetting(language)
    libDLLSetting(listenport)
    libDLLSetting(encryptSignal)
    libDLLSetting(encryptMedia)
    libDLLSetting(encryptMediaHigh)
     // IMPL: Setting Name here

     // Methods
    libDLLMethod1(placeCall)
    libDLLMethod0(hangupCall)
    libDLLMethod0(answerCall)
    libDLLMethod1(sendDTMF)
    libDLLMethod3(farEndCamera)
    libDLLMethod0(start)
    libDLLMethod3(farEndMotorized)
    libDLLMethod1(realTimeTextInput)
    libDLLMethod0(realTimeTextNewLine)
    libDLLMethod1(secondCall)
    libDLLMethod0(stop)
    libDLLMethod0(dhParameters)
    // IMPL: Method Name here

private:
    PLUSdevice * m_Impl;
};


