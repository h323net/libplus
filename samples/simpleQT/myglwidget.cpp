/*
 * myqtwidget.cpp
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


#include "myglwidget.h"

#include <QPainter>
#include <QColor>

#include <QVideoSurfaceFormat>
#include <QPaintEvent>

#include "mainwindow.h"


MyGLWidget::MyGLWidget(QWidget *parent)
: QWidget(parent), m_caption("Video")
, m_surface(0)
{
    setAutoFillBackground(false);
    setAttribute(Qt::WA_NoSystemBackground, true);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::darkGray);
    setPalette(Pal);

    m_surface = new VideoWidgetSurface(this);
}


MyGLWidget::~MyGLWidget()
{
    delete m_surface;
}


void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


void MyGLWidget::SetCaption(QString caption)
{
    m_caption = caption;
    update();
}


bool MyGLWidget::RenderFrame(const QVideoFrame & frame)
{
    if (!frame.isValid())
        return false;

    QVideoSurfaceFormat currentFormat = m_surface->surfaceFormat();

    if (frame.pixelFormat() != currentFormat.pixelFormat()
            || frame.size() != currentFormat.frameSize()) {
        QVideoSurfaceFormat format(frame.size(), frame.pixelFormat());

        if (!m_surface->start(format))
            return false;
    }

    if (!m_surface->present(frame)) {
        m_surface->stop();

        return false;
    } else {
        return true;
    }
}

void MyGLWidget::StopSurface() {
    m_surface->stop();
    this->repaint();
}


void MyGLWidget::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    if (m_surface->isActive()) {
        const QRect videoRect = m_surface->videoRect();

        if (!videoRect.contains(event->rect())) {
            QRegion region = event->region();
            region.subtracted(videoRect);

            QBrush brush = palette().background();

            foreach (const QRect &rect, region.rects())
                painter.fillRect(rect, brush);
        }

        m_surface->paint(&painter);
    } else
    {
        painter.fillRect(event->rect(), palette().background());
        painter.setPen(Qt::yellow);
        painter.setFont(QFont("Arial", 24));
        painter.drawText(rect(), Qt::AlignCenter, m_caption);
        painter.end();
    }
}


void MyGLWidget::resizeEvent(QResizeEvent * event)
{
     QWidget::resizeEvent(event);
     m_surface->updateVideoRect();
}


QSize MyGLWidget::sizeHint() const
{
    return m_surface->surfaceFormat().sizeHint();
}

