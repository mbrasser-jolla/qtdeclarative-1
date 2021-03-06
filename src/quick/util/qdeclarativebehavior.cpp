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

#include "qdeclarativebehavior_p.h"

#include "qdeclarativeanimation_p.h"
#include <qdeclarativecontext.h>
#include <qdeclarativeinfo.h>
#include <private/qdeclarativeproperty_p.h>
#include <private/qdeclarativeguard_p.h>
#include <private/qdeclarativeengine_p.h>
#include <private/qabstractanimationjob_p.h>
#include <private/qdeclarativetransition_p.h>

#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE

class QDeclarativeBehaviorPrivate : public QObjectPrivate, public QAnimation2ChangeListener
{
    Q_DECLARE_PUBLIC(QDeclarativeBehavior)
public:
    QDeclarativeBehaviorPrivate() : animation(0), animationInstance(0), enabled(true), finalized(false)
      , blockRunningChanged(false) {}

    virtual void animationStateChanged(QAbstractAnimationJob *, QAbstractAnimationJob::State newState, QAbstractAnimationJob::State oldState);

    QDeclarativeProperty property;
    QVariant targetValue;
    QDeclarativeGuard<QDeclarativeAbstractAnimation> animation;
    QAbstractAnimationJob *animationInstance;
    bool enabled;
    bool finalized;
    bool blockRunningChanged;
};

/*!
    \qmlclass Behavior QDeclarativeBehavior
    \inqmlmodule QtQuick 2
    \ingroup qml-animation-transition
    \brief The Behavior element allows you to specify a default animation for a property change.

    A Behavior defines the default animation to be applied whenever a
    particular property value changes.

    For example, the following Behavior defines a NumberAnimation to be run
    whenever the \l Rectangle's \c width value changes. When the MouseArea
    is clicked, the \c width is changed, triggering the behavior's animation:

    \snippet doc/src/snippets/declarative/behavior.qml 0

    Note that a property cannot have more than one assigned Behavior. To provide
    multiple animations within a Behavior, use ParallelAnimation or
    SequentialAnimation.

    If a \l{QML States}{state change} has a \l Transition that matches the same property as a
    Behavior, the \l Transition animation overrides the Behavior for that
    state change. For general advice on using Behaviors to animate state changes, see
    \l{Using QML Behaviors with States}.

    \sa {QML Animation and Transitions}, {declarative/animation/behaviors}{Behavior example}, QtDeclarative
*/


QDeclarativeBehavior::QDeclarativeBehavior(QObject *parent)
    : QObject(*(new QDeclarativeBehaviorPrivate), parent)
{
}

QDeclarativeBehavior::~QDeclarativeBehavior()
{
    Q_D(QDeclarativeBehavior);
    delete d->animationInstance;
}

/*!
    \qmlproperty Animation QtQuick2::Behavior::animation
    \default

    This property holds the animation to run when the behavior is triggered.
*/

QDeclarativeAbstractAnimation *QDeclarativeBehavior::animation()
{
    Q_D(QDeclarativeBehavior);
    return d->animation;
}

void QDeclarativeBehavior::setAnimation(QDeclarativeAbstractAnimation *animation)
{
    Q_D(QDeclarativeBehavior);
    if (d->animation) {
        qmlInfo(this) << tr("Cannot change the animation assigned to a Behavior.");
        return;
    }

    d->animation = animation;
    if (d->animation) {
        d->animation->setDefaultTarget(d->property);
        d->animation->setDisableUserControl();
    }
}


void QDeclarativeBehaviorPrivate::animationStateChanged(QAbstractAnimationJob *, QAbstractAnimationJob::State newState,QAbstractAnimationJob::State)
{
    if (!blockRunningChanged)
        animation->notifyRunningChanged(newState == QAbstractAnimationJob::Running);
}

/*!
    \qmlproperty bool QtQuick2::Behavior::enabled

    This property holds whether the behavior will be triggered when the tracked
    property changes value.

    By default a Behavior is enabled.
*/

bool QDeclarativeBehavior::enabled() const
{
    Q_D(const QDeclarativeBehavior);
    return d->enabled;
}

void QDeclarativeBehavior::setEnabled(bool enabled)
{
    Q_D(QDeclarativeBehavior);
    if (d->enabled == enabled)
        return;
    d->enabled = enabled;
    emit enabledChanged();
}

void QDeclarativeBehavior::write(const QVariant &value)
{
    Q_D(QDeclarativeBehavior);
    bool bypass = !d->enabled || !d->finalized;
    if (!bypass)
        qmlExecuteDeferred(this);
    if (!d->animation || bypass) {
        QDeclarativePropertyPrivate::write(d->property, value, QDeclarativePropertyPrivate::BypassInterceptor | QDeclarativePropertyPrivate::DontRemoveBinding);
        d->targetValue = value;
        return;
    }

    if (d->animation->isRunning() && value == d->targetValue)
        return;

    const QVariant &currentValue = d->property.read();
    d->targetValue = value;

    if (d->animationInstance && d->animationInstance->duration() != -1
            && !d->animationInstance->isStopped()) {
        d->blockRunningChanged = true;
        d->animationInstance->stop();
    }

    QDeclarativeStateOperation::ActionList actions;
    QDeclarativeAction action;
    action.property = d->property;
    action.fromValue = currentValue;
    action.toValue = value;
    actions << action;

    QList<QDeclarativeProperty> after;
    QAbstractAnimationJob *prev = d->animationInstance;
    d->animationInstance = d->animation->transition(actions, after, QDeclarativeAbstractAnimation::Forward);
    if (d->animationInstance != prev) {
        d->animationInstance->addAnimationChangeListener(d, QAbstractAnimationJob::StateChange);
        if (prev)
            delete prev;
    }
    d->animationInstance->start();
    d->blockRunningChanged = false;
    if (!after.contains(d->property))
        QDeclarativePropertyPrivate::write(d->property, value, QDeclarativePropertyPrivate::BypassInterceptor | QDeclarativePropertyPrivate::DontRemoveBinding);
}

void QDeclarativeBehavior::setTarget(const QDeclarativeProperty &property)
{
    Q_D(QDeclarativeBehavior);
    d->property = property;
    if (d->animation)
        d->animation->setDefaultTarget(property);

    QDeclarativeEnginePrivate *engPriv = QDeclarativeEnginePrivate::get(qmlEngine(this));
    static int finalizedIdx = -1;
    if (finalizedIdx < 0)
        finalizedIdx = metaObject()->indexOfSlot("componentFinalized()");
    engPriv->registerFinalizeCallback(this, finalizedIdx);
}

void QDeclarativeBehavior::componentFinalized()
{
    Q_D(QDeclarativeBehavior);
    d->finalized = true;
}

QT_END_NAMESPACE
