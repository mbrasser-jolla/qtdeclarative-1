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

#include "qquickparticleaffector_p.h"
#include <QDebug>
QT_BEGIN_NAMESPACE

/*!
    \qmlclass Affector QQuickParticleAffector
    \inqmlmodule QtQuick.Particles 2
    \brief Affector elements can alter the attributes of logical particles at any point in their lifetime.

    The base Affector does not alter any attributes, but can be used to emit a signal
    when a particle meets certain conditions.

    If an affector has a defined size, then it will only affect particles within its size and position on screen.

    Affectors have different performance characteristics to the other particle system elements. In particular,
    they have some simplifications to try to maintain a simulation at real-time or faster. When running a system
    with Affectors, irregular frame timings that grow too large ( > one second per frame) will cause the Affectors
    to try and cut corners with a faster but less accurate simulation. If the system has multiple affectors the order
    in which they are applied is not guaranteed, and when simulating larger time shifts they will simulate the whole
    shift each, which can lead to different results compared to smaller time shifts.

    Accurate simulation for large numbers of particles (hundreds) with multiple affectors may be possible on some hardware,
    but on less capable hardware you should expect small irregularties in the simulation as simulates with worse granularity.
*/
/*!
    \qmlproperty ParticleSystem QtQuick.Particles2::Affector::system
    This is the system which will be affected by the element.
    If the Affector is a direct child of a ParticleSystem, it will automatically be associated with it.
*/
/*!
    \qmlproperty list<string> QtQuick.Particles2::Affector::groups
    Which logical particle groups will be affected.

    If empty, it will affect all particles.
*/
/*!
    \qmlproperty list<string> QtQuick.Particles2::Affector::whenCollidingWith
    If any logical particle groups are specified here, then the affector
    will only be triggered if the particle being examined intersects with
    a particle of one of these groups.

    This is different from the groups property. The groups property selects which
    particles might be examined, and if they meet other criteria (including being
    within the bounds of the Affector, modified by shape) then they will be tested
    again to see if they intersect with a particles from one of the particle groups
    in whenCollidingWith.

    By default, no groups are specified.
*/
/*!
    \qmlproperty bool QtQuick.Particles2::Affector::enabled
    If enabled is set to false, this affector will not affect any particles.

    Usually this is used to conditionally turn an affector on or off.

    Default value is true.
*/
/*!
    \qmlproperty bool QtQuick.Particles2::Affector::once
    If once is set to true, this affector will only affect each particle
    once in their lifetimes. If the affector normally simulates a continuous
    effect over time, then it will simulate the effect of one second of time
    the one instant it affects the particle.

    Default value is false.
*/
/*!
    \qmlproperty Shape QtQuick.Particles2::Affector::shape
    If a size has been defined, the shape property can be used to affect a
    non-rectangular area.
*/
/*!
    \qmlsignal QtQuick.Particles2::Affector::onAffected(x, y)

    This signal is emitted each time the affector actually affects a particle.

    x,y are the coordinates of the affected particle, relative to the ParticleSystem.

*/

/*!
    \qmlsignal QtQuick.Particles2::Affector::affectParticle(particle particle, real dt)

    This handler is called when particles are selected to be affected.

    dt is the time since the last time it was affected. Use dt to normalize
    trajectory manipulations to real time.

    Note that JS is slower to execute, so it is not recommended to use this in
    high-volume particle systems.
*/
/*!
    \qmlsignal QtQuick.Particles2::Affector::affected(real x, real y)

    This handler is called when a particle is selected to be affected. It will
    only be called if signal is set to true.

    x,y is the particles current position.
*/
QQuickParticleAffector::QQuickParticleAffector(QQuickItem *parent) :
    QQuickItem(parent), m_needsReset(false), m_ignoresTime(false), m_onceOff(false), m_enabled(true)
    , m_system(0), m_updateIntSet(false), m_shape(new QQuickParticleExtruder(this))
{
}

bool QQuickParticleAffector::isAffectedConnected()
{
    static int idx = QObjectPrivate::get(this)->signalIndex("affected(qreal,qreal)");
    return QObjectPrivate::get(this)->isSignalConnected(idx);
}


void QQuickParticleAffector::componentComplete()
{
    if (!m_system && qobject_cast<QQuickParticleSystem*>(parentItem()))
        setSystem(qobject_cast<QQuickParticleSystem*>(parentItem()));
    QQuickItem::componentComplete();
}

