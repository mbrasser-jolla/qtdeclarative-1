/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtSG module of the Qt Toolkit.
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

#include "qquickpincharea_p_p.h"
#include "qquickcanvas.h"

#include <QtGui/qevent.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qstylehints.h>

#include <float.h>
#include <math.h>

QT_BEGIN_NAMESPACE

/*!
    \qmlclass PinchEvent QQuickPinchEvent
    \inqmlmodule QtQuick 2
    \ingroup qml-event-elements
    \brief The PinchEvent object provides information about a pinch event.

    \bold {The PinchEvent element was added in QtQuick 1.1}

    The \c center, \c startCenter, \c previousCenter properties provide the center position between the two touch points.

    The \c scale and \c previousScale properties provide the scale factor.

    The \c angle, \c previousAngle and \c rotation properties provide the angle between the two points and the amount of rotation.

    The \c point1, \c point2, \c startPoint1, \c startPoint2 properties provide the positions of the touch points.

    The \c accepted property may be set to false in the \c onPinchStarted handler if the gesture should not
    be handled.

    \sa PinchArea
*/

/*!
    \qmlproperty QPointF QtQuick2::PinchEvent::center
    \qmlproperty QPointF QtQuick2::PinchEvent::startCenter
    \qmlproperty QPointF QtQuick2::PinchEvent::previousCenter

    These properties hold the position of the center point between the two touch points.

    \list
    \o \c center is the current center point
    \o \c previousCenter is the center point of the previous event.
    \o \c startCenter is the center point when the gesture began
    \endlist
*/

/*!
    \qmlproperty real QtQuick2::PinchEvent::scale
    \qmlproperty real QtQuick2::PinchEvent::previousScale

    These properties hold the scale factor determined by the change in distance between the two touch points.

    \list
    \o \c scale is the current scale factor.
    \o \c previousScale is the scale factor of the previous event.
    \endlist

    When a pinch gesture is started, the scale is 1.0.
*/

/*!
    \qmlproperty real QtQuick2::PinchEvent::angle
    \qmlproperty real QtQuick2::PinchEvent::previousAngle
    \qmlproperty real QtQuick2::PinchEvent::rotation

    These properties hold the angle between the two touch points.

    \list
    \o \c angle is the current angle between the two points in the range -180 to 180.
    \o \c previousAngle is the angle of the previous event.
    \o \c rotation is the total rotation since the pinch gesture started.
    \endlist

    When a pinch gesture is started, the rotation is 0.0.
*/

/*!
    \qmlproperty QPointF QtQuick2::PinchEvent::point1
    \qmlproperty QPointF QtQuick2::PinchEvent::startPoint1
    \qmlproperty QPointF QtQuick2::PinchEvent::point2
    \qmlproperty QPointF QtQuick2::PinchEvent::startPoint2

    These properties provide the actual touch points generating the pinch.

    \list
    \o \c point1 and \c point2 hold the current positions of the points.
    \o \c startPoint1 and \c startPoint2 hold the positions of the points when the second point was touched.
    \endlist
*/

/*!
    \qmlproperty bool QtQuick2::PinchEvent::accepted

    Setting this property to false in the \c PinchArea::onPinchStarted handler
    will result in no further pinch events being generated, and the gesture
    ignored.
*/

/*!
    \qmlproperty int QtQuick2::PinchEvent::pointCount

    Holds the number of points currently touched.  The PinchArea will not react
    until two touch points have initited a gesture, but will remain active until
    all touch points have been released.
*/

QQuickPinch::QQuickPinch()
    : m_target(0), m_minScale(1.0), m_maxScale(1.0)
    , m_minRotation(0.0), m_maxRotation(0.0)
    , m_axis(NoDrag), m_xmin(-FLT_MAX), m_xmax(FLT_MAX)
    , m_ymin(-FLT_MAX), m_ymax(FLT_MAX), m_active(false)
{
}

QQuickPinchAreaPrivate::~QQuickPinchAreaPrivate()
{
    delete pinch;
}

