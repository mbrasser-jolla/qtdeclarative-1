// Commit: ebd4bc73c46c2962742a682b6a391fb68c482aec
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

#ifndef QQUICKREPEATER_P_H
#define QQUICKREPEATER_P_H

#include "qquickitem.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDeclarativeChangeSet;

class QQuickRepeaterPrivate;
class Q_AUTOTEST_EXPORT QQuickRepeater : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QVariant model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QDeclarativeComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_CLASSINFO("DefaultProperty", "delegate")

public:
    QQuickRepeater(QQuickItem *parent=0);
    virtual ~QQuickRepeater();

    QVariant model() const;
    void setModel(const QVariant &);

    QDeclarativeComponent *delegate() const;
    void setDelegate(QDeclarativeComponent *);

    int count() const;

    Q_INVOKABLE QQuickItem *itemAt(int index) const;

Q_SIGNALS:
    void modelChanged();
    void delegateChanged();
    void countChanged();

    void itemAdded(int index, QQuickItem *item);
    void itemRemoved(int index, QQuickItem *item);

private:
    void clear();
    void regenerate();

protected:
    virtual void componentComplete();
    void itemChange(ItemChange change, const ItemChangeData &value);

private Q_SLOTS:
    void createdItem(int index, QQuickItem *item);
    void initItem(int, QQuickItem *item);
    void modelUpdated(const QDeclarativeChangeSet &changeSet, bool reset);

private:
    Q_DISABLE_COPY(QQuickRepeater)
    Q_DECLARE_PRIVATE(QQuickRepeater)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickRepeater)

QT_END_HEADER

#endif // QQUICKREPEATER_P_H
