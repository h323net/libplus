
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
#include <string>
using namespace std;


class MylibPLUS : public libPLUS
{

public:
    MylibPLUS() {}

    // To collect all events override this function and return true.
    virtual bool HandleEvent(int id, const char * str1, const char * str2, const char * str3, const char * str4)
    {
        cout << "Evt " << id << " " << str1 << " " << str2 << " " << str3 << " " << str4 << endl;
        return true;  // Must return true
    }

    virtual bool outvideo(const void * data, int size, int width = 0, int height = 0)
    {
        //cout << "frame " << size << " " << width << " " << height << endl;
        return true; // Must return true
    }

};

int main()
{
    cout << "libPLUS WinDLL Test:" << endl;

    cout << "Loading library.....";
    MylibPLUS x;
    x.Load();
    cout << "done.\n\n";

    // implementers custom settings here
    x.settracing("6");
    x.setcurdrvvideoplay("External");
    x.setvideooutformat("BGR32");
    x.dovideosize(std::to_string(libPLUS::videoOut).c_str(), "352", "288");

    cout << "SETTINGS:"         << endl;
    cout << "version= "         << x.getversion() << endl;
    cout << "tracing= "         << x.gettracing() << endl;
    cout << "username= "        << x.getusername() << endl;
    cout << "password= "        << x.getpassword() << endl;
    cout << "server= "          << x.getserver() << endl;
    cout << "quality= "         << x.getquality() << endl;
    cout << "accessability= "   << x.getaccessability() << endl;
    cout << "content= "         << x.getcontent() << endl;
    cout << "autoanswer= "      << x.getautoanswer() << endl;
    cout << "drvvideoplay= "    << x.getdrvvideoplay() << endl;
    cout << "drvvideorec= "     << x.getdrvvideorec() << endl;
    cout << "drvaudioplay= "    << x.getdrvaudioplay() << endl;
    cout << "drvaudiorec= "     << x.getdrvaudiorec() << endl;
    cout << "curdrvvideoplay= " << x.getcurdrvvideoplay() << endl;
    cout << "curdrvvideorec= "  << x.getcurdrvvideorec() << endl;
    cout << "curdrvaudioplay= " << x.getdevvideoplay() << endl;
    cout << "curdrvaudiorec= "  << x.getdevvideorec() << endl;
    cout << "devvideoplay= "    << x.getaudioplay() << endl;
    cout << "devvideorec= "     << x.getdevaudiorec() << endl;
    cout << "devaudioplay= "    << x.getaudioplay() << endl;
    cout << "devaudiorec= "     << x.getdevaudiorec() << endl;
    cout << "audioplay= "       << x.getaudioplay() << endl;
    cout << "audiorec= "        << x.getaudiorec() << endl;
    cout << "videoplay= "       << x.getvideoplay() << endl;
    cout << "videorec= "        << x.getvideorec() << endl;
    cout << "videoformats= "    << x.getvideoformats() << endl;
    cout << "videoinformat= "   << x.getvideoinformat() << endl;
    cout << "videooutformat= "  << x.getvideooutformat() << endl;
    cout << "secondvideo= "     << x.getsecondvideo() << endl;
    // IMPL: Setting Names Here

    // Start the endpoint
    x.dostart();
    cout << endl << "Press X to exit." << endl;

    // Simplest possible user interface
    std::string input;
    for (;;) {
        cout << "H323> " << flush;
        getline(cin, input);

        string instruct = input.substr(0, 1);
        if (!instruct.compare("c") && (input.size() > 3)) {
            string number = input.substr(2, input.size() - 2);
            x.doplaceCall(number.c_str());
        }

        if (!instruct.compare("h"))
            x.dohangupCall();


        if (!instruct.compare("x"))
            break;
  
    }

    x.dostop();
    x.Unload();

    return 0;
}

