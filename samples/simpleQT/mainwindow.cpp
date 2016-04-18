/*
 * mainwindow.cpp
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


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mylibplus.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_shutDown(false)
{
    ui->setupUi(this);

    m_libPLUS = new mylibPLUS(this);
    m_libPLUS->Load();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(5);

    m_libPLUS->dostart();
}

MainWindow::~MainWindow()
{
    m_libPLUS->dostop();
    m_libPLUS->Unload();
    delete m_libPLUS;

    delete ui;
}


void MainWindow::OnLibPlusEvent(int id, QString str1, QString str2, QString str3, QString str4)
{
    PlusEvent evt;
        evt.id = id;
        evt.str1 = str1;
        evt.str2 = str2;
        evt.str3 = str3;
        evt.str4 = str4;

    m_mutEvents.lock();
    m_events.push(evt);
    m_mutEvents.unlock();
}


void MainWindow::OnLibPlusImage(int id, uchar * data, int width, int height)
{
    PlusImage img;
    img.id = id;
    img.frame = QVideoFrame(QImage(data,width,height, QImage::Format_RGB32));

    m_mutFrames.lock();
    m_frames.push(img);
    m_mutFrames.unlock();
}


void MainWindow::update()
{
    ProcessEvents();
    ProcessFrames();
    QMainWindow::update();
}


bool MainWindow::ProcessEvents()
{
    while (m_events.size() > 0) {
        PlusEvent & evt = m_events.front();
        switch (evt.id) {
            case libPLUS::e_status:
               ui->txtStatus->addItem(evt.str1 + QString(" ") + evt.str2);
               break;
            default:
               break;
        }
        m_mutEvents.lock();
        m_events.pop();
        m_mutEvents.unlock();
    }
    return true;
}


bool MainWindow::ProcessFrames()
{
      if (m_shutDown)
          return false;

      while (m_frames.size() > 0) {
        PlusImage & img = m_frames.front();
        switch (img.id) {
            case 1:
                ui->video1->RenderFrame(img.frame);
                break;
            default:
                break;
        }
        m_mutFrames.lock();
        m_frames.pop();
        m_mutFrames.lock();
    }
    return !m_shutDown;
}
