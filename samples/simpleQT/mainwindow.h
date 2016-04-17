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

#include <map>
#include <QMutex>
#include <QVideoFrame>

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


    void OnVideoImage(int id, uchar * data, int width, int height);

private:
    Ui::MainWindow *ui;

    mylibPLUS * m_libPLUS;

    QMutex                    m_mutex;
    std::map<int,QVideoFrame> m_frames;
};

#endif // MAINWINDOW_H