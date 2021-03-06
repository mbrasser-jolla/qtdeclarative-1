/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the Declarative module of the Qt Toolkit.
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

#include "qquickparticleextruder_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmlclass Shape QQuickParticleExtruder
    \inqmlmodule QtQuick.Particles 2
    \brief The Shape element allows you to specify an area for affectors and emitter.

    The base class is just a rectangle.
*/

QQuickParticleExtruder::QQuickParticleExtruder(QObject *parent) :
    QObject(parent)
{
}

QPointF QQuickParticleExtruder::extrude(const QRectF &rect)
{
    return QPointF(((qreal)rand() / RAND_MAX) * rect.width() + rect.x(),
                   ((qreal)rand() / RAND_MAX) * rect.height() + rect.y());
}

bool QQuickParticleExtruder::contains(const QRectF &bounds, const QPointF &point)
{
    return bounds.contains(point);
}

QT_END_NAMESPACE
