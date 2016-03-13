/*
* libplus_jni.h
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

class JNI_PLUSdevice : public PLUSdevice
{
    public:
        JNI_PLUSdevice(JNIEnv* env, jobject obj);
        virtual ~JNI_PLUSdevice();

    protected:

        // events callback
        virtual void Event(
            int evtID,                // ID from Event enum
            const char * p1 = "",     // parameter 1
            const char * p2 = "",     // parameter 2
            const char * p3 = "",     // parameter 3
            const char * p4 = ""      // parameter 4
            );

    private:
        JNIEnv *                 m_env;
        jobject                  m_obj;
        std::map<int, jmethodID> m_eventMap;

};



