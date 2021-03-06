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
#include <QtDeclarative/qdeclarativeexpression.h>
#include <QtDeclarative/qdeclarativescriptstring.h>
#include "../../shared/util.h"

class tst_qdeclarativeexpression : public QDeclarativeDataTest
{
    Q_OBJECT
public:
    tst_qdeclarativeexpression() {}

private slots:
    void scriptString();
    void syntaxError();
};

class TestObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeScriptString scriptString READ scriptString WRITE setScriptString)
    Q_PROPERTY(QDeclarativeScriptString scriptStringError READ scriptStringError WRITE setScriptStringError)
public:
    TestObject(QObject *parent = 0) : QObject(parent) {}

    QDeclarativeScriptString scriptString() const { return m_scriptString; }
    void setScriptString(QDeclarativeScriptString scriptString) { m_scriptString = scriptString; }

    QDeclarativeScriptString scriptStringError() const { return m_scriptStringError; }
    void setScriptStringError(QDeclarativeScriptString scriptString) { m_scriptStringError = scriptString; }

private:
    QDeclarativeScriptString m_scriptString;
    QDeclarativeScriptString m_scriptStringError;
};

QML_DECLARE_TYPE(TestObject)

void tst_qdeclarativeexpression::scriptString()
{
    qmlRegisterType<TestObject>("Test", 1, 0, "TestObject");

    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, testFileUrl("scriptString.qml"));
    TestObject *testObj = qobject_cast<TestObject*>(c.create());
    QVERIFY(testObj != 0);

    QDeclarativeScriptString script = testObj->scriptString();
    QCOMPARE(script.script(), QLatin1String("value1 + value2"));

    QDeclarativeExpression expression(script);
    QVariant value = expression.evaluate();
    QCOMPARE(value.toInt(), 15);

    QDeclarativeScriptString scriptError = testObj->scriptStringError();
    QCOMPARE(scriptError.script(), QLatin1String("value3 * 5"));

    //verify that the expression has the correct error location information
    QDeclarativeExpression expressionError(scriptError);
    QVariant valueError = expressionError.evaluate();
    QVERIFY(!valueError.isValid());
    QVERIFY(expressionError.hasError());
    QDeclarativeError error = expressionError.error();
    QCOMPARE(error.url(), c.url());
    QCOMPARE(error.line(), 8);
}

// QTBUG-21310 - crash test
void tst_qdeclarativeexpression::syntaxError()
{
    QDeclarativeEngine engine;
    QDeclarativeExpression expression(engine.rootContext(), 0, "asd asd");
    QVariant v = expression.evaluate();
    QCOMPARE(v, QVariant());
}

QTEST_MAIN(tst_qdeclarativeexpression)

#include "tst_qdeclarativeexpression.moc"
