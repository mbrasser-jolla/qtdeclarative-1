/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEANIMATEDIMAGE_H
#define QDECLARATIVEANIMATEDIMAGE_H

#include "private/qdeclarativeimage_p.h"

#ifndef QT_NO_MOVIE

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QMovie;


class QDeclarative1AnimatedImagePrivate;

class Q_AUTOTEST_EXPORT QDeclarative1AnimatedImage : public QDeclarative1Image
{
    Q_OBJECT

    Q_PROPERTY(bool playing READ isPlaying WRITE setPlaying NOTIFY playingChanged)
    Q_PROPERTY(bool paused READ isPaused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(int currentFrame READ currentFrame WRITE setCurrentFrame NOTIFY frameChanged)
    Q_PROPERTY(int frameCount READ frameCount)

    // read-only for AnimatedImage
    Q_PROPERTY(QSize sourceSize READ sourceSize NOTIFY sourceSizeChanged)

public:
    QDeclarative1AnimatedImage(QDeclarativeItem *parent=0);
    ~QDeclarative1AnimatedImage();

    bool isPlaying() const;
    void setPlaying(bool play);

    bool isPaused() const;
    void setPaused(bool pause);

    int currentFrame() const;
    void setCurrentFrame(int frame);

    int frameCount() const;

    // Extends QDeclarative1Image's src property*/
    virtual void setSource(const QUrl&);

Q_SIGNALS:
    void playingChanged();
    void pausedChanged();
    void frameChanged();
    void sourceSizeChanged();

private Q_SLOTS:
    void movieUpdate();
    void movieRequestFinished();
    void playingStatusChanged();

protected:
    virtual void load();
    void componentComplete();

private:
    Q_DISABLE_COPY(QDeclarative1AnimatedImage)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarative1AnimatedImage)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarative1AnimatedImage)

QT_END_HEADER

#endif // QT_NO_MOVIE

#endif
