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
    ui(new Ui::MainWindow)
{
    m_libPLUS = new mylibPLUS(this);
    m_libPLUS->Load();


    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    m_libPLUS->Unload();
    delete m_libPLUS;

    delete ui;
}


void MainWindow::OnVideoImage(int id, uchar * data, int width, int height)
{

    QImage img(data,width,height, QImage::Format_RGB32);
    QVideoFrame frame(img.copy());

    m_mutex.lock();
        m_frames.insert(std::pair<int,QVideoFrame>(id,frame));
    m_mutex.unlock();

}
