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
#define PlusProcessSetUserRestart    case e_username: SetLocalUserName(value); break;

#define PlusProcessMethod(name)  case e_##name: m_endpoint->Do##name(p1,p2,p3); break;

///////////////////////////////////////////////////////////////////////////////////////////
// Locals

PBoolean l_vEndPointLocked = true;   // Whether the endpoint is not available to receive settings
PString  l_vLocalUser = "default";

class Method_Buffer
{
public:

    struct Msg {

        Msg(int id=0, const PString & ver1="", const PString & ver2="", const PString & ver3="")
            : msgId(id), v1(ver1), v2(ver2), v3(ver3) {}

        int msgId;
        PString v1;
        PString v2;
        PString v3;
    };

    void AddMessage(int id, const PString & v1, const PString & v2, const PString & v3) {
        PWaitAndSignal m(lMute);
        Msg newMsg(id, v1, v2, v3);
        lBuffer.push(newMsg);
    };

    PBoolean GetMessage(Msg & msg) {
        PWaitAndSignal m(lMute);
        if (lBuffer.size() == 0)
            return false;
        msg = lBuffer.front();
        lBuffer.pop();
        return true;
    }

private:
    std::queue<Msg> lBuffer;

    PMutex lMute;
};

Method_Buffer l_vMessages;

///////////////////////////////////////////////////////////////////////////////////////////


PString PlusProcess::dataStoreName = defDBName;
PString PlusProcess::dataStoreKey = defDBPass;

PlusProcess::PlusProcess(PLUSdevice * apiInstance)
    : PLibraryProcess(LIBPLUS_MAKER, LIBPLUS_PRODUCT,
        OPENH323_MAJOR, OPENH323_MINOR, ReleaseCode, OPENH323_BUILD),
    m_endpoint(NULL), m_eventCallBack(apiInstance), m_endpointThread(NULL)
{
    // Open DB to default settings Table
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

    Initialise();
}

PlusProcess::~PlusProcess()
{
    Uninitialise();
    delete m_dataStore;
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


void PlusProcess::SetLocalUserName(const PString & username)
{
    // When we set the local username we have to unitialise and reinitialise the endpoint.
    l_vLocalUser = username;

    Uninitialise();
    PProcess::Sleep(200);
    Initialise();

}


void PlusProcess::ThreadEndpoint(PThread &, INT)
{
    PTRACE(2, "EP\tStarting Endpoint");
    m_endpoint = new PlusEndPoint(*this, *m_dataStore);

    l_vEndPointLocked = false;

    Method_Buffer::Msg msg;
    while (!m_shutdown.Wait(0)) {
        while (l_vMessages.GetMessage(msg))
            InternalDoMethod((Method)msg.msgId, msg.v1, msg.v2, msg.v3);

        PThread::Sleep(50);
    }

    delete m_endpoint;
    m_endpoint = NULL;

    PTRACE(2, "EP\tEndpoint Shutdown");
    m_shutdown.Acknowledge();
}

void PlusProcess::SetSetting(Setting set, const PString & value)
{

    while (l_vEndPointLocked) {
        PProcess::Sleep(250);
    }

    if (!m_endpoint) return;

    switch (set) {
        PlusProcessSetSetting(version)
        PlusProcessSetSetting(tracing)
        PlusProcessSetUserRestart     //PlusProcessSetSetting(username)
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
        PlusProcessSetSetting(secondVideo)
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
    if (l_vEndPointLocked)
        PThread::Sleep(200);  // Wait for the endpoint instance to be setup

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
        PlusProcessGetSetting(secondVideo)

        PlusProcessGetSetting(encryptsignal)
        PlusProcessGetSetting(encryptmedia)
        PlusProcessGetSetting(encryptmediahigh)
        // IMPL: Setting Names Here
    default: break;
    }
    return PString();
}


void PlusProcess::DoMethod(Method id, const PString & p1, const PString & p2, const PString & p3)
{
    if (!m_endpoint) return;

    l_vMessages.AddMessage(id, p1, p2, p3);
}


void PlusProcess::InternalDoMethod(Method id, const PString & p1, const PString & p2, const PString & p3)
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
        // IMPL: Method Names Here
    }
}

void PlusProcess::HandleEvent(Event evt, const std::string & p1, const std::string & p2, const std::string & p3, const std::string & p4)
{
      m_eventCallBack->Event((int)evt, p1.c_str(), p2.c_str(), p3.c_str(), p4.c_str());
}