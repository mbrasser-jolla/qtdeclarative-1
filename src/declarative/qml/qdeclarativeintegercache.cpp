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

#include "qdeclarativeintegercache_p.h"

QT_BEGIN_NAMESPACE

QDeclarativeIntegerCache::QDeclarativeIntegerCache()
{
}

QDeclarativeIntegerCache::~QDeclarativeIntegerCache()
{
}

QString QDeclarativeIntegerCache::findId(int value) const
{
    for (StringCache::ConstIterator iter = stringCache.begin();
            iter != stringCache.end(); ++iter) {
        if (iter.value() == value)
            return iter.key();
    }
    return QString();
}

void QDeclarativeIntegerCache::reserve(int size)
{ 
    stringCache.reserve(size);
}

void QDeclarativeIntegerCache::add(const QString &id, int value)
{
    Q_ASSERT(!stringCache.contains(id));

    stringCache.insert(id, value);
}

int QDeclarativeIntegerCache::value(const QString &id)
{
    int *rv = stringCache.value(id);
    return rv?*rv:-1;
}

QT_END_NAMESPACE
