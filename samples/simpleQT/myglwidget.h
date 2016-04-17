/*
 * myqtwidget.h
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


#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "videowidgetsurface.h"
#include <QOpenGLFunctions>

class MyGLWidget : public QWidget, protected QOpenGLFunctions
{
public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

    void SetCaption(QString caption);

    bool RenderFrame(const QVideoFrame & frame);

    void StopSurface();

    QAbstractVideoSurface *videoSurface() const { return m_surface; }

    QSize sizeHint() const;


protected:
    void initializeGL();
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

private:
    QString              m_caption;
    VideoWidgetSurface * m_surface;

signals:

public slots:

};






#endif // MYGLWIDGET_H
