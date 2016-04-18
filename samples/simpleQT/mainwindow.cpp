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

////////////////////////////////////////////////////////
// Constructor

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_libPLUS = new mylibPLUS(this);
    m_libPLUS->Load();

    // debugging
    m_libPLUS->settracing("6");

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

    m_timer->start(5);
}

MainWindow::~MainWindow()
{
    m_libPLUS->dostop();
    m_libPLUS->Unload();
    delete m_libPLUS;
    delete m_timer;
    delete ui;
}

////////////////////////////////////////////////////////
// Event handling

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
        bool success(evt.str1.toInt()==1);
        switch (evt.id) {
            case libPLUS::e_status:
                ui->lblStatus->setText(evt.str2);
                break;
            case libPLUS::e_isinitialised:
                EnableDisplay(success);
                break;
            case libPLUS::e_incall:
                SetInCall(success);
                break;
            case libPLUS::e_callerid:

            case libPLUS::e_incomingcall:

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
    return true;
}


////////////////////////////////////////////////////////
// UI functions

void MainWindow::EnableDisplay(bool toEnable)
{
    if (toEnable) {
        ui->btnStart->setText("Stop");

    } else {
        ui->btnStart->setText("Start");
        ui->lblStatus->setText("");
    }

    ui->btnCall->setEnabled(toEnable);
    ui->lblNumber->setEnabled(toEnable);
}

void MainWindow::SetInCall(bool inCall)
{
    if (inCall) {
        ui->btnCall->setText("HangUp");
        ui->lblNumber->setEnabled(false);
        ui->btnCall->setChecked(true);
        ui->btnStart->setEnabled(false);
    } else {
        ui->btnCall->setText("Call");
        ui->btnCall->setChecked(false);
        ui->lblNumber->setEnabled(true);
        ui->lblNumber->setText("");
        ui->btnStart->setEnabled(true);
    }
}

////////////////////////////////////////////////////////
// UI Events

void MainWindow::on_btnStart_clicked(bool checked)
{
    if (checked) {
        m_libPLUS->dostart();
    } else {
        m_libPLUS->dostop();
    }
}

void MainWindow::on_btnCall_clicked(bool checked)
{
    QString number = ui->lblNumber->text();
    if (checked && (number.size() >0)) {
        m_libPLUS->doplaceCall(number.toStdString().c_str());
    } else {
        m_libPLUS->dohangupCall();
    }
}