bool QQuickParticleAffector::activeGroup(int g) {
    if (m_updateIntSet){ //This can occur before group ids are properly assigned, but that resets the flag
        m_groupIds.clear();
        foreach (const QString &p, m_groups)
            m_groupIds << m_system->groupIds[p];
        m_updateIntSet = false;
    }
    return m_groupIds.isEmpty() || m_groupIds.contains(g);
}

bool QQuickParticleAffector::shouldAffect(QQuickParticleData* d)
{
    if (!d)
        return false;
    if (activeGroup(d->group)){
        if ((m_onceOff && m_onceOffed.contains(qMakePair(d->group, d->index)))
                || !d->stillAlive())
            return false;
        //Need to have previous location for affected anyways
        if (width() == 0 || height() == 0
                || m_shape->contains(QRectF(m_offset.x(), m_offset.y(), width(), height()), QPointF(d->curX(), d->curY()))){
            if (m_whenCollidingWith.isEmpty() || isColliding(d)){
                return true;
            }
        }
    }
    return false;

}

void QQuickParticleAffector::postAffect(QQuickParticleData* d)
{
    m_system->needsReset << d;
    if (m_onceOff)
        m_onceOffed << qMakePair(d->group, d->index);
    if (isAffectedConnected())
        emit affected(d->curX(), d->curY());
}

const qreal QQuickParticleAffector::simulationDelta = 0.020;
const qreal QQuickParticleAffector::simulationCutoff = 1.000;//If this goes above 1.0, then m_once behaviour needs special codepath

void QQuickParticleAffector::affectSystem(qreal dt)
{
    if (!m_enabled)
        return;
    //If not reimplemented, calls affectParticle per particle
    //But only on particles in targeted system/area
    updateOffsets();//### Needed if an ancestor is transformed.
    if (m_onceOff)
        dt = 1.0;
    foreach (QQuickParticleGroupData* gd, m_system->groupData) {
        if (activeGroup(m_system->groupData.key(gd))) {
            foreach (QQuickParticleData* d, gd->data) {
                if (shouldAffect(d)) {
                    bool affected = false;
                    qreal myDt = dt;
                    if (!m_ignoresTime && myDt < simulationCutoff) {
                        int realTime = m_system->timeInt;
                        m_system->timeInt -= myDt * 1000.0;
                        while (myDt > simulationDelta) {
                            m_system->timeInt += simulationDelta * 1000.0;
                            if (d->alive())//Only affect during the parts it was alive for
                                affected = affectParticle(d, simulationDelta) || affected;
                            myDt -= simulationDelta;
                        }
                        m_system->timeInt = realTime;
                    }
                    if (myDt > 0.0)
                        affected = affectParticle(d, myDt) || affected;
                    if (affected)
                        postAffect(d);
                }
            }
        }
    }
}

bool QQuickParticleAffector::affectParticle(QQuickParticleData *, qreal )
{
    return true;
}

void QQuickParticleAffector::reset(QQuickParticleData* pd)
{//TODO: This, among other ones, should be restructured so they don't all need to remember to call the superclass
    if (m_onceOff)
        if (activeGroup(pd->group))
            m_onceOffed.remove(qMakePair(pd->group, pd->index));
}

void QQuickParticleAffector::updateOffsets()
{
    if (m_system)
        m_offset = m_system->mapFromItem(this, QPointF(0, 0));
}

bool QQuickParticleAffector::isColliding(QQuickParticleData *d)
{
    qreal myCurX = d->curX();
    qreal myCurY = d->curY();
    qreal myCurSize = d->curSize()/2;
    foreach (const QString &group, m_whenCollidingWith){
        foreach (QQuickParticleData* other, m_system->groupData[m_system->groupIds[group]]->data){
            if (!other->stillAlive())
                continue;
            qreal otherCurX = other->curX();
            qreal otherCurY = other->curY();
            qreal otherCurSize = other->curSize()/2;
            if ((myCurX + myCurSize > otherCurX - otherCurSize
                 && myCurX - myCurSize < otherCurX + otherCurSize)
                 && (myCurY + myCurSize > otherCurY - otherCurSize
                     && myCurY - myCurSize < otherCurY + otherCurSize))
                return true;
        }
    }
    return false;
}

QT_END_NAMESPACE