/*!
    \qmlclass PinchArea QQuickPinchArea
    \inqmlmodule QtQuick 2
    \brief The PinchArea item enables simple pinch gesture handling.
    \inherits Item

    \bold {The PinchArea element was added in QtQuick 1.1}

    A PinchArea is an invisible item that is typically used in conjunction with
    a visible item in order to provide pinch gesture handling for that item.

    The \l enabled property is used to enable and disable pinch handling for
    the proxied item. When disabled, the pinch area becomes transparent to
    mouse/touch events.

    PinchArea can be used in two ways:

    \list
    \o setting a \c pinch.target to provide automatic interaction with an element
    \o using the onPinchStarted, onPinchUpdated and onPinchFinished handlers
    \endlist

    \sa PinchEvent
*/

/*!
    \qmlsignal QtQuick2::PinchArea::onPinchStarted()

    This handler is called when the pinch area detects that a pinch gesture has started.

    The \l {PinchEvent}{pinch} parameter provides information about the pinch gesture,
    including the scale, center and angle of the pinch.

    To ignore this gesture set the \c pinch.accepted property to false.  The gesture
    will be canceled and no further events will be sent.
*/

/*!
    \qmlsignal QtQuick2::PinchArea::onPinchUpdated()

    This handler is called when the pinch area detects that a pinch gesture has changed.

    The \l {PinchEvent}{pinch} parameter provides information about the pinch gesture,
    including the scale, center and angle of the pinch.
*/

/*!
    \qmlsignal QtQuick2::PinchArea::onPinchFinished()

    This handler is called when the pinch area detects that a pinch gesture has finished.

    The \l {PinchEvent}{pinch} parameter provides information about the pinch gesture,
    including the scale, center and angle of the pinch.
*/


/*!
    \qmlproperty Item QtQuick2::PinchArea::pinch.target
    \qmlproperty bool QtQuick2::PinchArea::pinch.active
    \qmlproperty real QtQuick2::PinchArea::pinch.minimumScale
    \qmlproperty real QtQuick2::PinchArea::pinch.maximumScale
    \qmlproperty real QtQuick2::PinchArea::pinch.minimumRotation
    \qmlproperty real QtQuick2::PinchArea::pinch.maximumRotation
    \qmlproperty enumeration QtQuick2::PinchArea::pinch.dragAxis
    \qmlproperty real QtQuick2::PinchArea::pinch.minimumX
    \qmlproperty real QtQuick2::PinchArea::pinch.maximumX
    \qmlproperty real QtQuick2::PinchArea::pinch.minimumY
    \qmlproperty real QtQuick2::PinchArea::pinch.maximumY

    \c pinch provides a convenient way to make an item react to pinch gestures.

    \list
    \i \c pinch.target specifies the id of the item to drag.
    \i \c pinch.active specifies if the target item is currently being dragged.
    \i \c pinch.minimumScale and \c pinch.maximumScale limit the range of the Item::scale property.
    \i \c pinch.minimumRotation and \c pinch.maximumRotation limit the range of the Item::rotation property.
    \i \c pinch.dragAxis specifies whether dragging in not allowed (\c Pinch.NoDrag), can be done horizontally (\c Pinch.XAxis), vertically (\c Pinch.YAxis), or both (\c Pinch.XandYAxis)
    \i \c pinch.minimum and \c pinch.maximum limit how far the target can be dragged along the corresponding axes.
    \endlist
*/

QQuickPinchArea::QQuickPinchArea(QQuickItem *parent)
  : QQuickItem(*(new QQuickPinchAreaPrivate), parent)
{
    Q_D(QQuickPinchArea);
    d->init();
}

QQuickPinchArea::~QQuickPinchArea()
{
}
/*!
    \qmlproperty bool QtQuick2::PinchArea::enabled
    This property holds whether the item accepts pinch gestures.

    This property defaults to true.
*/
bool QQuickPinchArea::isEnabled() const
{
    Q_D(const QQuickPinchArea);
    return d->absorb;
}

void QQuickPinchArea::setEnabled(bool a)
{
    Q_D(QQuickPinchArea);
    if (a != d->absorb) {
        d->absorb = a;
        emit enabledChanged();
    }
}

