/*
 * videowidgetsurface.h
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

#ifndef VIDEOWIDGETSURFACE_H
#define VIDEOWIDGETSURFACE_H

#include <QRect>
#include <QImage>
#include <QAbstractVideoSurface>
#include <QVideoFrame>
#include <QWidget>

class VideoWidgetSurface : public QAbstractVideoSurface
{
    Q_OBJECT
 public:
     VideoWidgetSurface(QWidget *widget, QObject *parent = 0);

     QList<QVideoFrame::PixelFormat> supportedPixelFormats(
             QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
     bool isFormatSupported(const QVideoSurfaceFormat &format, QVideoSurfaceFormat *similar) const;

     bool start(const QVideoSurfaceFormat &format);
     void stop();

     bool present(const QVideoFrame &frame);

     QRect videoRect() const { return targetRect; }
     void updateVideoRect();

     void paint(QPainter *painter);

 private:
     QWidget *widget;
     QImage::Format imageFormat;
     QRect targetRect;
     QSize imageSize;
     QRect sourceRect;
     QVideoFrame currentFrame;

signals:

public slots:
};

#endif // VIDEOWIDGETSURFACE_H
