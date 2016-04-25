/*
* plusprocess.cxx
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
#include "plusprocess.h"

#include <queue>

///////////////////////////////////////////////////////////////////////////////////////////
// PlusProcess MACROS

#define PlusProcessGetSetting(name)  case e_##name: return m_endpoint->get_##name();
#define PlusProcessSetSetting(name)  case e_##name: m_endpoint->set_##name(value); break;
#define PlusProcessSet(name)         case e_##name: set##name(value); break;

#define PlusProcessMethod(name)  case e_##name: { \
                                        PTRACE(6, "EP\tDo Method " << #name << " vals: " << p1 << " " << p2 << " " << p3 << " " << p4); \
                                        m_endpoint->Do##name(p1,p2,p3,p4); break; }

#define PLUSProcessMediaBody(name) \
bool PlusProcess::in##name(void * data, int size, int width, int height) { return false; } \
bool PlusProcess::out##name(void * data, int size, int width, int height) { return m_eventCallBack->out##name(data, size, width, height); }


///////////////////////////////////////////////////////////////////////////////////////////
// Locals

PBoolean l_vEndPointLocked = true;   // Whether the endpoint is not available to receive settings
PString  l_vLocalUser = defUserName;

///////////////////////////////////////////////////////////////////////////////////////////


PString PlusProcess::dataStoreName = defDBName;
PString PlusProcess::dataStoreKey = defDBPass;

PlusProcess::PlusProcess(PLUSdevice * apiInstance)
    : PLibraryProcess(LIBPLUS_MAKER, LIBPLUS_PRODUCT,
        OPENH323_MAJOR, OPENH323_MINOR, ReleaseCode, OPENH323_BUILD),
    m_endpoint(NULL), m_eventCallBack(apiInstance), m_endpointThread(NULL)
{

    if (defTraceLevel > 0)
        PTrace::Initialise( defTraceLevel, defTraceFile, 
            PTrace::DateAndTime | PTrace::TraceLevel | PTrace::FileAndLine);

    // Open DB to default settings Table
#ifdef H323_DATASTORE
    m_dataStore = new H323DataStore(dataStoreName, dataStoreKey, defDBTable, defDBId);
    if (!m_dataStore->HasKey(defDBSection, defDBKey)) {
        l_vLocalUser = m_dataStore->GetString(defDBSection, defDBKey, l_vLocalUser);
        m_dataStore->CreateTable(defDBSetTable);
    } else {
        l_vLocalUser = m_dataStore->GetString(defDBSection, defDBKey, l_vLocalUser);
        m_dataStore->SetTable(defDBSetTable);
    }
        
    m_dataStore->SetDefaultID(l_vLocalUser);
    m_dataStore->SetDefaultSection(defDBSectVar);
#endif
    Initialise();
}

PlusProcess::~PlusProcess()
{
    Uninitialise();
#ifdef H323_DATASTORE
    delete m_dataStore;
#endif
}

void PlusProcess::Uninitialise()
{
    if (!l_vEndPointLocked) {
        l_vEndPointLocked = true;
        m_shutdown.Signal();
        m_endpointThread = NULL;
    }
}


void PlusProcess::Initialise()
{
    if (m_eventCallBack == NULL)
        return;

    if (l_vEndPointLocked) {
        m_endpointThread = PThread::Create(PCREATE_NOTIFIER(ThreadEndpoint), 0,
            PThread::AutoDeleteThread,
            PThread::NormalPriority,
            "plusep:%x");
    }
}


void PlusProcess::setusername(const PString & username)
{
    if (l_vLocalUser == username)
        return;

    if (username.IsEmpty())
        l_vLocalUser = defUserName;
    else
        l_vLocalUser = username;

#ifdef H323_DATASTORE
    if (m_dataStore) {
        m_dataStore->SetTable(defDBTable);
        m_dataStore->SetString(defDBSection, defDBKey, l_vLocalUser);

        m_dataStore->SetTable(defDBSetTable);
        m_dataStore->SetDefaultID(l_vLocalUser);
        m_dataStore->SetDefaultSection(defDBSectVar);
    }
#endif
    m_endpoint->set_username(l_vLocalUser);
    m_endpoint->InitialiseSettings(true);
}

bool PlusProcess::IsLoading()
{
    return l_vEndPointLocked;
}


void PlusProcess::ThreadEndpoint(PThread &, H323_INT)
{
    PTRACE(2, "EP\tStarting Endpoint");
#ifdef H323_DATASTORE
    m_endpoint = new PlusEndPoint(*this, *m_dataStore);
#else
    m_endpoint = new PlusEndPoint(*this);
#endif

    l_vEndPointLocked = false;

    while (!m_shutdown.Wait(0)) {
        ProcessMessages();
        ProcessMediaSamples();
        PThread::Sleep(2);
    }

    delete m_endpoint;
    m_endpoint = NULL;

    PTRACE(2, "EP\tEndpoint Shutdown");
    m_shutdown.Acknowledge();
}

void PlusProcess::SetSetting(Setting set, const PString & value)
{

    if (IsLoading())
        PThread::Sleep(250);

    if (!m_endpoint) return;

    switch (set) {
        PlusProcessSetSetting(version)
        PlusProcessSetSetting(tracing)
        PlusProcessSet(username)   // Setting username changes profile
        PlusProcessSetSetting(password)
        PlusProcessSetSetting(server)
        PlusProcessSetSetting(quality)
        PlusProcessSetSetting(accessability)
        PlusProcessSetSetting(content)
        PlusProcessSetSetting(autoanswer)

        //PlusProcessSetSetting(drvvideoplay)
        //PlusProcessSetSetting(drvvideorec)
        //PlusProcessSetSetting(drvaudioplay)
        //PlusProcessSetSetting(drvaudiorec)
        PlusProcessSetSetting(curdrvvideoplay)
        PlusProcessSetSetting(curdrvvideorec)
        PlusProcessSetSetting(curdrvaudioplay)
        PlusProcessSetSetting(curdrvaudiorec)
        //PlusProcessSetSetting(devvideoplay)
        //PlusProcessSetSetting(devvideorec)
        //PlusProcessSetSetting(devaudioplay)
        //PlusProcessSetSetting(devaudiorec)
        PlusProcessSetSetting(audioplay)
        PlusProcessSetSetting(audiorec)
        PlusProcessSetSetting(videoplay)
        PlusProcessSetSetting(videorec)

        PlusProcessSetSetting(call)
        PlusProcessSetSetting(audiomute)
        PlusProcessSetSetting(videomute)
        PlusProcessSetSetting(h281support)
        PlusProcessSetSetting(t140support)
        PlusProcessSetSetting(h284support)
        //PlusProcessSetSetting(h281call)
        //PlusProcessSetSetting(t140call)
        //PlusProcessSetSetting(h284call)
        PlusProcessSetSetting(usere164)
        PlusProcessSetSetting(useruri)
        PlusProcessSetSetting(showlocalvideo)
        //PlusProcessGetSetting(instanceid)
        //PlusProcessSetSetting(initialised)
        PlusProcessSetSetting(language)
        PlusProcessSetSetting(listenport)
        PlusProcessSetSetting(videoformats)
        PlusProcessSetSetting(videoinformat)
        PlusProcessSetSetting(videooutformat)
        PlusProcessSetSetting(secondvideo)

        PlusProcessSetSetting(encryptsignal)
        PlusProcessSetSetting(encryptmedia)
        PlusProcessSetSetting(encryptmediahigh)
        // IMPL: Setting Names Here
        default:
            break;
    }
}

PString PlusProcess::GetSetting(Setting set)
{
    if (IsLoading())
        PThread::Sleep(250);  // Wait for the endpoint instance to be setup

    if (!m_endpoint) return PString();

    switch (set) {
        PlusProcessGetSetting(version)
        PlusProcessGetSetting(tracing)
        PlusProcessGetSetting(username)
        PlusProcessGetSetting(password)
        PlusProcessGetSetting(server)
        PlusProcessGetSetting(quality)
        PlusProcessGetSetting(accessability)
        PlusProcessGetSetting(content)
        PlusProcessGetSetting(autoanswer)

        PlusProcessGetSetting(drvvideoplay)
        PlusProcessGetSetting(drvvideorec)
        PlusProcessGetSetting(drvaudioplay)
        PlusProcessGetSetting(drvaudiorec)
        PlusProcessGetSetting(curdrvvideoplay)
        PlusProcessGetSetting(curdrvvideorec)
        PlusProcessGetSetting(curdrvaudioplay)
        PlusProcessGetSetting(curdrvaudiorec)
        PlusProcessGetSetting(devvideoplay)
        PlusProcessGetSetting(devvideorec)
        PlusProcessGetSetting(devaudioplay)
        PlusProcessGetSetting(devaudiorec)
        PlusProcessGetSetting(audioplay)
        PlusProcessGetSetting(audiorec)
        PlusProcessGetSetting(videoplay)
        PlusProcessGetSetting(videorec)

        PlusProcessGetSetting(call)
        PlusProcessGetSetting(audiomute)
        PlusProcessGetSetting(videomute)
        PlusProcessGetSetting(callstate)
        PlusProcessGetSetting(h281support)
        PlusProcessGetSetting(t140support)
        PlusProcessGetSetting(h284support)
        PlusProcessGetSetting(h281call)
        PlusProcessGetSetting(t140call)
        PlusProcessGetSetting(h284call)
        PlusProcessGetSetting(usere164)
        PlusProcessGetSetting(useruri)
        PlusProcessGetSetting(showlocalvideo)
        //PlusProcessGetSetting(instanceid)
        PlusProcessGetSetting(initialised)
        PlusProcessGetSetting(language)
        PlusProcessGetSetting(listenport)
        PlusProcessGetSetting(videoformats)
        PlusProcessGetSetting(videoinformat)
        PlusProcessGetSetting(videooutformat)
        PlusProcessGetSetting(secondvideo)

        PlusProcessGetSetting(encryptsignal)
        PlusProcessGetSetting(encryptmedia)
        PlusProcessGetSetting(encryptmediahigh)
        // IMPL: Setting Names Here
        default: break;
    }
    return PString();
}


void PlusProcess::DoMethod(Method id, const PString & p1, const PString & p2, const PString & p3, const PString & p4)
{
    if (!m_endpoint) return;

    m_messages.AddMessage(id, p1, p2, p3,p4);
}


void PlusProcess::InternalDoMethod(Method id, const PString & p1, const PString & p2, const PString & p3, const PString & p4)
{
    switch (id) {
        PlusProcessMethod(placecall)
        PlusProcessMethod(hangup)
        PlusProcessMethod(answer)
        PlusProcessMethod(dtmf)
        PlusProcessMethod(fecc)
        PlusProcessMethod(start)
        PlusProcessMethod(h284instruction)
        PlusProcessMethod(h281text)
        PlusProcessMethod(h281return)
        PlusProcessMethod(secondcall)
        PlusProcessMethod(stop) 
        PlusProcessMethod(dhParameters)
        PlusProcessMethod(videosize)
        PlusProcessMethod(userMethod)
        // IMPL: Method Names Here
        default: break;
    }
}

void PlusProcess::HandleEvent(Event evt, const std::string & p1, const std::string & p2, const std::string & p3, const std::string & p4)
{
      m_eventCallBack->Event((int)evt, p1.c_str(), p2.c_str(), p3.c_str(), p4.c_str());
}


PlusProcess::Sample::Sample()
    : PBYTEArray(defBufferSize), m_id(-1), m_width(0), m_height(0)
{

}


void PlusProcess::ProcessMessages()
{
    if (m_messages.GetMessage(m_msg))
        InternalDoMethod((Method)m_msg.msgId, m_msg.v1, m_msg.v2, m_msg.v3, m_msg.v4);
}


void PlusProcess::ProcessMediaSamples()
{
    if (m_endpoint->GetMediaManager().ProcessMediaSamples(m_mediaBuffer.m_id, m_mediaBuffer.GetPointer(), m_mediaBuffer.m_size, m_mediaBuffer.m_width, m_mediaBuffer.m_height)) {
        switch (m_mediaBuffer.m_id) {
            case PlusMediaManager::e_audioOut: 
                break;
            case PlusMediaManager::e_videoOut:
                outvideo(m_mediaBuffer.GetPointer(), m_mediaBuffer.m_size, m_mediaBuffer.m_width, m_mediaBuffer.m_height);
                break;
            case PlusMediaManager::e_contentOut:
                break;
            default: break;
        }
    }
}

// Media Handling
PLUSProcessMediaBody(audio)
PLUSProcessMediaBody(video)
PLUSProcessMediaBody(content)



