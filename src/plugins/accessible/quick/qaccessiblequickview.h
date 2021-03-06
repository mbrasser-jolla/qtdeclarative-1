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

#ifndef QAccessibleQuickView_H
#define QAccessibleQuickView_H

#include <QtGui/qaccessible2.h>
#include <QtGui/qaccessibleobject.h>
#include <QtQuick/qquickview.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_ACCESSIBILITY

class QAccessibleQuickView : public QAccessibleObject
{
public:
    QAccessibleQuickView(QQuickView *object);

    QAccessibleInterface *parent() const;
    QAccessibleInterface *child(int index) const;

    QAccessible::Role role() const;
    QAccessible::State state() const;
    QRect rect() const;

    int childCount() const;
    int navigate(QAccessible::RelationFlag rel, int entry, QAccessibleInterface **target) const;
    int indexOfChild(const QAccessibleInterface *iface) const;
    QString text(QAccessible::Text text) const;
    QAccessibleInterface *childAt(int x, int y) const;
private:
    QQuickView *view() const { return static_cast<QQuickView*>(object()); }
};

#endif // QT_NO_ACCESSIBILITY

QT_END_NAMESPACE

#endif // QAccessibleQuickView_H
