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

#ifndef QSGCONTEXTPLUGIN_H
#define QSGCONTEXTPLUGIN_H

#include <QtQuick/qtquickglobal.h>
#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>

#include <QDeclarativeImageProvider>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QSGContext;

struct Q_QUICK_EXPORT QSGContextFactoryInterface : public QFactoryInterface
{
    virtual QSGContext *create(const QString &key) const = 0;

    virtual QDeclarativeTextureFactory *createTextureFactoryFromImage(const QImage &image) = 0;
};

#define QSGContextFactoryInterface_iid \
        "com.trolltech.Qt.QSGContextFactoryInterface"
Q_DECLARE_INTERFACE(QSGContextFactoryInterface, QSGContextFactoryInterface_iid)

class Q_QUICK_EXPORT QSGContextPlugin : public QObject, public QSGContextFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QSGContextFactoryInterface:QFactoryInterface)
public:
    explicit QSGContextPlugin(QObject *parent = 0);
    virtual ~QSGContextPlugin();

    virtual QStringList keys() const = 0;
    virtual QSGContext *create(const QString &key) const = 0;

    virtual QDeclarativeTextureFactory *createTextureFactoryFromImage(const QImage &image) { return 0; }
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QSGCONTEXTPLUGIN_H
