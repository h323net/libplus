
/*
 * simpleDLL.h
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

#include "stdafx.h"

#include <iostream>
using namespace std;

//#include "Windows.h"


class MylibPLUS : public libPLUS
{

public:
    MylibPLUS() {}

};

int main()
{
    cout << "libPLUS WinDLL Test:" << "\n";

    cout << "Loading library.....";
    MylibPLUS x;
    x.Load();
    cout << "done.\n\n";

    cout << "SETTINGS:"         << "\n";
    cout << "version= "         << x.getversion() << "\n";
    cout << "tracing= "         << x.gettracing() << "\n";
    cout << "username= "        << x.getusername() << "\n";
    cout << "password= "        << x.getpassword() << "\n";
    cout << "server= "          << x.getserver() << "\n";
    cout << "quality= "         << x.getquality() << "\n";
    cout << "accessability= "   << x.getaccessability() << "\n";
    cout << "content= "         << x.getcontent() << "\n";
    cout << "autoanswer= "      << x.getautoanswer() << "\n";
    cout << "drvvideoplay= "    << x.getdrvvideoplay() << "\n";
    cout << "drvvideorec= "     << x.getdrvvideorec() << "\n";
    cout << "drvaudioplay= "    << x.getdrvaudioplay() << "\n";
    cout << "drvaudiorec= "     << x.getdrvaudiorec() << "\n";
    cout << "curdrvvideoplay= " << x.getcurdrvvideoplay() << "\n";
    cout << "curdrvvideorec= "  << x.getcurdrvvideorec() << "\n";
    cout << "curdrvaudioplay= " << x.getdevvideoplay() << "\n";
    cout << "curdrvaudiorec= "  << x.getdevvideorec() << "\n";
    cout << "devvideoplay= "    << x.getaudioplay() << "\n";
    cout << "devvideorec= "     << x.getdevaudiorec() << "\n";
    cout << "devaudioplay= "    << x.getaudioplay() << "\n";
    cout << "devaudiorec= "     << x.getdevaudiorec() << "\n";
    cout << "audioplay= "       << x.getaudioplay() << "\n";
    cout << "audiorec= "        << x.getaudiorec() << "\n";
    cout << "videoplay= "       << x.getvideoplay() << "\n";
    cout << "videorec= "        << x.getvideorec() << "\n";
    cout << "videoformats= "    << x.getvideoformats() << "\n";
    cout << "videoinformat= "   << x.getvideoinformat() << "\n";
    cout << "videooutformat= "  << x.getvideooutformat() << "\n";
    cout << "secondvideo= "     << x.getsecondvideo() << "\n";
    // IMPL: Setting Names Here

    return 0;
}