void QQuickPinchArea::touchEvent(QTouchEvent *event)
{
    Q_D(QQuickPinchArea);
    if (!d->absorb || !isVisible()) {
        QQuickItem::event(event);
        return;
    }

    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
        d->touchPoints.clear();
        for (int i = 0; i < event->touchPoints().count(); ++i) {
            if (!(event->touchPoints().at(i).state() & Qt::TouchPointReleased)) {
                d->touchPoints << event->touchPoints().at(i);
            }
        }
        updatePinch();
        break;
    case QEvent::TouchEnd:
        d->touchPoints.clear();
        updatePinch();
        break;
    default:
        QQuickItem::event(event);
    }
}

void QQuickPinchArea::updatePinch()
{
    Q_D(QQuickPinchArea);
    if (d->touchPoints.count() == 0) {
        if (d->inPinch) {
            d->inPinch = false;
            QPointF pinchCenter = mapFromScene(d->sceneLastCenter);
            QQuickPinchEvent pe(pinchCenter, d->pinchLastScale, d->pinchLastAngle, d->pinchRotation);
            pe.setStartCenter(d->pinchStartCenter);
            pe.setPreviousCenter(pinchCenter);
            pe.setPreviousAngle(d->pinchLastAngle);
            pe.setPreviousScale(d->pinchLastScale);
            pe.setStartPoint1(mapFromScene(d->sceneStartPoint1));
            pe.setStartPoint2(mapFromScene(d->sceneStartPoint2));
            pe.setPoint1(mapFromScene(d->lastPoint1));
            pe.setPoint2(mapFromScene(d->lastPoint2));
            emit pinchFinished(&pe);
            d->pinchStartDist = 0;
            d->pinchActivated = false;
            if (d->pinch && d->pinch->target())
                d->pinch->setActive(false);
        }
        d->initPinch = false;
        d->pinchRejected = false;
        d->stealMouse = false;
        setKeepMouseGrab(false);
        QQuickCanvas *c = canvas();
        if (c && c->mouseGrabberItem() == this)
            ungrabMouse();
        return;
    }
    QTouchEvent::TouchPoint touchPoint1 = d->touchPoints.at(0);
    QTouchEvent::TouchPoint touchPoint2 = d->touchPoints.at(d->touchPoints. count() >= 2 ? 1 : 0);
    if (d->touchPoints.count() == 2
        && (touchPoint1.state() & Qt::TouchPointPressed || touchPoint2.state() & Qt::TouchPointPressed)) {
        d->id1 = touchPoint1.id();
        d->sceneStartPoint1 = touchPoint1.scenePos();
        d->sceneStartPoint2 = touchPoint2.scenePos();
        d->pinchActivated = true;
        d->initPinch = true;
    }
    if (d->pinchActivated && !d->pinchRejected){
        const int dragThreshold = qApp->styleHints()->startDragDistance();
        QPointF p1 = touchPoint1.scenePos();
        QPointF p2 = touchPoint2.scenePos();
        qreal dx = p1.x() - p2.x();
        qreal dy = p1.y() - p2.y();
        qreal dist = sqrt(dx*dx + dy*dy);
        QPointF sceneCenter = (p1 + p2)/2;
        qreal angle = QLineF(p1, p2).angle();
        if (d->touchPoints.count() == 1) {
            // If we only have one point then just move the center
            if (d->id1 == touchPoint1.id())
                sceneCenter = d->sceneLastCenter + touchPoint1.scenePos() - d->lastPoint1;
            else
                sceneCenter = d->sceneLastCenter + touchPoint2.scenePos() - d->lastPoint2;
            angle = d->pinchLastAngle;
        }
        d->id1 = touchPoint1.id();
        if (angle > 180)
            angle -= 360;
        if (!d->inPinch || d->initPinch) {
            if (d->touchPoints.count() >= 2
                    && (qAbs(p1.x()-d->sceneStartPoint1.x()) > dragThreshold
                    || qAbs(p1.y()-d->sceneStartPoint1.y()) > dragThreshold
                    || qAbs(p2.x()-d->sceneStartPoint2.x()) > dragThreshold
                    || qAbs(p2.y()-d->sceneStartPoint2.y()) > dragThreshold)) {
                d->initPinch = false;
                d->sceneStartCenter = sceneCenter;
                d->sceneLastCenter = sceneCenter;
                d->pinchStartCenter = mapFromScene(sceneCenter);
                d->pinchStartDist = dist;
                d->pinchStartAngle = angle;
                d->pinchLastScale = 1.0;
                d->pinchLastAngle = angle;
                d->pinchRotation = 0.0;
                d->lastPoint1 = p1;
                d->lastPoint2 = p2;
                QQuickPinchEvent pe(d->pinchStartCenter, 1.0, angle, 0.0);
                pe.setStartCenter(d->pinchStartCenter);
                pe.setPreviousCenter(d->pinchStartCenter);
                pe.setPreviousAngle(d->pinchLastAngle);
                pe.setPreviousScale(d->pinchLastScale);
                pe.setStartPoint1(mapFromScene(d->sceneStartPoint1));
                pe.setStartPoint2(mapFromScene(d->sceneStartPoint2));
                pe.setPoint1(mapFromScene(d->lastPoint1));
                pe.setPoint2(mapFromScene(d->lastPoint2));
                pe.setPointCount(d->touchPoints.count());
                emit pinchStarted(&pe);
                if (pe.accepted()) {
                    d->inPinch = true;
                    d->stealMouse = true;
                    QQuickCanvas *c = canvas();
                    if (c && c->mouseGrabberItem() != this)
                        grabMouse();
                    setKeepMouseGrab(true);
                    if (d->pinch && d->pinch->target()) {
                        d->pinchStartPos = pinch()->target()->pos();
                        d->pinchStartScale = d->pinch->target()->scale();
                        d->pinchStartRotation = d->pinch->target()->rotation();
                        d->pinch->setActive(true);
                    }
                } else {
                    d->pinchRejected = true;
                }
            }
        } else if (d->pinchStartDist > 0) {
            qreal scale = dist ? dist / d->pinchStartDist : d->pinchLastScale;
            qreal da = d->pinchLastAngle - angle;
            if (da > 180)
                da -= 360;
            else if (da < -180)
                da += 360;
            d->pinchRotation += da;
            QPointF pinchCenter = mapFromScene(sceneCenter);
            QQuickPinchEvent pe(pinchCenter, scale, angle, d->pinchRotation);
            pe.setStartCenter(d->pinchStartCenter);
            pe.setPreviousCenter(mapFromScene(d->sceneLastCenter));
            pe.setPreviousAngle(d->pinchLastAngle);
            pe.setPreviousScale(d->pinchLastScale);
            pe.setStartPoint1(mapFromScene(d->sceneStartPoint1));
            pe.setStartPoint2(mapFromScene(d->sceneStartPoint2));
            pe.setPoint1(touchPoint1.pos());
            pe.setPoint2(touchPoint2.pos());
            pe.setPointCount(d->touchPoints.count());
            d->pinchLastScale = scale;
            d->sceneLastCenter = sceneCenter;
            d->pinchLastAngle = angle;
            d->lastPoint1 = touchPoint1.scenePos();
            d->lastPoint2 = touchPoint2.scenePos();
            emit pinchUpdated(&pe);
            if (d->pinch && d->pinch->target()) {
                qreal s = d->pinchStartScale * scale;
                s = qMin(qMax(pinch()->minimumScale(),s), pinch()->maximumScale());
                pinch()->target()->setScale(s);
                QPointF pos = sceneCenter - d->sceneStartCenter + d->pinchStartPos;
                if (pinch()->axis() & QQuickPinch::XAxis) {
                    qreal x = pos.x();
                    if (x < pinch()->xmin())
                        x = pinch()->xmin();
                    else if (x > pinch()->xmax())
                        x = pinch()->xmax();
                    pinch()->target()->setX(x);
                }
                if (pinch()->axis() & QQuickPinch::YAxis) {
                    qreal y = pos.y();
                    if (y < pinch()->ymin())
                        y = pinch()->ymin();
                    else if (y > pinch()->ymax())
                        y = pinch()->ymax();
                    pinch()->target()->setY(y);
                }
                if (d->pinchStartRotation >= pinch()->minimumRotation()
                        && d->pinchStartRotation <= pinch()->maximumRotation()) {
                    qreal r = d->pinchRotation + d->pinchStartRotation;
                    r = qMin(qMax(pinch()->minimumRotation(),r), pinch()->maximumRotation());
                    pinch()->target()->setRotation(r);
                }
            }
        }
    }
}

