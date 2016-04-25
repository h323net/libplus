/*
 * mylibplus.h
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


#ifndef MYLIBPLUS_H
#define MYLIBPLUS_H

#include <libPLUSdll.h>


class MainWindow;
class mylibPLUS  : public libPLUS
{

  public:

    mylibPLUS(MainWindow * window);

    void InitialiseQT();

    // Collective Event Handler
    virtual bool HandleEvent(int id, const char * str1, const char * str2,
                                     const char * str3, const char * str4);

    // Collect individual Events
    // libDLLEvent1(status)
    // IMPL: Event Names here

    // Media Handling
    virtual bool outvideo(const void * data, int size, int width=0, int height=0);

  private:
    MainWindow * m_window;

};

#endif // MYLIBPLUS_H
