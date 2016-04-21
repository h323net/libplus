/*
* libplus.h
*
* Copyright (c) 2015 ISVO (Asia) Pte Ltd. All Rights Reserved.
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

#define PlusDevMedia(name) \
bool in##name(void * data, int size, int width=0, int height=0); \
virtual bool out##name(void * data, int size, int width=0, int height=0);


/* PLUSdevice
    PLUSdevice class
*/
class PLUSdevice
{
public:

    /* Class Constructor
        This will instance the device process
     */
    PLUSdevice();

    /* Class Destructor
        This will destroy the device process
    */
    virtual ~PLUSdevice();

    /* Setting
        Setting identifier to pass with Get_Value and Set_Value function
      */
    enum Setting {
        e_settingNotDefined,
        e_version,
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
        e_farEndCameraSupport,
        e_realTimeTextSupport,
        e_farEndMotorizedSupport,
        e_farEndCameraInCall,
        e_realTimeTextInCall,
        e_farEndMotorizedInCall,
        e_userPhoneNumber,
        e_userURIAddress,
        e_showlocalvideo,
        e_instanceid,
        e_initialised,
        e_language,
        e_listenport,
        e_videoformats,
        e_videoinformat,
        e_videooutformat,
        e_secondvideo,

        e_encryptSignal = 101,
        e_encryptMedia,
        e_encryptMediaHigh,

        // Internal settings
        e_storageName   = 991,
        e_storageKey,
        e_dhOID,
        e_dhPrime,
        e_dhGenerator,

        e_userDefinedSetting = 1000     
        // IMPL: Setting Names Here
    };

    /* Method
        Method identifier to pass with Call function
    */
    enum Method {
        e_methNotDefined,
        e_placeCall,
        e_hangupCall,
        e_answerCall,
        e_sendDTMF,
        e_farEndCamera,
        e_start,
        e_farEndMotorized,
        e_realTimeTextInput,
        e_realTimeTextNewLine,
        e_secondCall,
        e_stop,
        e_dhParameters,
        e_videosize,
        e_userDefinedMethod = 1000
        // IMPL: Method Names Here
    };

    /* Event
        Event identifier that is passed back with Event callback
    */
    enum Event {
        e_eventNotDefined,
        e_status,
        e_isinitialised,
        e_notused1,  /* Not Currently used*/
        e_callerid,
        e_incomingcall,
        e_incall,
        e_encryption,
        e_farEndCameraInstruct,
        e_realTimeTextInstruct,
        e_farEndMotoisedInstruct,
        e_PhoneNumberEvent,
        e_URIaddressEvent,
        e_presenceSupport,
        e_duplicate,
        e_forwardCall,
        e_dhGenerate,
        e_userDefinedEvent = 1000 
        // IMPL: Event Names Here
    };

    /* Set_Value
        Set a value in the device
     */
    void Set_Value(
        int settingID,                  // setting from setting enum
        const char * value              // Value of the setting
        );

    /* Get_Value
        Get a value from the device
        @return@
        current value for the settingID
    */
    const char * Get_Value(
        int settingID  // setting from setting enum
        );

    /* Call a method with the supplied methodID
        pass parameters for the method
    */
    void Call(
        int methodID,             // ID from Method enum
        const char * p1 = "",     // parameter 1
        const char * p2 = "",     // parameter 2
        const char * p3 = "",     // parameter 3
        const char * p4 = ""      // parameter 3
        );


    /* Event
    Event callback function
    Derive this class to receive event notifications
    */
    virtual void Event(
        int evtID,                // ID from Event enum
        const char * p1 = "",     // parameter 1
        const char * p2 = "",     // parameter 2
        const char * p3 = "",     // parameter 3
        const char * p4 = ""      // parameter 4
        );

    /* Media Handling
     */
    PlusDevMedia(Audio)
    PlusDevMedia(Video)
    PlusDevMedia(Content)

    /* IsLoading
	Check to see if the underlying library is loading
	Use this to check when the library is instanced that the 
`	settings have been fully loaded from the datastore.
     */
    bool IsLoading();

protected:

};


