/*
 * mylibplus.cpp
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

#include "mylibplus.h"
#include "mainwindow.h"

#define mylibDLLEvtBody(name) \
void mylibPLUS::on##name() {}

#define mylibDLLEvtBody1(name) \
void mylibPLUS::on##name(const char * /*str1*/) {}

mylibPLUS::mylibPLUS(MainWindow * window)
: m_window(window)
{

}

bool mylibPLUS::HandleEvent(int id, const char * str1, const char * str2,
                                    const char * str3, const char * str4)
{
    qDebug() << "Event Rec'd " << id << " " << QString(str1) << " " << QString(str2);

    switch (id) {
     case e_status:
     case e_isinitialised:
     case e_callerid:
     case e_incomingcall:
     case e_incall:
        m_window->OnLibPlusEvent(id,str1, str2, str3, str4);
        break;
     default:
        return true;
    }

    return true;
}

bool mylibPLUS::outvideo(const void * /*data*/, int size, int width, int height)
{
   qDebug() << "Video Rec'd " << size << " w:" << width << " h:" << height;

   return true;
}

// mylibDLLEvtBody1(status)
// IMPL: Event Names here


