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

#ifndef QDECLARATIVETRANSITION_H
#define QDECLARATIVETRANSITION_H

#include "qdeclarativestate_p.h"
#include <qdeclarative.h>

#include <QtCore/qobject.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDeclarativeAbstractAnimation;
class QDeclarativeTransitionPrivate;
class QDeclarativeTransitionManager;
class QDeclarativeTransition;
class QAbstractAnimationJob;

class Q_QUICK_EXPORT QDeclarativeTransitionInstance
{
public:
    QDeclarativeTransitionInstance();
    ~QDeclarativeTransitionInstance();

    void start();
    void stop();

    bool isRunning() const;

private:
    QAbstractAnimationJob *m_anim;
    friend class QDeclarativeTransition;
};

class Q_QUICK_EXPORT QDeclarativeTransition : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeTransition)

    Q_PROPERTY(QString from READ fromState WRITE setFromState NOTIFY fromChanged)
    Q_PROPERTY(QString to READ toState WRITE setToState NOTIFY toChanged)
    Q_PROPERTY(bool reversible READ reversible WRITE setReversible NOTIFY reversibleChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeAbstractAnimation> animations READ animations)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_CLASSINFO("DefaultProperty", "animations")
    Q_CLASSINFO("DeferredPropertyNames", "animations")

public:
    QDeclarativeTransition(QObject *parent=0);
    ~QDeclarativeTransition();

    QString fromState() const;
    void setFromState(const QString &);

    QString toState() const;
    void setToState(const QString &);

    bool reversible() const;
    void setReversible(bool);

    bool enabled() const;
    void setEnabled(bool enabled);

    QDeclarativeListProperty<QDeclarativeAbstractAnimation> animations();

    QDeclarativeTransitionInstance *prepare(QDeclarativeStateOperation::ActionList &actions,
                 QList<QDeclarativeProperty> &after,
                 QDeclarativeTransitionManager *end);

    void setReversed(bool r);

Q_SIGNALS:
    void fromChanged();
    void toChanged();
    void reversibleChanged();
    void enabledChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeTransition)

QT_END_HEADER

#endif // QDECLARATIVETRANSITION_H
