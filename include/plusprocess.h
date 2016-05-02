/*
* plusprocess.h
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

#include "libplus.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Messages
{
public:
    struct Msg {
        Msg(int id = 0, const PString & ver1 = "", const PString & ver2 = "", const PString & ver3 = "", const PString & ver4 = "")
            : msgId(id), v1(ver1), v2(ver2), v3(ver3), v4(ver4) {}
        int msgId; 
        PString v1, v2, v3, v4;
    };

    void AddMessage(int id, const PString & v1, const PString & v2, const PString & v3, const PString & v4) {
        PWaitAndSignal m(lMute);
        lBuffer.push(Msg(id, v1, v2, v3, v4));
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

////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PLUSProcessMedia(name) \
bool in##name(void * data, int size, int width = 0, int height = 0); \
bool out##name(void * data, int size, int width = 0, int height = 0);

class PlusEndPoint;
class PlusProcess : public PLibraryProcess
{

public:
    PlusProcess(PLUSdevice * apiInstance);
    ~PlusProcess();

    enum Setting {
        e_version = 1,
        e_tracing,
        e_username,
        e_password,
        e_server,
        e_quality,
        e_accessability,
        e_content,
        e_autoanswer,
        e_drvvideoplay,
        e_drvvideorec,
        e_drvaudioplay,
        e_drvaudiorec,
        e_curdrvvideoplay,
        e_curdrvvideorec,
        e_curdrvaudioplay,
        e_curdrvaudiorec,
        e_devvideoplay,
        e_devvideorec,
        e_devaudioplay,
        e_devaudiorec,
        e_audiorec,
        e_audioplay,
        e_videoplay,
        e_videorec,
        e_call,
        e_audiomute,
        e_videomute,
        e_framewidth,
        e_frameheight,
        e_callstate,
        e_h281support,
        e_t140support,
        e_h284support,
        e_h281call,
        e_t140call,
        e_h284call,
        e_usere164,
        e_useruri,
        e_showlocalvideo,
        e_instanceid,
        e_initialised,
        e_language,
        e_listenport,
        e_videoformats,
        e_videoinformat,
        e_videooutformat,
        e_secondvideo,

        e_encryptsignal = 101,
        e_encryptmedia,
        e_encryptmediahigh,

        // Internal Use Only
        e_storageName = 991,
        e_storageKey,
        e_dhOID,
        e_dhPrime,
        e_dhGenerator,

        e_userSetting = 1000
        // IMPL: Setting Names Here
    };

    enum Method {
        e_placecall = 1,
        e_hangup,
        e_answer,
        e_dtmf,
        e_fecc,
        e_start,
        e_h284instruction,
        e_h281text,
        e_h281return,
        e_secondcall,
        e_stop,
        e_dhParameters,
        e_videosize,
        e_audioformat,
        e_userMethod = 1000
        // IMPL: Method Names Here
    };

    enum Event {
        e_eventNotDefined,
        e_status,
        e_isinitialised,
        e_notused1,
        e_callerid,
        e_incomingcall,
        e_incall,
        e_encryption,
        e_ish281call,
        e_ist140call,
        e_ish284call,
        e_isusere164,
        e_isuseruri,
        e_presence,
        e_duplicate,
        e_forwardcall,
        e_dhGenerate,
        e_userEvent = 1000
        // IMPL: Event Names Here
    };

    void    SetSetting(Setting set, const PString & value);
    PString GetSetting(Setting set);

    void DoMethod(Method id, const PString & p1, const PString & p2, const PString & p3, const PString & p4);

    // Events
    void HandleEvent(Event evt, const std::string & p1 = "", const std::string & p2 = "", const std::string & p3 = "", const std::string & p4 = "");


    struct Sample : public PBYTEArray
    {
        Sample();

        unsigned m_id;
        unsigned m_size;
        unsigned m_width;
        unsigned m_height;
    };

    /* Media Handling
    */
    PLUSProcessMedia(audio)
    PLUSProcessMedia(video)
    PLUSProcessMedia(content)

	// Internal
    bool IsLoading();

    static PString dataStoreName;
    static PString dataStoreKey;

protected:

    void Initialise();
    void Uninitialise();

    void setusername(const PString & username);

    void InternalDoMethod(Method id, const PString & p1, const PString & p2, const PString & p3, const PString & p4);

    void ProcessMessages();
    void ProcessMediaSamples();

private:
    PlusEndPoint *  m_endpoint;
    PLUSdevice*     m_eventCallBack;
    PThread *       m_endpointThread;
    PDECLARE_NOTIFIER(PThread, PlusProcess, ThreadEndpoint);

    Messages            m_messages;
    Messages::Msg       m_msg;
    Sample              m_mediaBuffer;

#ifdef H323_DATASTORE
    H323DataStore * m_dataStore;
#endif
    PSyncPointAck   m_shutdown;

};
