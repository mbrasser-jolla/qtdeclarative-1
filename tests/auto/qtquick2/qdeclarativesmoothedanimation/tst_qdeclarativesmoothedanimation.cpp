/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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
#include <qtest.h>
#include <QtDeclarative/qdeclarativeengine.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QtQuick/private/qdeclarativesmoothedanimation_p.h>
#include <QtQuick/private/qquickrectangle_p.h>
#include <private/qdeclarativevaluetype_p.h>
#include "../../shared/util.h"

class tst_qdeclarativesmoothedanimation : public QDeclarativeDataTest
{
    Q_OBJECT
public:
    tst_qdeclarativesmoothedanimation();

private slots:
    void defaultValues();
    void values();
    void disabled();
    void simpleAnimation();
    void valueSource();
    void behavior();

private:
    QDeclarativeEngine engine;
};

tst_qdeclarativesmoothedanimation::tst_qdeclarativesmoothedanimation()
{
}

void tst_qdeclarativesmoothedanimation::defaultValues()
{
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, testFileUrl("smoothedanimation1.qml"));
    QDeclarativeSmoothedAnimation *obj = qobject_cast<QDeclarativeSmoothedAnimation*>(c.create());

    QVERIFY(obj != 0);

    QCOMPARE(obj->to(), 0.);
    QCOMPARE(obj->velocity(), 200.);
    QCOMPARE(obj->duration(), -1);
    QCOMPARE(obj->maximumEasingTime(), -1);
    QCOMPARE(obj->reversingMode(), QDeclarativeSmoothedAnimation::Eased);

    delete obj;
}

void tst_qdeclarativesmoothedanimation::values()
{
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, testFileUrl("smoothedanimation2.qml"));
    QDeclarativeSmoothedAnimation *obj = qobject_cast<QDeclarativeSmoothedAnimation*>(c.create());

    QVERIFY(obj != 0);

    QCOMPARE(obj->to(), 10.);
    QCOMPARE(obj->velocity(), 200.);
    QCOMPARE(obj->duration(), 300);
    QCOMPARE(obj->maximumEasingTime(), -1);
    QCOMPARE(obj->reversingMode(), QDeclarativeSmoothedAnimation::Immediate);

    delete obj;
}

void tst_qdeclarativesmoothedanimation::disabled()
{
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, testFileUrl("smoothedanimation3.qml"));
    QDeclarativeSmoothedAnimation *obj = qobject_cast<QDeclarativeSmoothedAnimation*>(c.create());

    QVERIFY(obj != 0);

    QCOMPARE(obj->to(), 10.);
    QCOMPARE(obj->velocity(), 250.);
    QCOMPARE(obj->maximumEasingTime(), 150);
    QCOMPARE(obj->reversingMode(), QDeclarativeSmoothedAnimation::Sync);

    delete obj;
}

void tst_qdeclarativesmoothedanimation::simpleAnimation()
{
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, testFileUrl("simpleanimation.qml"));
    QObject *obj = c.create();
    QVERIFY(obj);

    QQuickRectangle *rect = obj->findChild<QQuickRectangle*>("rect");
    QVERIFY(rect);

    QDeclarativeSmoothedAnimation *animation = obj->findChild<QDeclarativeSmoothedAnimation*>("anim");
    QVERIFY(animation);

    animation->setTarget(rect);
    animation->setProperty("x");
    animation->setTo(200);
    animation->setDuration(250);
    QVERIFY(animation->target() == rect);
    QVERIFY(animation->property() == "x");
    QVERIFY(animation->to() == 200);
    animation->start();
    QVERIFY(animation->isRunning());
    QTest::qWait(animation->duration());
    QTRY_COMPARE(rect->x(), qreal(200));
    QTest::qWait(100);  //smoothed animation doesn't report stopped until delayed timer fires

    QVERIFY(!animation->isRunning());
    rect->setX(0);
    animation->start();
    QVERIFY(animation->isRunning());
    animation->pause();
    QVERIFY(animation->isRunning());
    QVERIFY(animation->isPaused());
    animation->setCurrentTime(125);
    QVERIFY(animation->currentTime() == 125);
    QCOMPARE(rect->x(), qreal(100));
}

void tst_qdeclarativesmoothedanimation::valueSource()
{
    QDeclarativeEngine engine;

    QDeclarativeComponent c(&engine, testFileUrl("smoothedanimationValueSource.qml"));

    QQuickRectangle *rect = qobject_cast<QQuickRectangle*>(c.create());
    QVERIFY(rect);

    QQuickRectangle *theRect = rect->findChild<QQuickRectangle*>("theRect");
    QVERIFY(theRect);

    QDeclarativeSmoothedAnimation *easeX = rect->findChild<QDeclarativeSmoothedAnimation*>("easeX");
    QVERIFY(easeX);
    QVERIFY(easeX->isRunning());

    QDeclarativeSmoothedAnimation *easeY = rect->findChild<QDeclarativeSmoothedAnimation*>("easeY");
    QVERIFY(easeY);
    QVERIFY(easeY->isRunning());

    // XXX get the proper duration
    QTest::qWait(100);

    QTRY_VERIFY(!easeX->isRunning());
    QTRY_VERIFY(!easeY->isRunning());

    QTRY_COMPARE(theRect->x(), qreal(200));
    QTRY_COMPARE(theRect->y(), qreal(200));

    delete rect;
}

void tst_qdeclarativesmoothedanimation::behavior()
{
    QDeclarativeEngine engine;

    QDeclarativeComponent c(&engine, testFileUrl("smoothedanimationBehavior.qml"));

    QQuickRectangle *rect = qobject_cast<QQuickRectangle*>(c.create());
    QVERIFY(rect);

    QQuickRectangle *theRect = rect->findChild<QQuickRectangle*>("theRect");
    QVERIFY(theRect);

    QDeclarativeSmoothedAnimation *easeX = rect->findChild<QDeclarativeSmoothedAnimation*>("easeX");
    QVERIFY(easeX);

    QDeclarativeSmoothedAnimation *easeY = rect->findChild<QDeclarativeSmoothedAnimation*>("easeY");
    QVERIFY(easeY);

    // XXX get the proper duration
    QTest::qWait(400);

    QTRY_VERIFY(!easeX->isRunning());
    QTRY_VERIFY(!easeY->isRunning());

    QTRY_COMPARE(theRect->x(), qreal(200));
    QTRY_COMPARE(theRect->y(), qreal(200));

    delete rect;
}

QTEST_MAIN(tst_qdeclarativesmoothedanimation)

#include "tst_qdeclarativesmoothedanimation.moc"
