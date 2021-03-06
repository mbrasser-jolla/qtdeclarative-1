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

#include "private/qdeclarativelayoutitem_p.h"

#include <QDebug>

#include <limits.h>

QT_BEGIN_NAMESPACE



/*!
    \qmlclass LayoutItem QDeclarative1LayoutItem
    \inqmlmodule QtQuick 1
    \ingroup qml-utility-elements
    \since QtQuick 1.0
    \brief The LayoutItem element allows declarative UI elements to be placed inside Qt's Graphics View layouts.

    LayoutItem is a variant of \l Item with additional size hint properties. These properties provide the size hints
    necessary for items to work in conjunction with Qt \l{Graphics View Framework}{Graphics View} layout classes
    such as QGraphicsLinearLayout and QGraphicsGridLayout. The Qt layout mechanisms will resize the LayoutItem as appropriate,
    taking its size hints into account, and you can propagate this to the other elements in your UI via anchors and bindings.

    This is a QGraphicsLayoutItem subclass, and its properties merely expose the QGraphicsLayoutItem functionality to QML.

    The \l{declarative/cppextensions/qgraphicslayouts/layoutitem}{LayoutItem example}
    demonstrates how a LayoutItem can be used within a \l{Graphics View Framework}{Graphics View}
    scene.
*/

/*!
    \qmlproperty QSizeF QtQuick1::LayoutItem::maximumSize

    The maximumSize property can be set to specify the maximum desired size of this LayoutItem
*/

/*!
    \qmlproperty QSizeF QtQuick1::LayoutItem::minimumSize

    The minimumSize property can be set to specify the minimum desired size of this LayoutItem
*/

/*!
    \qmlproperty QSizeF QtQuick1::LayoutItem::preferredSize

    The preferredSize property can be set to specify the preferred size of this LayoutItem
*/

QDeclarative1LayoutItem::QDeclarative1LayoutItem(QDeclarativeItem* parent)
    : QDeclarativeItem(parent), m_maximumSize(INT_MAX,INT_MAX), m_minimumSize(0,0), m_preferredSize(0,0)
{
    setGraphicsItem(this);
}

void QDeclarative1LayoutItem::setGeometry(const QRectF & rect)
{
    setX(rect.x());
    setY(rect.y());
    setWidth(rect.width());
    setHeight(rect.height());
}

QSizeF QDeclarative1LayoutItem::sizeHint(Qt::SizeHint w, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);
    if(w == Qt::MinimumSize){
        return m_minimumSize;
    }else if(w == Qt::MaximumSize){
        return m_maximumSize;
    }else{
        return m_preferredSize;
    }
}



QT_END_NAMESPACE
