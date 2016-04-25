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
    ui(new Ui::MainWindow), m_videoRunning(false), m_loading(false)
{
    ui->setupUi(this);

    m_libPLUS = new mylibPLUS(this);
    m_libPLUS->InitialiseQT();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

    m_timer->start(15);
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

    m_events.enqueue(evt);
}


void MainWindow::OnLibPlusImage(int id, uchar * data, int width, int height)
{
    PlusImage img;
    img.id = id;
    img.frame = QVideoFrame(QImage(data,width,height, QImage::Format_RGB32));

    m_frames.enqueue(img);
}


void MainWindow::update()
{
    ProcessEvents();
    ProcessFrames();
    QMainWindow::update();
}

bool MainWindow::ProcessEvents()
{
    PlusEvent evt;
    bool success;
    while (!m_events.isEmpty()) {
        evt = m_events.head();
        m_events.dequeue();

        success = (evt.str1.toInt()==1);
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
    }
    return true;
}


bool MainWindow::ProcessFrames()
{
    int id;
    QVideoFrame frame;
    while (!m_frames.isEmpty()) {
        if (!m_videoRunning) {
            m_frames.dequeue();
            continue;
        }

        id = m_frames.head().id;
        frame = m_frames.head().frame;
        m_frames.dequeue();

        switch (id) {
            case 1:
                ui->video1->RenderFrame(frame);
                break;
            default:
                break;
        }
    }
    return true;
}


////////////////////////////////////////////////////////
// UI functions

void MainWindow::LoadDisplay(bool toEnable)
{
    m_loading = true;

    ui->btnCall->setEnabled(toEnable);
    ui->lblNumber->setEnabled(toEnable);
    ui->lbFmt->setEnabled(toEnable);
    ui->lbVideoPlay->setEnabled(toEnable);

    if (toEnable) {
        // devices
        QString dev1(m_libPLUS->getdrvvideoplay());
        QStringList devlist1 = dev1.split(';');

        ui->lbVideoPlay->clear();
        for (int i=0; i < devlist1.size(); ++i)
            ui->lbVideoPlay->addItem(devlist1[i]);

        ui->lbVideoPlay->setCurrentText(QString(m_libPLUS->getvideoplay()));


        // formats
        QString dev2(m_libPLUS->getvideoformats());
        QStringList devlist2 = dev2.split(';');

        ui->lbFmt->clear();
        for (int i=0; i < devlist2.size(); ++i)
            ui->lbFmt->addItem(devlist2[i]);

        ui->lbFmt->setCurrentText(QString(m_libPLUS->getvideooutformat()));
    } else {

        ui->lblStatus->clear();
        ui->lblNumber->clear();
        ui->lbFmt->clear();
        ui->lbVideoPlay->clear();

    }

    m_loading = false;
}

void MainWindow::EnableDisplay(bool toEnable)
{
    if (toEnable)
        ui->btnStart->setText("Stop");
    else
        ui->btnStart->setText("Start");

    LoadDisplay(toEnable);
}

void MainWindow::SetInCall(bool inCall)
{
    if (inCall) {
        ui->btnCall->setText("HangUp");
        ui->lblNumber->setEnabled(false);
        ui->btnCall->setChecked(true);
        ui->btnStart->setEnabled(false);
        m_videoRunning = true;
    } else {
        ui->btnCall->setText("Call");
        ui->btnCall->setChecked(false);
        ui->lblNumber->setEnabled(true);
        ui->lblNumber->setText("");
        ui->btnStart->setEnabled(true);
        m_videoRunning = false;
        ui->video1->StopSurface();
    }
}

///////////////////////////////////////////////////////////
//  Default Settings

void MainWindow::LoadDefaultSettings()
{

    m_libPLUS->setusername("testuser");
    m_libPLUS->setpassword("");
    m_libPLUS->setserver("");

}

////////////////////////////////////////////////////////
// UI Events

void MainWindow::on_btnStart_clicked(bool checked)
{
    if (checked) {
        LoadDefaultSettings();
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

void MainWindow::on_lbVideoPlay_currentIndexChanged(const QString &arg1)
{
    if (!m_loading)
        m_libPLUS->setcurdrvvideoplay(arg1.toStdString().c_str());
}

void MainWindow::on_lbFmt_currentIndexChanged(const QString &arg1)
{
    if (!m_loading) {
        m_libPLUS->setvideooutformat(arg1.toStdString().c_str());
    }
}
