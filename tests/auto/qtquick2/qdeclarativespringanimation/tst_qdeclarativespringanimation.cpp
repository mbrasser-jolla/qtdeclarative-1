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
#include <private/qdeclarativespringanimation_p.h>
#include <private/qdeclarativevaluetype_p.h>
#include "../../shared/util.h"

class tst_qdeclarativespringanimation : public QDeclarativeDataTest
{
    Q_OBJECT
public:
    tst_qdeclarativespringanimation();

private slots:
    void defaultValues();
    void values();
    void disabled();

private:
    QDeclarativeEngine engine;
};

tst_qdeclarativespringanimation::tst_qdeclarativespringanimation()
{
}

void tst_qdeclarativespringanimation::defaultValues()
{
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, testFileUrl("springanimation1.qml"));
    QDeclarativeSpringAnimation *obj = qobject_cast<QDeclarativeSpringAnimation*>(c.create());

    QVERIFY(obj != 0);

    QCOMPARE(obj->to(), 0.);
    QCOMPARE(obj->velocity(), 0.);
    QCOMPARE(obj->spring(), 0.);
    QCOMPARE(obj->damping(), 0.);
    QCOMPARE(obj->epsilon(), 0.01);
    QCOMPARE(obj->modulus(), 0.);
    QCOMPARE(obj->mass(), 1.);
    QCOMPARE(obj->isRunning(), false);

    delete obj;
}

void tst_qdeclarativespringanimation::values()
{
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, testFileUrl("springanimation2.qml"));
    QObject *root = c.create();

    QDeclarativeSpringAnimation *obj = root->findChild<QDeclarativeSpringAnimation*>();

    QVERIFY(obj != 0);

    QCOMPARE(obj->to(), 1.44);
    QCOMPARE(obj->velocity(), 0.9);
    QCOMPARE(obj->spring(), 1.0);
    QCOMPARE(obj->damping(), 0.5);
    QCOMPARE(obj->epsilon(), 0.25);
    QCOMPARE(obj->modulus(), 360.0);
    QCOMPARE(obj->mass(), 2.0);
    QCOMPARE(obj->isRunning(), true);

    QTRY_COMPARE(obj->isRunning(), false);

    delete obj;
}

void tst_qdeclarativespringanimation::disabled()
{
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, testFileUrl("springanimation3.qml"));
    QDeclarativeSpringAnimation *obj = qobject_cast<QDeclarativeSpringAnimation*>(c.create());

    QVERIFY(obj != 0);

    QCOMPARE(obj->to(), 1.44);
    QCOMPARE(obj->velocity(), 0.9);
    QCOMPARE(obj->spring(), 1.0);
    QCOMPARE(obj->damping(), 0.5);
    QCOMPARE(obj->epsilon(), 0.25);
    QCOMPARE(obj->modulus(), 360.0);
    QCOMPARE(obj->mass(), 2.0);
    QCOMPARE(obj->isRunning(), false);

    delete obj;
}

QTEST_MAIN(tst_qdeclarativespringanimation)

#include "tst_qdeclarativespringanimation.moc"
