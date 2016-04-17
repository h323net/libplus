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

    libDLLEvent1(status)
    // IMPL: Event Names here

  private:
    MainWindow * m_window;

};

#endif // MYLIBPLUS_H
