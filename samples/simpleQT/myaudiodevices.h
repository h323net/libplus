/*
 * myaudiodevices.h
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


#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QAudioOutput>

class AudioDevice : public QObject {
    Q_OBJECT

public:

    // Initialize the sound card
    AudioDevice(bool player);
    virtual ~AudioDevice();

    void Close();

private:

    QAudioInput  * m_audioIn;
    QAudioOutput * m_audioOut;

    bool           m_isPlayer;
};


#endif // AUDIODEVICE_H