void QQuickPinchArea::mousePressEvent(QMouseEvent *event)
{
    Q_D(QQuickPinchArea);
    d->stealMouse = false;
    if (!d->absorb)
        QQuickItem::mousePressEvent(event);
    else {
        setKeepMouseGrab(false);
        event->setAccepted(true);
    }
}

void QQuickPinchArea::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QQuickPinchArea);
    if (!d->absorb) {
        QQuickItem::mouseMoveEvent(event);
        return;
    }
}

void QQuickPinchArea::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QQuickPinchArea);
    d->stealMouse = false;
    if (!d->absorb) {
        QQuickItem::mouseReleaseEvent(event);
    } else {
        QQuickCanvas *c = canvas();
        if (c && c->mouseGrabberItem() == this)
            ungrabMouse();
        setKeepMouseGrab(false);
    }
}

void QQuickPinchArea::mouseUngrabEvent()
{
    setKeepMouseGrab(false);
}

bool QQuickPinchArea::sendMouseEvent(QMouseEvent *event)
{
    Q_D(QQuickPinchArea);
    QRectF myRect = mapRectToScene(QRectF(0, 0, width(), height()));

    QQuickCanvas *c = canvas();
    QQuickItem *grabber = c ? c->mouseGrabberItem() : 0;
    bool stealThisEvent = d->stealMouse;
    if ((stealThisEvent || myRect.contains(event->windowPos())) && (!grabber || !grabber->keepMouseGrab())) {
        QMouseEvent mouseEvent(event->type(), mapFromScene(event->windowPos()), event->windowPos(), event->screenPos(),
                               event->button(), event->buttons(), event->modifiers());
        mouseEvent.setAccepted(false);

        switch (mouseEvent.type()) {
        case QEvent::MouseMove:
            mouseMoveEvent(&mouseEvent);
            break;
        case QEvent::MouseButtonPress:
            mousePressEvent(&mouseEvent);
            break;
        case QEvent::MouseButtonRelease:
            mouseReleaseEvent(&mouseEvent);
            break;
        default:
            break;
        }
        grabber = c->mouseGrabberItem();
        if (grabber && stealThisEvent && !grabber->keepMouseGrab() && grabber != this)
            grabMouse();

        return stealThisEvent;
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        d->stealMouse = false;
        if (c && c->mouseGrabberItem() == this)
            ungrabMouse();
        setKeepMouseGrab(false);
    }
    return false;
}

