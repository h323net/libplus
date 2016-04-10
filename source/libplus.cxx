/*
* libplus.cxx
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
#include "libplus.h"

////////////////////////////////////////////////////////////////////////////
// PlusProcess Instance

PlusProcess * l_vProcess = NULL;

////////////////////////////////////////////////////////////////////////////
// Main Wrapper Class

PLUSdevice::PLUSdevice()
{
    l_vProcess = new PlusProcess(this);
}

PLUSdevice::~PLUSdevice()
{
    delete l_vProcess;
    l_vProcess = NULL;
}

bool PLUSdevice::IsLoading()
{
    return l_vProcess->IsLoading();
}

void PLUSdevice::Set_Value(int settingID, const char * value)
{
    if (!l_vProcess)
        return;

    l_vProcess->SetSetting((PlusProcess::Setting)settingID, value);
}

const char * PLUSdevice::Get_Value(int settingID)
{
    if (!l_vProcess)
        return (const char *)PString();

   return (const char *)l_vProcess->GetSetting((PlusProcess::Setting)settingID);
}

void PLUSdevice::Call(int methodID, const char * p1, const char * p2, const char * p3)
{
    if (!l_vProcess)
        return;

    l_vProcess->DoMethod((PlusProcess::Method)methodID,p1,p2,p3);
}

void PLUSdevice::Event(int evtID, const char * p1, const  char * p2, const char * p3, const char * p4)
{
    
}