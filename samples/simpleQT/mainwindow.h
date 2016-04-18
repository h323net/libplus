/*
 * maimwindow.h
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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>
#include <map>
#include <queue>
#include <QTimer>
#include <QMutex>
#include <QVideoFrame>

#include <QDebug>

namespace Ui {
class MainWindow;
}

class mylibPLUS;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Event Processing
    bool ProcessEvents();
    bool ProcessFrames();

    struct PlusEvent {
        int id;
        QString str1;
        QString str2;
        QString str3;
        QString str4;
    };
    void OnLibPlusEvent(int id, QString str1="", QString str2="", QString str3="", QString str4="");


    struct PlusImage {
        int         id;
        QVideoFrame frame;
    };
    void OnLibPlusImage(int id, uchar * data, int width, int height);


public slots:
    /**
     * @brief update
     * Fires every x milliseconds to process messages from libPLUS
     */
    void update();

private slots:
    void on_btnStart_clicked(bool checked);

    void on_btnCall_clicked(bool checked);

protected:
    void EnableDisplay(bool toEnable);
    void SetInCall(bool inCall);

private:
    Ui::MainWindow *ui;

    mylibPLUS * m_libPLUS;

    // Manage Events
    QTimer *                  m_timer;       ///< event timer
    std::queue<PlusEvent>     m_events;      ///< libPlus Events
    QMutex                    m_mutEvents;
    std::queue<PlusImage>     m_frames;      ///< libPlus Video Images
    QMutex                    m_mutFrames;

};

#endif // MAINWINDOW_H
