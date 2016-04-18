/*
* pluslang.h
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

#ifndef PLUSLANG_H
#define PLUSLANG_H

enum UXLang {
    uxUnknown,
    uxEN
    // More here
};

// Enumerators
enum EPStatus {
    uiUnknown,
    uiWaiting,
    uiIncomingCall,
    uiConnectingCall,
    uiWaitingForAnswer,
    uiCallInProgress,
    uiEndedByLocalUser,         ///< Local endpoint application cleared call
    uiEndedByNoAccept,          ///< Local endpoint did not accept call OnIncomingCall()=FALSE
    uiEndedByAnswerDenied,      ///< Local endpoint declined to answer call
    uiEndedByRemoteUser,        ///< Remote endpoint application cleared call
    uiEndedByRefusal,           ///< Remote endpoint refused call
    uiEndedByNoAnswer,          ///< Remote endpoint did not answer in required time
    uiEndedByCallerAbort,       ///< Remote endpoint stopped calling
    uiEndedByTransportFail,     ///< Transport error cleared call
    uiEndedByConnectFail,       ///< Transport connection failed to establish call
    uiEndedByGatekeeper,        ///< Gatekeeper has cleared call
    uiEndedByNoUser,            ///< Call failed as could not find user (in GK)
    uiEndedByNoBandwidth,       ///< Call failed as could not get enough bandwidth
    uiEndedByCapabilityExchange,///< Could not find common capabilities
    uiEndedByCallForwarded,     ///< Call was forwarded using FACILITY message
    uiEndedBySecurityDenial,    ///< Call failed a security check and was ended
    uiEndedByLocalBusy,         ///< Local endpoint busy
    uiEndedByLocalCongestion,   ///< Local endpoint congested
    uiEndedByRemoteBusy,        ///< Remote endpoint busy
    uiEndedByRemoteCongestion,  ///< Remote endpoint congested
    uiEndedByUnreachable,       ///< Could not reach the remote party
    uiEndedByNoEndPoint,        ///< The remote party is not running an endpoint
    uiEndedByHostOffline,       ///< The remote party host off line
    uiEndedByTemporaryFailure,  ///< The remote failed temporarily app may retry
    uiEndedByQ931Cause,         ///< The remote ended the call with unmapped Q.931 cause code
    uiEndedByDurationLimit,     ///< Call cleared due to an enforced duration limit
    uiEndedByInvalidConferenceID, ///< Call cleared due to invalid conference ID
    uiEndedByOSPRefusal,          ///< Call cleared as OSP server unable or unwilling to route
    uiEndedByInvalidNumberFormat, ///< Call cleared as number was invalid format
    uiEndedByUnspecifiedProtocolError, ///< Call cleared due to unspecified protocol error
    uiEndedByNoFeatureSupport         ///< Call ended due to Feature not being present.
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// Languages

std::string StatusString_EN(int status)
{
    std::string str;
    switch (status) {
        case uiWaiting:                     str = "Ready for Call";             break;
        case uiIncomingCall:                str = "Incoming Call";              break;
        case uiConnectingCall:              str = "Calling";                    break;
        case uiWaitingForAnswer:            str = "Remote Ringing";             break;
        case uiCallInProgress:              str = "Call in Progress";           break;
        case uiEndedByLocalUser:            str = "Hung Up";                    break;
        case uiEndedByNoAccept:             str = "Incoming call rejected";     break;
        case uiEndedByAnswerDenied:         str = "Incoming call refused";      break;
        case uiEndedByRemoteUser:           str = "Remote Hungup";              break;
        case uiEndedByRefusal:              str = "Call not Accepted";          break;
        case uiEndedByNoAnswer:             str = "No Answer";                  break;
        case uiEndedByCallerAbort:          str = "Stopped calling";            break;
        case uiEndedByTransportFail:        str = "Please Wait";                break;
        case uiEndedByConnectFail:          str = "Network error";              break;
        case uiEndedByGatekeeper:           str = "No Answer";                  break;
        case uiEndedByNoUser:               str = "User unknown or unavailable"; break;
        case uiEndedByNoBandwidth:          str = "Insufficient bandwidth";     break;
        case uiEndedByCapabilityExchange:   str = "No common codec";            break;
        case uiEndedByCallForwarded:        str = "Call Forwarded";             break;
        case uiEndedBySecurityDenial:       str = "Security Failure";           break;
        case uiEndedByLocalBusy:            str = "Local Busy";                 break;
        case uiEndedByLocalCongestion:      str = "Local Congestion";           break;
        case uiEndedByRemoteBusy:           str = "Line Busy";                  break;
        case uiEndedByRemoteCongestion:     str = "Network Congestion";         break;
        case uiEndedByUnreachable:          str = "User unreachable";           break;
        case uiEndedByNoEndPoint:           str = "No phone at address";        break;
        case uiEndedByHostOffline:          str = "User offline";               break;
        case uiEndedByTemporaryFailure:     str = "No Answer";                  break;
        case uiEndedByQ931Cause:            str = "Signal Error";               break;
        case uiEndedByDurationLimit:        str = "TimeLimit Reached";          break;
        case uiEndedByInvalidConferenceID:  str = "Invalid Conference";         break;
        case uiEndedByOSPRefusal:           str = "OSP Refusal";                break;
        case uiEndedByInvalidNumberFormat:  str = "Invalid Number Format";      break;
        case uiEndedByUnspecifiedProtocolError:  str = "Unspecified Error";     break;
        case uiEndedByNoFeatureSupport:     str = "No Feature Support";         break;
        default:                            str = "Unknown status";
    }

    return str;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Language Strings

std::string UXStatusString(UXLang lang, int status)
{
    switch (lang) {
        case uxEN :
        default:
            return StatusString_EN(status);
    }
}

std::string UXStatus(const std::string & lang, int status)
{
    return UXStatusString(uxEN, status);
}



#endif