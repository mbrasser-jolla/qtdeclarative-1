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

#include "qquickspritegoal_p.h"
#include <private/qquickspriteengine_p.h>
#include <private/qquicksprite_p.h>
#include "qquickimageparticle_p.h"
#include <QDebug>

QT_BEGIN_NAMESPACE

/*!
    \qmlclass SpriteGoal QQuickSpriteGoalAffector
    \inqmlmodule QtQuick.Particles 2
    \inherits Affector
    \brief The SpriteGoal Affector allows you to change the state of a sprite particle.

*/
/*!
    \qmlproperty string QtQuick.Particles2::SpriteGoal::goalState

    The name of the Sprite which the affected particles should move to.

    Sprite states have defined durations and transitions between them, setting goalState
    will cause it to disregard any path weightings (including 0) and head down the path
    which will reach the goalState quickest. It will pass through intermediate states
    on that path.
*/
/*!
    \qmlproperty bool QtQuick.Particles2::SpriteGoal::jump

    If true, affected sprites will jump directly to the goal state instead of taking the
    the shortest valid path to get there. They will also not finish their current state,
    but immediately move to the beginning of the goal state.

    Default is false.
*/
/*!
    \qmlproperty bool QtQuick.Particles2::SpriteGoal::systemStates

    deprecated, use GroupGoal instead
*/

QQuickSpriteGoalAffector::QQuickSpriteGoalAffector(QQuickItem *parent) :
    QQuickParticleAffector(parent),
    m_goalIdx(-1),
    m_lastEngine(0),
    m_jump(false),
    m_systemStates(false),
    m_notUsingEngine(false)
{
    m_ignoresTime = true;
}

void QQuickSpriteGoalAffector::updateStateIndex(QQuickStochasticEngine* e)
{
    if (m_systemStates){
        m_goalIdx = m_system->groupIds[m_goalState];
    }else{
        m_lastEngine = e;
        for (int i=0; i<e->stateCount(); i++){
            if (e->state(i)->name() == m_goalState){
                m_goalIdx = i;
                return;
            }
        }
        m_goalIdx = -1;//Can't find it
    }
}

void QQuickSpriteGoalAffector::setGoalState(QString arg)
{
    if (m_goalState != arg) {
        m_goalState = arg;
        emit goalStateChanged(arg);
        if (m_goalState.isEmpty())
            m_goalIdx = -1;
        else
            m_goalIdx = -2;
    }
}

bool QQuickSpriteGoalAffector::affectParticle(QQuickParticleData *d, qreal dt)
{
    Q_UNUSED(dt);
    QQuickStochasticEngine *engine = 0;
    if (!m_systemStates){
        //TODO: Affect all engines
        foreach (QQuickParticlePainter *p, m_system->groupData[d->group]->painters)
            if (qobject_cast<QQuickImageParticle*>(p))
                engine = qobject_cast<QQuickImageParticle*>(p)->spriteEngine();
    }else{
        engine = m_system->stateEngine;
        if (!engine)
            m_notUsingEngine = true;
    }
    if (!engine && !m_notUsingEngine)
        return false;

    if (m_goalIdx == -2 || engine != m_lastEngine)
        updateStateIndex(engine);
    int index = d->index;
    if (m_systemStates)
        index = d->systemIndex;
    if (m_notUsingEngine){//systemStates && no stochastic states defined. So cut out the engine
        //TODO: It's possible to move to a group that is intermediate and not used by painters or emitters - but right now that will redirect to the default group
        m_system->moveGroups(d, m_goalIdx);
    }else if (engine->curState(index) != m_goalIdx){
        engine->setGoal(m_goalIdx, index, m_jump);
        return true; //Doesn't affect particle data, but necessary for onceOff
    }
    return false;
}

QT_END_NAMESPACE