bool QQuickPinchArea::childMouseEventFilter(QQuickItem *i, QEvent *e)
{
    Q_D(QQuickPinchArea);
    if (!d->absorb || !isVisible())
        return QQuickItem::childMouseEventFilter(i, e);
    switch (e->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseMove:
    case QEvent::MouseButtonRelease:
        return sendMouseEvent(static_cast<QMouseEvent *>(e));
        break;
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate: {
            QTouchEvent *touch = static_cast<QTouchEvent*>(e);
            d->touchPoints.clear();
            for (int i = 0; i < touch->touchPoints().count(); ++i)
                if (!(touch->touchPoints().at(i).state() & Qt::TouchPointReleased))
                    d->touchPoints << touch->touchPoints().at(i);
            updatePinch();
        }
        return d->inPinch;
    case QEvent::TouchEnd:
        d->touchPoints.clear();
        updatePinch();
        break;
    default:
        break;
    }

    return QQuickItem::childMouseEventFilter(i, e);
}

void QQuickPinchArea::geometryChanged(const QRectF &newGeometry,
                                            const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

void QQuickPinchArea::itemChange(ItemChange change, const ItemChangeData &value)
{
    QQuickItem::itemChange(change, value);
}

QQuickPinch *QQuickPinchArea::pinch()
{
    Q_D(QQuickPinchArea);
    if (!d->pinch)
        d->pinch = new QQuickPinch;
    return d->pinch;
}


QT_END_NAMESPACE

