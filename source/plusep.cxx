/*
* plusep.cxx
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

#include "precompile.h"

#include "pluslang.h"

#include <ptclib/pdns.h>
#include <h235/h235support.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Media Buffer Handling

/* Serialise
Convert an array of strings into a stream seperated by ;
@return@
A ; deliminated string
*/
PString Serialise(const PStringArray & strarray) {

    if (strarray.GetSize() == 0)
        return PString("");
    if (strarray.GetSize() == 1)
        return strarray[0].Trim();

    PString ser = PString();
    for (int i = 0; i < strarray.GetSize() - 1; ++i) {
        ser += strarray[i].Trim() + ";";
    }
    ser += strarray[strarray.GetSize() - 1].Trim();
    return ser;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct {
    PlusMediaManager::MediaStream id;
    unsigned v1;
    unsigned v2;
    PString  v3;
    unsigned v4;
    unsigned v5;
} defaultMedia[PlusMediaManager::e_NoOfMediaStream] = {
    { PlusMediaManager::e_audioIn,       8000,      2,              "", 1, 20 },
    { PlusMediaManager::e_audioOut,      8000,      2,              "", 1, 20 },
    { PlusMediaManager::e_videoIn,        352,    288,  defVideoFormat, 0,  0 },
    { PlusMediaManager::e_videoOut,         0,      0,  defVideoFormat, 0,  0 },
    { PlusMediaManager::e_contentIn,      352,    288,  defVideoFormat, 0,  0 },
    { PlusMediaManager::e_contentOut,       0,      0,  defVideoFormat, 0,  0 },
    { PlusMediaManager::e_localVideoOut,    0,      0,  defVideoFormat, 0,  0 }
};


PlusMediaManager::PlusMediaManager()
{
    // Initialise the manager
    for (unsigned i = 0; i < e_NoOfMediaStream; ++i) {
        m_queueMedia.insert(make_pair(defaultMedia[i].id, Queue(defaultMedia[i].v1, defaultMedia[i].v2, 
                                                                defaultMedia[i].v3, defaultMedia[i].v4, defaultMedia[i].v5)));
    }
}


PlusMediaManager::Sample::Sample(void * data, unsigned size, unsigned width, unsigned height)
: PBYTEArray((BYTE*)data,size), m_width(width), m_height(height)
{

}


PlusMediaManager::Queue::Queue()
: m_v1(0), m_v2(0), m_v3(""), m_shutdown(false)
{

}


PlusMediaManager::Queue::Queue(unsigned v1, unsigned v2, const PString & v3, unsigned v4, unsigned v5)
: m_v1(v1), m_v2(v2), m_v3(v3), m_v4(v4), m_v5(v5), m_isRunning(false), m_shutdown(false)
{

}


PStringArray PlusMediaManager::SupportedFormats(unsigned dir)
{
    if (dir >= e_NoOfMediaStream)
        return "";

    
    switch (dir) {
        case e_audioIn:
        case e_audioOut:
            return "PCM";

        case e_videoIn:
        case e_contentIn:
            return Serialise(H323ColourConverter::GetColourConverterList(m_queueMedia[e_videoIn].m_v3, false));

        case e_videoOut:
        case e_contentOut:
            return Serialise(H323ColourConverter::GetColourConverterList(m_queueMedia[e_videoOut].m_v3, true));
        default:
            return "";
    }
}

void PlusMediaManager::SetAudioFormat(unsigned sampleRate, unsigned bytesPerSample, unsigned noChannels, unsigned sampleTime)
{
    SetAudioFormat(PlusMediaManager::e_audioIn,  sampleRate, bytesPerSample, noChannels, sampleTime);
    SetAudioFormat(PlusMediaManager::e_audioOut, sampleRate, bytesPerSample, noChannels, sampleTime);
}

PBoolean PlusMediaManager::SetAudioFormat(unsigned id, unsigned sampleRate, unsigned bytesPerSample, unsigned noChannels, unsigned sampleTime)
{
    if (id > e_audioOut)
        return false;

    Queue & q = m_queueMedia[id];
        q.m_v1 = sampleRate;
        q.m_v2 = bytesPerSample;
        q.m_v4 = noChannels;
        q.m_v5 = sampleTime;

    return true;
}

void PlusMediaManager::GetAudioFormat(unsigned id, unsigned & sampleRate, unsigned & bytesPerSample, unsigned & noChannels, unsigned & sampleTime)
{
    if (id > e_audioOut)
        return;

    Queue & q = m_queueMedia[id];
        sampleRate    = q.m_v1;
        bytesPerSample= q.m_v2;
        noChannels    = q.m_v4;
        sampleTime    = q.m_v5;
}


void PlusMediaManager::GetColourFormat(unsigned id, PString & colourFormat)
{
    if (id >= e_NoOfMediaStream)
        return;

    colourFormat = m_queueMedia[id].m_v3;
}



PBoolean PlusMediaManager::SetColourFormat(unsigned id, const PString & colourFormat)
{
    if (id >= e_NoOfMediaStream || colourFormat.IsEmpty())
        return false;

    Queue & q = m_queueMedia[id];
        q.m_mutex.Wait();
            q.m_v3 = colourFormat;
        q.m_mutex.Signal();

    return true;
}


void PlusMediaManager::SetVideoFormat(H323Channel::Directions dir, const PString & colourFormat)
{
    if (dir == H323Channel::IsTransmitter) {
        SetColourFormat(e_videoOut, colourFormat);
        SetColourFormat(e_contentOut, colourFormat);
        SetColourFormat(e_localVideoOut, colourFormat);
    } else {
        SetColourFormat(e_videoIn, colourFormat);
        SetColourFormat(e_contentIn, colourFormat);
    }
}

PBoolean PlusMediaManager::Start(unsigned id)
{
    if (id >= e_NoOfMediaStream)
        return false;

    m_queueMedia[id].m_isRunning = true;
    return true;
}

PBoolean PlusMediaManager::Stop(unsigned id)
{
    if (id >= e_NoOfMediaStream)
        return false;

    Queue & q = m_queueMedia[id];

    q.m_isRunning = false;

    q.m_mutex.Wait();
        while (!q.size())
            q.pop();
    q.m_mutex.Signal();
    return true;
}

PBoolean PlusMediaManager::IsRunning(unsigned id)
{
    if (id >= e_NoOfMediaStream)
        return false;

    return m_queueMedia[id].m_isRunning;
}


PBoolean PlusMediaManager::GetFrameSize(unsigned id, unsigned & width, unsigned & height)
{
    if (id >= e_NoOfMediaStream)
        return false;

    Queue & q = m_queueMedia[id];
    width = q.m_v1;
    height = q.m_v2;
    return (q.m_v1 > 0 && q.m_v2 > 0);
}


PBoolean PlusMediaManager::SetFrameSize(unsigned id, unsigned width, unsigned height)
{
    if (id >= e_NoOfMediaStream)
        return false;

    Queue & q = m_queueMedia[id];
    q.m_v1 = width;
    q.m_v2 = height;
    return true;
}


bool PlusMediaManager::ProcessMediaSamples(unsigned & id, void * data, unsigned & size, unsigned & width, unsigned & height)
{
    // Initialise the manager
    for (unsigned i = 1; i < e_NoOfMediaStream; i += 2) {
        if (Read(i, false, data, size, width, height)) {
            id = i;
            return true;
        }
    }
    return false;
}

bool PlusMediaManager::Write(unsigned id, void * data, unsigned size, unsigned width, unsigned height)
{
    if (id >= e_NoOfMediaStream)
        return false;

    Queue & q = m_queueMedia[id];

    if (!q.m_isRunning)
        return true;

    q.m_mutex.Wait();
        q.push(Sample(data, size, width, height));
    q.m_mutex.Signal();

    return true;
}

bool PlusMediaManager::Read(unsigned id, bool toBlock, void * data, unsigned size)
{
    if (id > e_audioOut)
        return false;

    Queue & q = m_queueMedia[id];

    if (!toBlock) {
        if (!q.size()) return false;
    }
    else {
        while (!q.m_shutdown && !q.size())
            PThread::Sleep(3);
    }

    bool success = false;
    q.m_mutex.Wait();
    Sample & media = q.front();

    if (size > (unsigned)media.GetSize()) {
        PTRACE(2, "PLUSAUD\tMedia size is " << media.GetSize() << " expecting " << size);
    } else {
        memcpy(data, media.GetPointer(), size);
        success = true;
    }
    q.pop();
    q.m_mutex.Signal();

    return success;
}

bool PlusMediaManager::Read(unsigned id, bool toBlock, void * data, unsigned & size, unsigned & width, unsigned & height)
{
    if (id >= e_NoOfMediaStream)
        return false;

    Queue & q = m_queueMedia[id];

    if (!toBlock) {
        if (!q.size()) return false;
    } else {
        while (!q.m_shutdown && !q.size())
            PThread::Sleep(3);
    }  

    q.m_mutex.Wait();
        Sample & media = q.front();
        width = media.m_width;
        height = media.m_height;
        size = media.GetSize();
        memcpy(data, media.GetPointer(), size);
        q.pop();
    q.m_mutex.Signal();

    return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// endpoint Utilities

/* GetSoundDevice
get a usuable sound device for a given direction
@return@
name of a video device
*/
PString GetSoundDevice(PBoolean isEncoding, const PString & driver)
{
    PStringList devices = isEncoding ? PSoundChannel::GetDeviceNames(driver, PSoundChannel::Recorder)
                                     : PSoundChannel::GetDeviceNames(driver, PSoundChannel::Player);

    if (devices.GetSize() > 0) {
        for (PINDEX i = 0; i < devices.GetSize(); i++) {
            if (devices[i] != "Default")
                return devices[i];
        }
    } 
    return PString();
}

/* GetVideoDevice
    get a usuable video device for a given direction
    @return@
    name of a video device
*/
PString GetVideoDevice(PBoolean isEncoding, const PString & driver) {
    PStringList devices = isEncoding ? PVideoInputDevice::GetDriversDeviceNames(driver)
                                    : PVideoOutputDevice::GetDriversDeviceNames(driver);
    // Look for a useful device
    PString deviceName;
    for (PINDEX i = 0; i < devices.GetSize(); i++) {
        PCaselessString devName = devices[i];
        if (devName != "*.yuv" && devName != "fake" && devName != "NULL") {
            deviceName = devName;
            break;
        }
    }
    if (deviceName.IsEmpty())
        deviceName = isEncoding ? "none" : "NULL";

    return deviceName;
}


/* EnumerateSoundDevices
    Enumerate all the detected sound devices
    The defAudioDriver is defined in plussettings.h
*/
void EnumerateSoundDevices(PBoolean isEncoding, const PString & driver, PStringArray & devices)
{
    if (isEncoding)
        devices = PSoundChannel::GetDeviceNames(driver, PSoundChannel::Recorder);
    else 
        devices = PSoundChannel::GetDeviceNames(driver, PSoundChannel::Player);
}

/* EnumerateVideoDevices
    Enumerate all the detected video devices
    The defAudioDriver is defined in plussettings.h
*/
void EnumerateVideoDevices(PBoolean isEncoding, const PString & driver, PStringArray & devices)
{
    if (isEncoding)
        devices = PVideoInputDevice::GetDriversDeviceNames(driver);
    else
        devices = PVideoOutputDevice::GetDriversDeviceNames(driver);
}

/* GetDefaultDevice
Get the default devices for the supplied settiing using the supplied driver
@return@
a ; deliminated string of devices
*/
PString GetDefaultDevice(PlusProcess::Setting setting, const PString & driver)
{
    PStringArray devices;
    switch (setting) {
        case PlusProcess::e_videoplay:     return GetVideoDevice(false, driver);
        case PlusProcess::e_videorec:      return GetVideoDevice(true, driver); 
        case PlusProcess::e_audioplay:     return GetSoundDevice(false, driver); 
        case PlusProcess::e_audiorec:      return GetSoundDevice(true, driver); 
        default: return PString();
    }
}

/* GetDeviceList
    Get a list of all the devices matching the supplied setting
    @return@
    a ; deliminated string of devices
*/
PString GetDeviceList(PlusProcess::Setting setting, const PString & driver)
{
    PStringArray devices;
    switch (setting) {
        case PlusProcess::e_devvideoplay:     EnumerateVideoDevices(false, driver, devices); break;
        case PlusProcess::e_devvideorec:      EnumerateVideoDevices(true, driver, devices); break;
        case PlusProcess::e_devaudioplay:     EnumerateSoundDevices(false, driver, devices); break;
        case PlusProcess::e_devaudiorec:      EnumerateSoundDevices(true, driver, devices); break;
        default: break;
    }
    return Serialise(devices);
}

PString GetDriverList(PlusProcess::Setting setting)
{
    PStringArray drivers;
    switch (setting) {
        case PlusProcess::e_drvvideoplay:   drivers = PVideoOutputDevice::GetDriverNames();  break;
        case PlusProcess::e_drvvideorec:    drivers = PVideoInputDevice::GetDriverNames(); break;
        case PlusProcess::e_drvaudioplay:   drivers = PSoundChannel::GetDriverNames(); break;
        case PlusProcess::e_drvaudiorec:    drivers = PSoundChannel::GetDriverNames(); break;
        default: break;
    }
    return Serialise(drivers);
}

//////////////////////////////////////////////////////////////////////////

#define DRIVER_CHG(name) \
    m_dev##name = GetDeviceList(PlusProcess::e_dev##name, m_curdrv##name); \
    m_##name = GetDefaultDevice(PlusProcess::e_##name, m_curdrv##name);

#define DEVICE_DECL(name) \
    m_drv##name = GetDriverList(PlusProcess::e_drv##name); \
    m_curdrv##name = defdrv##name; \
    DRIVER_CHG(name)

#define INIT_SET(name,value) \
    ini_##name(value);

#define INIT_RESET(name) \
    INIT_SET(name,m_##name) 

#define INIT(reset, name, value) \
    if (reset) INIT_RESET(name) \
    else INIT_SET(name,value)

/////////////////////////////////////////////////////////////////////////

#ifdef H323_DATASTORE
PlusEndPoint::PlusEndPoint(PlusProcess & process, H323DataStore & _data)
#else
PlusEndPoint::PlusEndPoint(PlusProcess & process)
#endif
:  m_process(process), m_currentCallToken(PString()), m_activeCall(false), m_stunType(0), 
   m_endpointIsSetup(false), m_libPath(PProcess::Current().GetFile().GetDirectory())
#ifdef H323_DATASTORE
   ,m_dataStore(_data)
#endif
{

    InitialiseSettings(false);

    DEVICE_DECL(videoplay)
    DEVICE_DECL(videorec)
    DEVICE_DECL(audioplay)
    DEVICE_DECL(audiorec)

    m_videoformats = Serialise(m_mediaManager.SupportedFormats(PlusMediaManager::e_videoOut));
    
    m_framewidth = PString(defDisplayWidth);
    m_frameheight = PString(defDisplayHeight);
}

PlusEndPoint::~PlusEndPoint()
{
#ifdef H323_H235_AES256
    EncryptionCacheRemove();
#endif
}

void PlusEndPoint::InitialiseSettings(bool reset)
{

#ifdef H323_DATASTORE
    PString user = m_dataStore.GetDefaultID();
    PTRACE(6, "EP\tInitialise settings for " << user << " user.");
#else
    PString user = GetLocalUserName();
#endif

    INIT(reset, version, LIBPLUS_VERSION)
    INIT(reset, tracing, PString(defTraceLevel))
    INIT(reset, username, user)
    INIT(reset, password, "") 
    INIT(reset, server, "")
    INIT(reset, quality, "1") 
    INIT(reset, accessability, "0") 
    INIT(reset, content, "0")
    INIT(reset, autoanswer, "0") 
    // drvvideoplay
    // drvvideorec
    // drvaudioplay
    // drvaudiorec
    // curdrvvideoplay
    // curdrvvideorec
    // curdrvaudioplay
    // curdrvaudiorec
    // devvideoplay
    // devvideorec
    // devaudioplay
    // devaudiorec
    // audiorec
    // audioplay
    // videoplay
    // videorec
    INIT(reset, call, "") 
    INIT(reset, audiomute,"0") 
    INIT(reset, videomute,"0")
    INIT(reset, callstate,"0") 
    INIT(reset, h281support,"1") 
    INIT(reset, t140support, "0") 
    INIT(reset, h284support, "0") 
    INIT(reset, usere164, "")
    INIT(reset, useruri, "") 
    INIT(reset, showlocalvideo, "1") 
    INIT(reset, h281call, "0") 
    INIT(reset, t140call, "0") 
    INIT(reset, h284call, "0") 
    INIT(reset, initialised, "0") 
    INIT(reset, language, defLanguage)
    INIT(reset, listenport, defListenport)
    // videoformats
    INIT(reset, videoinformat, defVideoFormat)
    INIT(reset, videooutformat, defVideoFormat)
    INIT(reset, secondvideo, "0")

    INIT(reset, encryptsignal, "1") 
    INIT(reset, encryptmedia, "1")
    INIT(reset, encryptmediahigh, "0")

    // Internal Use Only
    INIT(reset, dhOID, "")
    INIT(reset, dhPrime, "")
    INIT(reset, dhGenerator, "")

    // IMPL: Setting Names here!

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Overrides from h323plus

H323Connection * PlusEndPoint::CreateConnection(unsigned callReference, void * userData)
{
  return new PlusConnection(*this, callReference);
}

void PlusEndPoint::OnConnectionEstablished(H323Connection & connection,
                                                 const PString & token)
{
    m_currentCallToken = token;
    FireStatus(uiCallInProgress);
    PTRACE(2, "EP\tIn call with " << connection.GetRemotePartyName());

    m_activeCall = true;
}


void PlusEndPoint::OnConnectionCleared(H323Connection & connection,
                                             const PString & clearedCallToken)
{
  if (m_currentCallToken == clearedCallToken)
    m_currentCallToken = PString();

  fire_callerid("");
  m_callstate = "0";
  FireStatus(uiWaiting);
}

void PlusEndPoint::OnCallClearing(H323Connection * connection,
									H323Connection::CallEndReason reason)
{

    PTRACE(4,"EP\tEnd call " << connection->GetCallToken());

    if (connection == NULL)
	    return;

    PlusSetValue(h281call, 0);
    PlusSetValue(t140call, 0);
    PlusSetValue(h284call, 0);
    fire_encryption("0");
    fire_incall("0");

    m_activeCall = false;

    // Fire to stop sending media...
    // Needs to be done here as the media channels may take a few seconds
    // to actually shutdown but we want to end the media ASAP.
    if (m_curdrvaudiorec == "External")
        fire_mediastart(PString(PlusMediaManager::e_audioIn), "0");

    if (m_curdrvaudioplay == "External")
        fire_mediastart(PString(PlusMediaManager::e_audioOut), "0");

    if (m_curdrvvideorec == "External")
        fire_mediastart(PString(PlusMediaManager::e_videoIn), "0");

    if (m_curdrvvideoplay == "External")
        fire_mediastart(PString(PlusMediaManager::e_videoOut), "0");
      
    if (reason < H323Connection::NumCallEndReasons)
        FireStatus(reason + 6);
}

void PlusEndPoint::OnClosedLogicalChannel(H323Connection & /*connection*/, const H323Channel & channel)
{
    unsigned id = channel.GetSessionID();
    H323Channel::Directions dir = channel.GetDirection();
    bool isEncoding = (dir == H323Channel::IsTransmitter);

    PTRACE(2, "EP\tClosing channel " << id << " " << (isEncoding ? "encoder " : "decoder "));

    if (!m_activeCall)
        return;

    if (id == RTP_Session::DefaultAudioSessionID) {
        if (isEncoding && m_curdrvaudiorec == "External")
            fire_mediastart(PString(PlusMediaManager::e_audioIn), "0");
        else if (m_curdrvaudioplay == "External")
            fire_mediastart(PString(PlusMediaManager::e_audioOut), "0");
    }

    if (id == RTP_Session::DefaultVideoSessionID) {
        if (isEncoding && m_curdrvvideorec == "External")
            fire_mediastart(PString(PlusMediaManager::e_videoIn), "0");
        else if (m_curdrvvideoplay == "External")
            fire_mediastart(PString(PlusMediaManager::e_videoOut), "0");
    }


}

PBoolean PlusEndPoint::OnIncomingCall(H323Connection & connection,
                                    const H323SignalPDU & setupPDU,
                                    H323SignalPDU &)
{  	
    return true;
}


H323Connection::AnswerCallResponse
                   PlusEndPoint::OnAnswerCall(H323Connection & connection,
                                                    const PString & caller,
                                                    const H323SignalPDU & setupPDU,
                                                    H323SignalPDU &)
{
  m_currentCallToken = connection.GetCallToken();

  PString DisplayName =setupPDU.GetQ931().GetDisplayName();
  if (m_autoanswer.AsInteger() > 0) {
    m_callstate = "1";
    fire_incall(m_callstate);
    FireStatus(uiCallInProgress);
    fire_callerid(DisplayName);
    return H323Connection::AnswerCallNow;
  }
  FireStatus(uiIncomingCall);
  fire_incomingcall(DisplayName);

  const H225_Setup_UUIE & setup = setupPDU.m_h323_uu_pdu.m_h323_message_body;
  if (setup.HasOptionalField(H225_Setup_UUIE::e_destinationAddress) && (setup.m_destinationAddress.GetSize() > 0)) {
      PString calledNumber = H323GetAliasAddressString(setup.m_destinationAddress[0]);
      if (calledNumber != m_username && calledNumber != m_usere164 && calledNumber != m_useruri) {
          PTRACE(4,"EP\tForward Call Number " << calledNumber);
          fire_forwardcall(calledNumber);
      }
  }

  return H323Connection::AnswerCallPending;
}

PBoolean PlusEndPoint::STUNNatType(int type)
{
    m_stunType = type;
    return true;
}

void PlusEndPoint::OnMediaEncryption(unsigned session, H323Channel::Directions dir, const PString & cipher) 
{
    if (session == 1 && dir == H323Channel::IsTransmitter)
        fire_encryption("1");
}

PBoolean PlusEndPoint::OpenAudioChannel(H323Connection & connection,
                                          PBoolean isEncoding,
                                          unsigned bufferSize,
                                          H323AudioCodec & codec)
{

  PString deviceDriver = isEncoding ? m_curdrvaudiorec : m_curdrvaudioplay;
  PString deviceName = isEncoding ? m_audiorec : m_audioplay;

  bool isExternal = (deviceDriver == "External");

  PTRACE(2,"AUDIO\tOpening audio channel " << (isEncoding ? "encoder " : "decoder ")
       << deviceName);

  codec.SetSilenceDetectionMode(H323AudioCodec::NoSilenceDetection);

  if (isEncoding) {
      SetSoundChannelRecordDriver(deviceDriver);
      SetSoundChannelRecordDevice(deviceName);
      if (isExternal)
          fire_mediastart(PString(PlusMediaManager::e_audioIn), "1");
  }
  else {
      SetSoundChannelPlayDriver(deviceDriver);
      SetSoundChannelPlayDevice(deviceName);
      if (isExternal)
          fire_mediastart(PString(PlusMediaManager::e_audioOut), "1");
  }

  return H323EndPoint::OpenAudioChannel(connection, isEncoding, bufferSize, codec);

}


PBoolean PlusEndPoint::OpenVideoChannel(H323Connection & connection, PBoolean isEncoding, H323VideoCodec & codec)
{

    PString deviceDriver = isEncoding ? m_curdrvvideorec : m_curdrvvideoplay;
    PString deviceName   = isEncoding ? m_videorec : m_videoplay;

    bool isExternal = (deviceDriver == "External");

    PTRACE(2, "VID\tOpening video channel " << (isEncoding ? "encoder" : "decoder") 
                    << " using driver: " << deviceDriver << " device: " << deviceName);

#if PTLIB_VER >= 2120
    PVideoDevice * device = isEncoding ? (PVideoDevice *)PVideoInputDevice::CreateOpenedDevice(deviceDriver, deviceName, FALSE)
                                       : (PVideoDevice *)PVideoOutputDevice::CreateOpenedDevice(deviceDriver, deviceName, FALSE);
#else
    PVideoDevice * device = isEncoding ? (PVideoDevice *)PVideoInputDevice::CreateDeviceByName(deviceName, deviceDriver)
                                       : (PVideoDevice *)PVideoOutputDevice::CreateDeviceByName(deviceName, deviceDriver);
#endif

    if (!device) {
        PTRACE(1, "Failed to instance video device \"" << deviceName << '"');
        return false;
    }

    if (isExternal) {
        if (isEncoding && PIsDescendant(device, PVideoInputDevice_External))
            ((PVideoInputDevice_External*)device)->AttachManager(PlusMediaManager::e_videoIn, &m_mediaManager);
        else if (PIsDescendant(device, PVideoOutputDevice_External))
            ((PVideoOutputDevice_External*)device)->AttachManager(PlusMediaManager::e_videoOut, &m_mediaManager);
    }

    if (isEncoding) {
        PVideoInputDevice::Capabilities videoCaps;
#if PTLIB_VER >= 2110
        if (((PVideoInputDevice *)device)->GetDeviceCapabilities(deviceDriver, deviceName, &videoCaps))
            codec.SetSupportedFormats(videoCaps.framesizes);
#endif
        // set fixed list of resolutions for drivers that don't provide a list
        if (videoCaps.framesizes.size() == 0) {
            PVideoFrameInfo cap;
            cap.SetColourFormat(defVideoFormat);
            cap.SetFrameRate(30);
            // sizes must be from largest to smallest
            cap.SetFrameSize(1280, 720);
            videoCaps.framesizes.push_back(cap);
            cap.SetFrameSize(704, 576);
            videoCaps.framesizes.push_back(cap);
            cap.SetFrameSize(352, 288);
            videoCaps.framesizes.push_back(cap);
            codec.SetSupportedFormats(videoCaps.framesizes);
        }
    }

    if (!device->SetFrameSize(codec.GetWidth(), codec.GetHeight()) ||
        !device->SetFrameRate(codec.GetFrameRate()) ||
        !device->SetColourFormatConverter(defVideoFormat)) {
        PTRACE(1, "Failed to configure the video device \"" << deviceName << '"');
        return FALSE;
    }

#if PTLIB_VER >= 2120
    device->Start();
#else
    if (!device->Open(deviceName, TRUE)) {
        PTRACE(1, "Failed to open the video device \"" << deviceName << '"');
        return FALSE;
    }
#endif

    PVideoChannel * channel = new PVideoChannel;
    if (isEncoding)
        channel->AttachVideoReader((PVideoInputDevice *)device);
    else
        channel->AttachVideoPlayer((PVideoOutputDevice *)device);

    if (isExternal) {
        if (isEncoding)
            fire_mediastart(PString(PlusMediaManager::e_videoIn), "1");
        else
            fire_mediastart(PString(PlusMediaManager::e_videoOut), "1");
    }

    return codec.AttachChannel(channel, TRUE);
}


void AnalyizeFrameSizes(PlusEndPoint & ep, const PString & codec, PVideoInputDevice::Capabilities & caps, PBoolean isDefault)
{
    std::list<PVideoFrameInfo>::iterator r = caps.framesizes.begin();
    while (r != caps.framesizes.end()) {
        double w = r->GetFrameWidth();
        double h = r->GetFrameHeight();
        unsigned R = r->GetFrameRate();

        unsigned minR = (ep.get_accessability().AsInteger() ? 20 : 13);

        if (isDefault && R < minR) {
            PTRACE(4, "VID\tRemoving low framerate size " << (unsigned)w << "x" << (unsigned)h);
            caps.framesizes.erase(r++);
        }
        else {
            // if not HD remove 16:9 aspect sizes
            if ((((w * h) / 256.0) < 1792) && ((w / h) > 1.5)) {
                PTRACE(4, "VID\tRemoving HD size " << (unsigned)w << "x" << (unsigned)h);
                caps.framesizes.erase(r++);
            }
            else if (codec.Find("H.264-720") != P_MAX_INDEX && ((w / h) < 1.23)) {
                PTRACE(4, "VID\tRemoving problem size " << (unsigned)w << "x" << (unsigned)h);
                caps.framesizes.erase(r++);    // don't understand parameter 10!            
            }
            else if (!isDefault && (w > ep.get_framewidth().AsInteger() || h > ep.get_frameheight().AsInteger())) {
                PTRACE(4, "VID\tRemoving not supported sz " << (unsigned)w << "x" << (unsigned)h);
                caps.framesizes.erase(r++);
            }
            else {
                ++r;
            }
        }
    }
}

PBoolean PlusEndPoint::OnGatekeeperAliases(const H225_ArrayOf_AliasAddress & aliases)
{
    PString aliasE164=PString();
    PString aliasURI=PString();
    for (PINDEX i=0; i < aliases.GetSize(); ++i) {
        switch (aliases[i].GetTag()) {
            case H225_AliasAddress::e_dialedDigits:
                if (aliasE164.IsEmpty())
                    aliasE164 = H323GetAliasAddressString(aliases[i]);
                break;
	        case H225_AliasAddress::e_url_ID:
                if (aliasURI.IsEmpty())
                    aliasURI = H323GetAliasAddressString(aliases[i]);
                break;
            default:
                break;
        }
    }
    PTRACE(2, "EP\tSetting Aliases to : " << aliasE164 << " " << aliasURI);
    PlusSetValue(usere164,aliasE164);
    PlusSetValue(useruri,aliasURI);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PlusEndPoint::SetVendorIdentifierInfo(H225_VendorIdentifier & info) const
{
    SetH221NonStandardInfo(info.m_vendor);

    info.IncludeOptionalField(H225_VendorIdentifier::e_productId);
    info.m_productId = PString(LIBPLUS_MAKER) + " " + PString(LIBPLUS_PRODUCT) + " " + PString(LIBPLUS_OS);
    info.m_productId.SetSize(info.m_productId.GetSize() + 2);

    PString version = LIBPLUS_VERSION;
    info.IncludeOptionalField(H225_VendorIdentifier::e_versionId);
    info.m_versionId = version;
    info.m_versionId.SetSize(info.m_versionId.GetSize() + 2);

}

void PlusEndPoint::Initialise()
{
    PBoolean success = true;
    if (!InitialiseEndpoint())
        success = false;
    else if (!m_server.IsEmpty())
        success = RegisterGatekeeper();
//    else
//       success = InitialiseGateway();

    if (success) {
        PlusSetValue(initialised, 1);
        if (!m_call) {
            PlaceCall(m_call);
            m_call = PString();
        }
        else {
            FireStatus(uiWaiting);
        }
    }
    else {
        PlusSetValue(initialised, 0);
    }
}

void PlusEndPoint::UnInitialise()
{
    // Logout
    RemoveGatekeeper();
    PlusSetValue(initialised, 2);
}

PBoolean PlusEndPoint::InitialiseEndpoint()
{
    if (m_endpointIsSetup) {
        PTRACE(2, "EP\tEndPoint already Setup.");
        return true;
    }

    PTRACE(2, "EP\tInitialising EndPoint");
    PDirectory m_libPath;
    PPluginManager & pluginMgr = PPluginManager::GetPluginManager();

    m_configFile = m_libPath + pathExcept;
    m_configList = GetConfigurationSections();

    if (m_tracing.AsInteger() > 0)
        InitialiseDebug();

    // Load the base featureSet
    LoadBaseFeatureSet();

    if (!m_username) SetLocalUserName(m_username);

    DisableFastStart(true);
    DisableH245Tunneling(false);
    DisableH245QoS(true);

    SetTCPPorts(tPortBase, tPortMax);
    SetUDPPorts(uPortBase, uPortMax);
    SetRtpIpPorts(lPortBase, lPortMax);
    SetMultiplexPort(mPortBase);

    SetAudioJitterDelay(minAudioJitterDelay + defAudioDelay, maxAudioJitterDelay + defAudioDelay);

#ifdef H323_H460P
    PresenceAddFeature(e_preAudio);
    PresenceAddFeature(e_preVideo);

    PresenceAddFeatureH460();
    //  PresenceSetLocalState(localAliasNames,e_preOnline, "OnLine");
#endif

    AddAllCapabilities(0, P_MAX_INDEX, "*");
    AddAllUserInputCapabilities(0, P_MAX_INDEX);
    SetSendUserInputMode(H323Connection::SendUserInputAsString);

#ifdef H323_H224
    capabilities.SetCapability(0, P_MAX_INDEX, new H323_H224Capability());
#endif

    capabilities.Remove("G.722.1-24");

    if (!m_secondvideo.AsInteger()) {
        capabilities.Remove("h.239*");
        RemoveCapability(H323Capability::e_ExtendVideo);
        RemoveCapability(H323Capability::e_GenericControl);
        RemoveCapability(H323Capability::e_ConferenceControl);
    }

    PStringArray order;
    order.AppendString("G.722.1-32");
    order.AppendString("G.722.1-24");
    order.AppendString("G.722-");
    order.AppendString("G.711");
    order.AppendString("G.729");
    order.AppendString("G.723");
    order.AppendString("iLBC-");
    order.AppendString("H.264");
    order.AppendString("H.263");
    order.AppendString("H.261");
    order.AppendString("h.239(h.264");
    order.AppendString("h.239(h.263");
    order.AppendString("h.239(h.261");
    capabilities.Reorder(order);

    PTRACE(2, "EP\tLoaded Codecs: " << capabilities);

#ifdef H323_H235
    if (m_encryptmedia.AsInteger() == 1) {
        InitialiseMediaEncryption();
        const char * defaulth235codecs[] = { "G.722", "G.711", "G.729", "G.723", "H.264", "H.261" };
        PStringArray h235codecs(sizeof(defaulth235codecs) / sizeof(defaulth235codecs[0]), defaulth235codecs);
        H235Capabilities::SetH235Codecs(h235codecs);
    }
#endif

#ifdef H323_TLS
    if (m_encryptsignal.AsInteger() == 1)
        InitialiseSignallingEncryption();
#endif

    PString iface = PIPSocket::GetGatewayInterfaceAddress(4);
    PIPSocket::Address interfaceAddress(iface);
    WORD interfacePort = m_listenport.AsInteger();

    H323ListenerTCP * listener = new H323ListenerTCP(*this, interfaceAddress, interfacePort);
    if (!StartListener(listener)) {
        PTRACE(2, "EP\tCould not open H.323 listener port on \"" << iface << '"');
        return false;
    }

    m_endpointIsSetup = true;
    return true;
}

PBoolean PlusEndPoint::InitialiseGateway()
{
    if (!m_h46017GW) {
        PTRACE(4, "EP\tGateway already set!");
        return true;
    }
#ifdef P_DNS
    PStringList gwsvr;
    PString server = "h323:user@" + PString(defGWsrv);
    if (PDNS::LookupSRV(server, "_h323cs._tcp.", gwsvr)) {
        if (m_stunType < 5) {
            PStringList gw = gwsvr[0].Tokenise("@");
            m_h46017GW = gw[1];
        }
        else {
            PIPSocket::Address ip; WORD port;
            for (PINDEX j = 0; j < gwsvr.GetSize(); ++j) {
                PStringList gw = gwsvr[j].Tokenise("@");
                H323TransportAddress taddr(gw[1]);
                taddr.GetIpAndPort(ip, port);
                if (m_stunType < 7 && port == 2775) {
                    m_h46017GW = gw[1];
                    break;
                }
                else if (port == 443) {
                    m_h46017GW = gw[1];
                    break;
                }
            }
        }
    }
    PStringList stunsvr;
    PString stun = "h323:user@" + PString(defSTUNsrv);
    if (PDNS::LookupSRV(stun, "_stun._udp.", stunsvr)) {
        PStringList stun = stunsvr[0].Tokenise("@");
        SetSTUNServer(stun[1]);
    }
#else
    m_h46017GW = defGWsrv;
#endif
    PTRACE(4, "EP\tCall Gateway set to " << m_h46017GW);
    return true;
}

PBoolean PlusEndPoint::RegisterGatekeeper()
{
    if (gatekeeper) {
        PTRACE(4, "EP\tAlready Registered!");
        return true;
    }

    if (m_server.IsEmpty()) {
        PTRACE(4, "EP\tNo Gatekeeper!");
        return false;
    }

    if (!m_username) SetLocalUserName(m_username);
    if (!m_password) SetGatekeeperPassword(m_password);

    PStringList gkHost;
#if P_DNS
    PString number = m_server;
    if (number.Left(5) != "h323:")
        number = "h323:user@" + number;

    PStringList str;
    if (PDNS::LookupSRV(number, "_h323rs._udp.", str)) {
        PTRACE(5, "H323\t" << str.GetSize() << " UDP SRV Records found");
        for (PINDEX i = 0; i < str.GetSize(); i++) {
            PString newhost = str[i].Right(str[i].GetLength() - 5);
            PTRACE(4, "H323\tGK Added UDP SRV " << m_server << " to " << newhost);
            gkHost.AppendString(newhost);
        }
    }
#else
    gkHost.AppendString(m_server);
#endif

    for (PINDEX j = 0; j < gkHost.GetSize(); j++) {
        H323TransportAddress tempAddr(gkHost[j]);
        PIPSocket::Address gkIP;
        tempAddr.GetIpAddress(gkIP);
        PIPSocket::Address routeIP = PIPSocket::GetRouteInterfaceAddress(gkIP);
        PStringStream gface;
        gface << routeIP.AsString(true) << ":" << uPortBase;
        PTRACE(4, "H323\tGK Trying " << gkHost[j]);
        if (UseGatekeeper(gkHost[j], PString(), gface)) {
            PTRACE(4, "H323\tGK Registration Success to " << gkHost[j]);
            return true;
        }
        else {
            H323Gatekeeper::RegistrationFailReasons reason;
            if (gatekeeper) {
                reason = gatekeeper->GetRegistrationFailReason();
                if (reason == H323Gatekeeper::SecurityDenied ||
                    (reason == H323Gatekeeper::DuplicateAlias)) {
                    PTRACE(4, "H323\tGK Failure reason " << reason);
                    delete gatekeeper;
                    gatekeeper = NULL;
                    return false;
                }
            }
        }
    }
#ifdef H323_H46017
    if (H46017CreateConnection(m_server, true)) {
        PTRACE(4, "H323\tGK H.460.17 SRV Gatekeeper Tunneling.");
        return true;
    }
#endif
    PTRACE(4, "EP\tGK Registration completely FAILED!");
    if (gatekeeper) {
        delete gatekeeper;
        gatekeeper = NULL;
    }
    return false;

}

void PlusEndPoint::PlaceCall(const PString & address)
{
    if (m_initialised.AsInteger() == 0) {
        PTRACE(2, "EP\tERROR No call to " << address << " NOT initalised!");
        return;
    }
    if (!m_currentCallToken.IsEmpty()) {
        PTRACE(2, "EP\tERROR No call to " << address << " already in a call!");
        return;
    }
    m_callstate = "1";
    fire_incall(m_callstate);
    fire_callerid(address);
    FireStatus(uiConnectingCall);
    PString addr = address;
    if (!gatekeeper && !m_h46017GW)
        addr = address + PString("@") + m_h46017GW;

    MakeCall(addr, m_currentCallToken);
}

void PlusEndPoint::HangUp()
{
    if (m_currentCallToken.IsEmpty())
        return;

    H323Connection * connection = FindConnectionWithLock(m_currentCallToken);
    if (connection != NULL) {
        FireStatus(uiEndedByLocalUser);
        connection->ClearCall();
        connection->Unlock();
    }
}

void PlusEndPoint::AnswerCall()
{
    if (m_currentCallToken.IsEmpty())
        return;

    m_callstate = "1";
    fire_incall(m_callstate);

    H323Connection * connection = FindConnectionWithLock(m_currentCallToken);
    if (connection != NULL) {
        connection->AnsweringCall(H323Connection::AnswerCallNow);
        fire_callerid(connection->GetDisplayName());
        connection->Unlock();
    }
}

void PlusEndPoint::DTMF(const PString & key)
{
    if (!m_currentCallToken) {
        H323Connection * connection = FindConnectionWithLock(m_currentCallToken);
        if (connection != NULL) {
            connection->SendUserInput(key);
            connection->Unlock();
        }
    }
    else {
        PDTMFEncoder encoder(key[0], 75);
        PSound soundtone(1, 8000, 16, encoder.GetSize(), (BYTE *)encoder.GetPointer());
        if (!soundtone.Play()) {
            PTRACE(3, "LID\tError Opening " << m_audioplay << " for DTMF Tone.");
        }
    }
}


void PlusEndPoint::HandleSettingChange(PlusProcess::Setting setting, const PString & name, const PString & value, PString & dftValue)
{

    switch (setting) {
        case PlusProcess::e_curdrvvideoplay:    DRIVER_CHG(videoplay)   return;
        case PlusProcess::e_curdrvvideorec:     DRIVER_CHG(videorec)    return;
        case PlusProcess::e_curdrvaudioplay:    DRIVER_CHG(audioplay)   return;
        case PlusProcess::e_curdrvaudiorec:     DRIVER_CHG(audiorec)    return;
        default: 
#ifdef H323_DATASTORE
            // Save settings
            if (dftValue != "*") {  // Initialise Save
               dftValue = m_dataStore.GetString(defDBSectVar, name, dftValue); 
            } else  // General Save
               m_dataStore.SetString(defDBSectVar, name, value);
#endif
            break;
    }

    // Set Video Formats
    switch (setting) {
        case PlusProcess::e_videoinformat:  m_mediaManager.SetVideoFormat(H323Channel::IsReceiver, value);  return;
        case PlusProcess::e_videooutformat: m_mediaManager.SetVideoFormat(H323Channel::IsTransmitter, value);  return;
        default: break;
    }

    if (m_currentCallToken.IsEmpty())  // We are not currently in a call the exit.
        return;

    switch (setting) {
        case PlusProcess::e_audiomute:
            break;
        case PlusProcess::e_videomute:
            return;   //(m_videoBucket && m_videoBucket->VideoPause(value.AsInteger()));
        default:
            break;
    }

}

void PlusEndPoint::FireStatus(int status)
{
    fire_status(PString(status), UXStatus(m_language, status));
}


PlusMediaManager & PlusEndPoint::GetMediaManager()
{
    return m_mediaManager;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef H323_TLS   
PBoolean PlusEndPoint::InitialiseSignallingEncryption()
{

     DisableH245Tunneling(false);
//        LoadTLSDirectory(tlsCertDir);
//        TLS_AddCACertificate(ca);
     
    SetTLSMediaPolicy(H323TransportSecurity::e_reqTLSMediaEncHigh);
    if (!TLS_Initialise()) {
        PTRACE(2, "TLS\tERROR: TLS not initialised.");
        return false;
    }

    PTRACE(4, "TLS\tERROR: TLS Initialised.");
    return true;

}
#endif

#ifdef H323_H235

#ifdef H323_H235_AES256
PBoolean GenerateDHParameters(PStringToString & param)
{
    PTRACE(4, "H235_DH\tDH Generation started.");
    PTimeInterval start = PTimer::Tick();
    H235_DiffieHellman::Generate(1536,2,param);
    PTimeInterval lapse = PTimer::Tick() - start;
    PTRACE(4, "H235_DH\tDH Generation complete " << lapse.GetSeconds() << "sec.");
    return (param.GetSize() > 0);
}
#endif

void PlusEndPoint::ThreadGenerate(PThread &, H323_INT)
{
    fire_dhGenerate("1");
    PStringToString params;
    if (GenerateDHParameters(params)) {
        set_dhOID(params["OID"]);
        set_dhPrime(params["PRIME"]);
        set_dhGenerator(params["GENERATOR"]);
    }
    fire_dhGenerate("0");
}

void PlusEndPoint::GenerateParameters()
{
    m_generateThread = PThread::Create(PCREATE_NOTIFIER(ThreadGenerate), 0,
        PThread::AutoDeleteThread,
        PThread::NormalPriority,
        "plusep:%x");
}

PBoolean PlusEndPoint::InitialiseMediaEncryption()
{
    H235MediaCipher ncipher;
    unsigned maxtoken;

#ifdef H323_H235_AES256
    if (m_encryptmediahigh.AsInteger() == 1) {
        // Load the DH parameters if present
       if (!m_dhOID) {
            PBYTEArray lprime;
            PBYTEArray lgenerator;
            PBase64::Decode(m_dhPrime, lprime);
            PBase64::Decode(m_dhGenerator, lgenerator);
            H235Authenticators::LoadDHData(m_dhOID, lprime, lgenerator);
            ncipher = encypt192;
        } else 
            ncipher = encypt256;
        maxtoken = 2048;
    } else
#endif    
    {
        ncipher = encypt128;
        maxtoken = 1024;
    }

    SetH235MediaEncryption(encyptRequest, ncipher, maxtoken);
#ifdef H323_H235_AES256
    if (m_encryptmediahigh.AsInteger() == 1)
       EncryptionCacheInitialise();
#endif

    return true;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef PTRACING
void PlusEndPoint::InitialiseDebug()
{
    if (m_tracing.AsInteger() == 0)
        return;

    if (!defTraceLevel) {
        PTrace::Initialise(m_tracing.AsInteger(), defTraceFile,
            PTrace::DateAndTime | PTrace::TraceLevel | PTrace::FileAndLine);
    } else if (m_tracing != defTraceLevel) {
        PTrace::SetLevel(m_tracing.AsInteger());
    }

    PTRACE(1, "EP\tlibPLUS v" << m_version << " Trace Level " << m_tracing );
    PTRACE(4, "EP\t---------------------");

    int i;
    PStringArray drivers = PSoundChannel::GetDriverNames();
    PTRACE(4, "EP\tAvailable Audio Drivers: " << drivers.GetSize());
    if (drivers.GetSize() > 0) {
        for (i = 0; i < drivers.GetSize(); i++) {
            bool def = (drivers[i] == m_curdrvaudioplay);
            PTRACE(4, "EP\tDriver " << i << ": " << drivers[i] << "  " << (def ? "(set)" : ""));
        }
    }
    drivers.SetSize(0);

    PTRACE(4, "EP\t---------------------");
    drivers = PVideoInputDevice::GetDriverNames();
    PTRACE(4, "EP\tAvailable Video Record Drivers: " << drivers.GetSize());
    if (drivers.GetSize() > 0) {
        for (i = 0; i < drivers.GetSize(); i++) {
            bool def = (drivers[i] == m_curdrvvideorec);
            PTRACE(4, "EP\tDriver " << i << ": " << drivers[i] << "  " << (def ? "(set)" : ""));
        }
    }
    drivers.SetSize(0);

    PTRACE(4, "EP\t---------------------");
    drivers = PVideoOutputDevice::GetDriverNames();
    PTRACE(4, "EP\tAvailable Video Play Drivers: " << drivers.GetSize());
    if (drivers.GetSize() > 0) {
        for (i = 0; i < drivers.GetSize(); i++) {
            bool def = (drivers[i] == m_curdrvvideoplay);
            PTRACE(4, "EP\tDriver " << i << ": " << drivers[i] << "  " << (def ? "(set)" : ""));
        }
    }
    drivers.SetSize(0);

    PTRACE(4, "EP\t---------------------");
    PStringArray devSoundRec;
    EnumerateSoundDevices(true, m_curdrvaudiorec, devSoundRec);
    PTRACE(4, "EP\tAvailable Audio Record Devices: " << devSoundRec.GetSize() << " using " << m_curdrvaudiorec << " Driver");
    if (devSoundRec.GetSize() > 0) {
        for (i = 0; i < devSoundRec.GetSize(); i++) {
            bool def = (devSoundRec[i] == m_audiorec);
            PTRACE(4, "EP\tDevice " << i << ": " << devSoundRec[i] << "  " << (def ? "(set)" : ""));
        }
    }

    PTRACE(4, "EP\t---------------------");
    PStringArray devSoundPlay;
    EnumerateSoundDevices(false, m_curdrvaudioplay, devSoundPlay);
    PTRACE(4, "EP\tAvailable Audio Play Devices: " << devSoundPlay.GetSize() << " using " << m_curdrvaudioplay << " Driver");
    if (devSoundPlay.GetSize() > 0) {
        for (i = 0; i < devSoundPlay.GetSize(); i++) {
            bool def = (devSoundPlay[i] == m_audioplay);
            PTRACE(4, "EP\tDevice " << i << ": " << devSoundPlay[i] << "  " << (def ? "(set)" : ""));
        }
    }

    PTRACE(4, "EP\t---------------------");
    PStringArray devVideoRec;
    EnumerateVideoDevices(true, m_curdrvvideorec, devVideoRec);
    if (devVideoRec.GetSize() > 0) {
        PTRACE(4, "EP\tAvailable Video Record Devices: " << devVideoRec.GetSize() << " using " << m_curdrvvideorec << " Driver");
        for (i = 0; i < devVideoRec.GetSize(); i++) {
            bool def = (devVideoRec[i] == m_videorec);
            PTRACE(4, "EP\tDevice " << i << ": " << devVideoRec[i] << "  " << (def ? "(set)" : ""));
        }

        for (i = 0; i < devVideoRec.GetSize(); i++) {
            PVideoInputDevice::Capabilities caps;
            if (PVideoInputDevice::GetDeviceCapabilities(devVideoRec[i], m_curdrvvideorec, &caps)) {

                PTRACE(4, "EP\tDevice " << i << ": " << devVideoRec[i] << " capabilities.");
                for (std::list<PVideoFrameInfo>::const_iterator r = caps.framesizes.begin(); r != caps.framesizes.end(); ++r) {
                    PTRACE(4, "EP\t   w: " << r->GetFrameWidth() << " h: " << r->GetFrameHeight() << " fmt: " << r->GetColourFormat() << " fps: " << r->GetFrameRate());
                }

                if (caps.controls.size() > 0) {
                    PTRACE(4, "EP\t  Camera Controls");
                    for (std::list<PVideoControlInfo>::const_iterator r = caps.controls.begin(); r != caps.controls.end(); ++r) {
#if PTLIB_VER >= 2140
                        PTRACE(4, "EP\tType " << r->GetType() << " step: " << r->GetStep());
#else
                        PTRACE(4, "EP\tType " << r->m_type << " Max: " << r->m_max << " Min: " << r->m_min << " step: " << r->m_step);
#endif
                    }
                }
            }
        }
    }

    PTRACE(4, "EP\t---------------------");
    PStringArray devVideoPlay;
    EnumerateVideoDevices(false, m_curdrvvideoplay, devVideoPlay);
    if (devVideoPlay.GetSize() > 0) {
        PTRACE(4, "EP\tAvailable Video Play Devices: " << devVideoPlay.GetSize() << " using " << m_curdrvvideoplay << " Driver");
        for (i = 0; i < devVideoPlay.GetSize(); i++) {
            bool def = (devVideoPlay[i] == m_videoplay);
            PTRACE(4, "EP\tDevice " << i << ": " << devVideoPlay[i] << "  " << (def ? "(set)" : ""));
        }
    }
    PTRACE(4, "EP\t---------------------");
}
#endif  // PTRACING

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Media


bool PlusEndPoint::inaudio(void * data, int size, int width, int height)
{
    return false;
}

bool PlusEndPoint::invideo(void * data, int size, int width, int height)
{
    return false;
}

bool PlusEndPoint::incontent(void * data, int size, int width, int height)
{
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Utilities

PStringList PlusEndPoint::GetConfigurationSections() const
{
    return PConfig(m_configFile, PString()).GetSections();
}

PConfig * PlusEndPoint::GetConfigurationRules(const PString & section)
{
    return new PConfig(m_configFile, section);
}

PConfig * PlusEndPoint::GetConfiguration(const PString & device)
{
    for (PINDEX i = 0; i < m_configList.GetSize(); i++) {
        if (device.Find(m_configList[i]) != P_MAX_INDEX)
            return GetConfigurationRules(m_configList[i]);
    }
    return NULL;
}

H323Capability::CapabilityFrameSize PlusEndPoint::GetFrameSize()
{
    H323Capability::CapabilityFrameSize m_frameSize;
    switch (m_quality.AsInteger()) {
    case 1:  m_frameSize = H323Capability::cifMPI; break;
    case 2:  m_frameSize = H323Capability::i480MPI; break;
    case 3:  m_frameSize = H323Capability::p720MPI; break;
    default:  m_frameSize = H323Capability::cifMPI; break;
    }
    return m_frameSize;
}

int PlusEndPoint::GetVideoBitRate()
{
    int m_bitrate;
    switch (m_quality.AsInteger()) {
    case 1:  m_bitrate = 512000; break;
    case 2:  m_bitrate = 1024000; break;
    case 3:  m_bitrate = 2048000; break;
    default:  m_bitrate = 512000; break;
    }
    return m_bitrate;
}

PBoolean PlusEndPoint::GetEmphasisSpeed()
{
    return (m_accessability.AsInteger() == 1);
}

PBoolean PlusEndPoint::GetContent()
{
    return (m_content.AsInteger() == 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef H323_H224
PBoolean PlusEndPoint::IsH224Active(PBoolean isTransmitter, const PString & id)
{
    if (id == "H281" && IsSet(h281support, 1)) { PlusSetValue(h281call, 1) return true; }
    if (id == "T140" && IsSet(t140support, 1)) { PlusSetValue(t140call, 1) return true; }
    if (id == "H284" && IsSet(h284support, 1)) { PlusSetValue(h284call, 1) return true; }
    return false;
}

void PlusEndPoint::H224RemoteSupportDetected(const PString & id)
{
    H224_Handler * handler = GetH224Handler(id);
    if (handler) {
        handler->SetLocalSupport();
        PTRACE(4, "H224\tRemote " << id << " Support detected.");
    }
    else {
        PTRACE(4, "H224\tRemote " << id << " Support detected. No Handler!");
    }
}

H224_Handler * PlusEndPoint::GetH224Handler(const PString & id)
{
    H224_Handler * handler = NULL;
    PlusConnection * connection = (PlusConnection *)FindConnectionWithLock(m_currentCallToken);
    if (connection != NULL) {
        handler = connection->GetH224Handler(id);
        connection->Unlock();
    }
    return handler;
}
#endif  // H323_H224

PBoolean PlusEndPoint::H224ReceivedSettings(const PString & id, BYTE cpid, const BYTE * data, int & length)
{
#ifdef H224_H284
    H224_Handler * handler = GetH224Handler(id);
    if (handler && id == "H284") {
        H224_H284Handler * h284 = (H224_H284Handler*)handler;
        H284_ControlPoint * cp = h284->GetControlPoint(cpid);
        if (cp && cp->SetData(data, length))
            return true;
    }
#endif
    return false;
}





/////////////////////////////////////////////////////////////////////////////////////////
// H.281 Far End Camera control

#ifdef H224_H281
class MyH224_H281Handler : public H224_H281Handler
{
public:
    MyH224_H281Handler(OpalH224Handler * h224Handler);
    ~MyH224_H281Handler();

    void AttachEndpoint(PlusEndPoint * ep);

    virtual PBoolean IsActive(H323Channel::Directions dir) const;

    virtual void OnRemoteSupportDetected();

    virtual void OnStartAction(H281_Frame::PanDirection panDirection,
        H281_Frame::TiltDirection tiltDirection,
        H281_Frame::ZoomDirection zoomDirection,
        H281_Frame::FocusDirection focusDirection);
    virtual void OnStopAction();

protected:
    PlusEndPoint      * ep;

};
#endif // H224_H281


void PlusEndPoint::FECC(const PString & key, const PString & direction, const PString & start)
{
#ifdef H224_H281
    PWaitAndSignal m(m_feccMutex);

    MyH224_H281Handler * FECCHandler = (MyH224_H281Handler *)GetH224Handler("H281");
    if (!FECCHandler)
        return;

    H281_Frame::PanDirection pan = H281_Frame::NoPan;
    H281_Frame::TiltDirection tilt = H281_Frame::NoTilt;
    H281_Frame::ZoomDirection zoom = H281_Frame::NoZoom;
    H281_Frame::FocusDirection focus = H281_Frame::NoFocus;

    int dir = direction.AsInteger();
    if (start.AsInteger()) {
        switch (key.AsInteger()) {
        case 1:   // Pan
            PTRACE(4, "EP\tSend FECC Instruction Pan " << (dir > 0 ? "left" : "right"));
            if (dir > 0)
                pan = H281_Frame::PanLeft;
            else
                pan = H281_Frame::PanRight;
            break;
        case 2:  // Tilt
            PTRACE(4, "EP\tSend FECC Instruction Tilt " << (dir > 0 ? "up" : "down"));
            if (dir > 0)
                tilt = H281_Frame::TiltUp;
            else
                tilt = H281_Frame::TiltDown;
            break;
        case 3:  // Roll
        case 4:  // Zoom
            PTRACE(4, "EP\tSend FECC Instruction Zoom " << (dir > 0 ? "in" : "out"));
            if (dir > 0)
                zoom = H281_Frame::ZoomIn;
            else
                zoom = H281_Frame::ZoomOut;
            break;
        }

        FECCHandler->StartAction(pan, tilt, zoom, focus);
    }
    else {
        PTRACE(4, "EP\tStop FECC Instruction.");
        FECCHandler->StopAction();
    }
#endif  // H224_H281
}

#ifdef H224_H281
void PlusEndPoint::SendH281Start(H281_Frame::PanDirection panDirection, H281_Frame::TiltDirection tiltDirection,
    H281_Frame::ZoomDirection zoomDirection, H281_Frame::FocusDirection focusDirection)
{
    MyH224_H281Handler * h281 = (MyH224_H281Handler *)GetH224Handler("H281");
    if (h281)
        h281->StartAction(panDirection, tiltDirection, zoomDirection, focusDirection);
}

void PlusEndPoint::SendH281Stop()
{
    MyH224_H281Handler * h281 = (MyH224_H281Handler *)GetH224Handler("H281");
    if (h281)
        h281->StopAction();
}

void PlusEndPoint::StartH281Action(H281_Frame::PanDirection panDirection, H281_Frame::TiltDirection tiltDirection,
    H281_Frame::ZoomDirection zoomDirection, H281_Frame::FocusDirection focusDirection)
{

}

void PlusEndPoint::StopH281Action()
{

}


MyH224_H281Handler::MyH224_H281Handler(OpalH224Handler * h224Handler)
    : H224_H281Handler(), ep(NULL)
{
    m_h224Handler = h224Handler;
    m_direction = h224Handler->GetDirection();
}

MyH224_H281Handler::~MyH224_H281Handler()
{
}

PBoolean MyH224_H281Handler::IsActive(H323Channel::Directions dir) const
{
    return ep && ep->IsH224Active(dir, GetName());
}

void MyH224_H281Handler::OnRemoteSupportDetected()
{
    if (ep)
        ep->H224RemoteSupportDetected(GetName());
}

void MyH224_H281Handler::AttachEndpoint(PlusEndPoint * _ep)
{
    if (!ep)
        ep = _ep;
}

void MyH224_H281Handler::OnStartAction(H281_Frame::PanDirection panDirection,
    H281_Frame::TiltDirection tiltDirection,
    H281_Frame::ZoomDirection zoomDirection,
    H281_Frame::FocusDirection focusDirection)
{
    if (ep)
        ep->StartH281Action(panDirection, tiltDirection, zoomDirection, focusDirection);
}

void MyH224_H281Handler::OnStopAction()
{
    if (ep)
        ep->StopH281Action();
}

#endif // H224_H281

//########################################################################################
// T.140 Real-Time text

#if defined(H224_T140)

class MyH224_T140Handler : public H224_T140Handler
{
public:
    MyH224_T140Handler(OpalH224Handler * h224Handler);
    ~MyH224_T140Handler();

    void AttachEndpoint(PlusEndPoint * ep);

    virtual void OnRemoteSupportDetected();

    virtual PBoolean IsActive(H323Channel::Directions dir) const;

    virtual void ReceivedBackSpace();
    virtual void ReceivedNewLine();
    virtual void ReceivedText(const PString & text);

protected:
    PlusEndPoint      * ep;
};

MyH224_T140Handler::MyH224_T140Handler(OpalH224Handler * h224Handler)
    : H224_T140Handler(), ep(NULL)
{
    m_h224Handler = h224Handler;
    m_direction = h224Handler->GetDirection();
}

MyH224_T140Handler::~MyH224_T140Handler()
{

}

PBoolean MyH224_T140Handler::IsActive(H323Channel::Directions dir) const
{
    return ep && ep->IsH224Active(dir, GetName());
}

void MyH224_T140Handler::OnRemoteSupportDetected()
{
    if (ep)
        ep->H224RemoteSupportDetected(GetName());
}

void MyH224_T140Handler::AttachEndpoint(PlusEndPoint * _ep)
{
    if (!ep)
        ep = _ep;
}

void MyH224_T140Handler::ReceivedBackSpace()
{
    if (ep)
        ep->ReceivedT140BackSpace();
}

void MyH224_T140Handler::ReceivedNewLine()
{
    ReceivedText(" ");
}

void MyH224_T140Handler::ReceivedText(const PString & text)
{
    if (ep)
        ep->ReceivedT140Text(text);
}

#endif // H224_T140

void PlusEndPoint::SendH284Instruction(int id, int action, int value)
{
#ifdef H224_H284
    MyH224_H284Handler * h284 = (MyH224_H284Handler *)GetH224Handler("H284");
    if (h284)
        h284->SendInstruction((H224_H284Handler::ControlPointID)id, (H284_ControlPoint::Action)action, value);
#endif
}

#if defined(H224_T140) || defined(H323_T140)
void PlusEndPoint::SendT140BackSpace()
{
#if defined(H224_T140)
    MyH224_T140Handler * t140 = (MyH224_T140Handler *)GetH224Handler("T140");
    if (t140)
        t140->SendBackSpace();
#endif
}

void PlusEndPoint::SendT140Character(const PString & ch)
{
#if defined(H224_T140)
    MyH224_T140Handler * t140 = (MyH224_T140Handler *)GetH224Handler("T140");
    if (t140)
        t140->SendCharacter(ch);
#endif
}

void PlusEndPoint::ReceivedT140Text(const PString & text)
{
    PTRACE(6, "H224\tReceived T140 " << text);

}

void PlusEndPoint::ReceivedT140BackSpace()
{

}
#endif //defined(H323_T140) || defined(H224_T140)


//======================================================================================

#ifdef H224_H284
PBoolean PlusEndPoint::OnCreateH284ControlID(H224_H284Handler::ControlPointID id, H284_ControlPoint & cp)
{
    switch (id) {
    case H224_H284Handler::e_NeckPan:
        cp.SetData(true, true, 0, 0, 180, 90, 72, 108);
        break;
    case H224_H284Handler::e_NeckTilt:
        cp.SetData(true, true, 0, 0, 180, 90, 72, 108);
        break;
    default:
        return false;
    }
    return true;
}

void PlusEndPoint::ReceiveH284Instruction(H224_H284Handler::ControlPointID id, H284_ControlPoint::Action action, unsigned value) const
{
    PTRACE(4, "H224\tInstruction received " << id << " A: " << action << " V: " << value);
}

// Handler
class MyH224_H284Handler : public H224_H284Handler
{
public:
    MyH224_H284Handler(OpalH224Handler * h224Handler);
    ~MyH224_H284Handler();

    void AttachEndpoint(PlusEndPoint * ep);

    virtual PBoolean IsActive(H323Channel::Directions dir) const;

    virtual void OnRemoteSupportDetected();

    virtual PBoolean OnAddControlPoint(ControlPointID id, H284_ControlPoint & cp);
    virtual PBoolean OnReceivedControlData(BYTE id, const BYTE * data, int & length);
    virtual void ReceiveInstruction(ControlPointID id, H284_ControlPoint::Action action, unsigned value) const;

protected:
    PlusEndPoint      * ep;
};

//////////////////////////

MyH224_H284Handler::MyH224_H284Handler(OpalH224Handler * h224Handler)
    : H224_H284Handler(), ep(NULL)
{
    m_h224Handler = h224Handler;
    m_direction = h224Handler->GetDirection();
}

MyH224_H284Handler::~MyH224_H284Handler()
{

}

PBoolean MyH224_H284Handler::IsActive(H323Channel::Directions dir) const
{
    return ep && ep->IsH224Active(dir, GetName());
}

void MyH224_H284Handler::OnRemoteSupportDetected()
{
    if (ep)
        ep->H224RemoteSupportDetected(GetName());
}

void MyH224_H284Handler::AttachEndpoint(PlusEndPoint * _ep)
{
    if (!ep) {
        ep = _ep;
        Add(e_NeckPan);
        Add(e_NeckTilt);
    }
}

void MyH224_H284Handler::ReceiveInstruction(H224_H284Handler::ControlPointID id, H284_ControlPoint::Action action, unsigned value) const
{
    if (ep)
        ep->ReceiveH284Instruction(id, action, value);
}

PBoolean MyH224_H284Handler::OnAddControlPoint(H224_H284Handler::ControlPointID id, H284_ControlPoint & cp)
{
    if (m_direction == H323Channel::IsTransmitter)
        return (ep && ep->OnCreateH284ControlID(id, cp));
    else
        return true;
}

PBoolean MyH224_H284Handler::OnReceivedControlData(BYTE id, const BYTE * data, int & length)
{
    return (ep && ep->H224ReceivedSettings(GetName(), id, data, length));
}
#endif //H224_H284


#ifdef H323_H460P
void PlusEndPoint::PresenceInstruction(const PString & locAlias, unsigned type, const PString & subAlias,
    const PString & subDisplay, const PString & subAvatar)
{
    PStringStream data;
    data << PString(type) << "," << subAlias << "," << subDisplay << "," << subAvatar;
    fire_presence(data);
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PlusConnection

PlusConnection::PlusConnection(PlusEndPoint & ep, unsigned ref)
    : H323Connection(ep, ref), myEndpoint(ep)
{
#ifdef H323_H224
    startH224 = true;
#endif
}

PBoolean PlusConnection::OnReceivedSignalSetup(const H323SignalPDU & setupPDU)
{
    if (setupPDU.m_h323_uu_pdu.m_h323_message_body.GetTag() != H225_H323_UU_PDU_h323_message_body::e_setup)
        return FALSE;

    const H225_Setup_UUIE & setup = setupPDU.m_h323_uu_pdu.m_h323_message_body;

    SetRemoteApplication(setup.m_sourceInfo);
    SetRemotePartyInfo(setupPDU);
    HandleExceptions();

    return H323Connection::OnReceivedSignalSetup(setupPDU);
}

PBoolean PlusConnection::OnReceivedSignalConnect(const H323SignalPDU & pdu)
{
    if (pdu.m_h323_uu_pdu.m_h323_message_body.GetTag() != H225_H323_UU_PDU_h323_message_body::e_connect)
        return FALSE;
    const H225_Connect_UUIE & connect = pdu.m_h323_uu_pdu.m_h323_message_body;

    SetRemoteApplication(connect.m_destinationInfo);
    SetRemotePartyInfo(pdu);
    HandleExceptions();

    return H323Connection::OnReceivedSignalConnect(pdu);
}

void PlusConnection::HandleExceptions()
{
    PINDEX vid = 1;
    H323Capability::CapabilityFrameSize sz = myEndpoint.GetFrameSize();

    PTRACE(4, "H323\tChecking Exception Rules for " << GetRemoteApplication());
    PConfig * config = myEndpoint.GetConfiguration(GetRemoteApplication());
    if (config) {
        PTRACE(2, "H323\tException Rule Found. Applying rules...");
        if (config->HasKey("Remove")) {
            PStringList removeList = config->GetString("Remove").Tokenise(",");
            for (PINDEX i = 0; i<removeList.GetSize(); i++) {
                PTRACE(4, "H323\tRemoving Capability " << removeList[i]);
                localCapabilities.Remove(removeList[i].Trim() + "*");
            }
        }

        if (config->HasKey("Video")) {
            vid = config->GetString("Video").AsInteger();
        }

        if (config->HasKey("FrameSize")) {
            PCaselessString fs = config->GetString("FrameSize");
            if (fs == "qcif")  sz = H323Capability::qcifMPI;
            else if (fs == "cif")  sz = H323Capability::cifMPI;
            else if (fs == "4cif")  sz = H323Capability::cif4MPI;
            else                    sz = H323Capability::p720MPI;
            PTRACE(4, "H323\tDerating video to " << sz);
        }

        if (config->HasKey("Tone")) {
            PCaselessString tone = config->GetString("Tone");
            if (tone == "q931")         SetSendUserInputMode(H323Connection::SendUserInputAsQ931);
            else if (tone == "string")  SetSendUserInputMode(H323Connection::SendUserInputAsString);
            else if (tone == "dtmf")    SetSendUserInputMode(H323Connection::SendUserInputAsTone);
            else if (tone == "rfc2833") SetSendUserInputMode(H323Connection::SendUserInputAsInlineRFC2833);
            PTRACE(4, "H323\tTone set to " << tone);
        }

        if (config->HasKey("Priority")) {
            PCaselessString order = config->GetString("Priority");
            PStringArray newOrder = order.Tokenise(',');
            localCapabilities.Reorder(newOrder);
            PTRACE(4, "H323\tPriority order set " << order);
        }

        if (config->HasKey("FastStart")) {
            PINDEX fs = config->GetString("FastStart").AsInteger();
            if (fs)
                fastStartState = FastStartInitiate;
        }

        if (config->HasKey("H245InSetup")) {
            PINDEX is = config->GetString("H245InSetup").AsInteger();
            if (is)
                doH245inSETUP = true;
        }

        if (config->HasKey("H245QoS")) {
            PINDEX qos = config->GetString("H245QoS").AsInteger();
            if (!qos)
                doH245QoS = false;
        }
    }

    if (vid)
        localCapabilities.SetVideoFrameSize(sz, 1);
    else
        localCapabilities.RemoveCapability(H323Capability::e_Video);

    int bitRate;
    if (config && config->HasKey("MaxBitRate")) {
        bitRate = config->GetString("MaxBitRate").AsInteger();
        PTRACE(4, "H323\tDerating BitRate to " << bitRate);
    }
    else
        bitRate = myEndpoint.GetVideoBitRate();

    SetInitialBandwidth(H323Capability::e_Video, bitRate);
    SetEmphasisSpeed(H323Capability::e_Video, myEndpoint.GetEmphasisSpeed());
    // if (m_MaxPayload > 0)
    SetMaxPayloadSize(H323Capability::e_Video, 1200); //myEndpoint.GetMaxPayloadSize());

    if (!myEndpoint.GetContent()) {
        localCapabilities.RemoveCapability(H323Capability::e_ExtendVideo);
        localCapabilities.Remove("H.239 Control");
    }
    else if (bitRate < 540000)
        SetInitialBandwidth(H323Capability::e_ExtendVideo, bitRate);
}



H224_Handler * PlusConnection::CreateH224Handler(H323Channel::Directions /*dir*/, OpalH224Handler & h224Handler, const PString & id)
{
#ifdef H224_H281
    if (id == "H281") return new MyH224_H281Handler(&h224Handler);
#endif // H224_H281
#ifdef H224_T140
    if (id == "T140") return new MyH224_T140Handler(&h224Handler);
#endif // H224_T140
#ifdef H224_H284
    if (id == "H284") return new MyH224_H284Handler(&h224Handler);
#endif // H224_H284
   return NULL;
}

PBoolean PlusConnection::OnCreateH224Handler(H323Channel::Directions dir, const PString & id, H224_Handler * m_handler) const
{
#ifdef H323_H224
    return (PRemoveConst(PlusConnection, this))->SetH224Handler(id, m_handler, dir);
#else
    return false;
#endif
}

PBoolean PlusConnection::SetH224Handler(const PString & id, H224_Handler * handler, H323Channel::Directions dir)
{
    PlusEndPoint * ep = &(PlusEndPoint &)endpoint;

    PTRACE(4, "EP\tAttaching " << (dir == H323Channel::IsTransmitter ? "Transmit" : "Receive") << " " << id << " " << (ep->IsH224Active(dir, id) ? "Active" : "INACTIVE"));
#ifdef H224_H281
    if (id == "H281") {
        MyH224_H281Handler * h281 = (MyH224_H281Handler *)handler;
        h281->AttachEndpoint(ep);
        if (dir == H323Channel::IsTransmitter)
            m_h224handlerMap.insert(pair<PString, H224_Handler*>(id, h281));
        return true;
    }
#endif  // H224_H281

#ifdef H224_T140
    if (id == "T140") {
        MyH224_T140Handler * t140 = (MyH224_T140Handler *)handler;
        t140->AttachEndpoint(ep);
        if (dir == H323Channel::IsTransmitter)
            m_h224handlerMap.insert(pair<PString, H224_Handler*>(id, t140));
        return true;
    }
#endif // H224_T140

#ifdef H224_H284
    if (id == "H284") {
        MyH224_H284Handler * h284 = (MyH224_H284Handler *)handler;
        h284->AttachEndpoint(ep);
        if (dir == H323Channel::IsTransmitter)
            m_h224handlerMap.insert(pair<PString, H224_Handler*>(id, h284));
        return true;
    }
#endif // H224_H284

    return false;
}

H224_Handler * PlusConnection::GetH224Handler(const PString & id)
{
#ifdef H323_H224
    map<PString, H224_Handler*>::iterator it = m_h224handlerMap.find(id);
    if (it != m_h224handlerMap.end())
        return it->second;
    else
#endif // H323_H224
        return NULL;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//======================================================================================


