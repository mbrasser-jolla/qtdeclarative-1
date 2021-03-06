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
#include <QtQuick/private/qquicktext_p.h>
#include <private/qdeclarativeengine_p.h>
#include <QtCore/qcryptographichash.h>
/*
#include <QtWebKit/qwebpage.h>
#include <QtWebKit/qwebframe.h>
#include <QtWebKit/qwebdatabase.h>
#include <QtWebKit/qwebsecurityorigin.h>
*/
#include <QtSql/qsqldatabase.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include "../../shared/util.h"

class tst_qdeclarativesqldatabase : public QDeclarativeDataTest
{
    Q_OBJECT
public:
    tst_qdeclarativesqldatabase()
    {
        qApp->setApplicationName("tst_qdeclarativesqldatabase");
        qApp->setOrganizationName("Nokia");
        qApp->setOrganizationDomain("nokia.com");
        engine = new QDeclarativeEngine;
    }

    ~tst_qdeclarativesqldatabase()
    {
        delete engine;
    }

private slots:
    void initTestCase();

    void checkDatabasePath();

    void testQml_data();
    void testQml();
    void testQml_cleanopen_data();
    void testQml_cleanopen();
    void totalDatabases();

    void cleanupTestCase();

private:
    QString dbDir() const;
    QDeclarativeEngine *engine;
};

void removeRecursive(const QString& dirname)
{
    QDir dir(dirname);
    QFileInfoList entries(dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot));
    for (int i = 0; i < entries.count(); ++i)
        if (entries[i].isDir())
            removeRecursive(entries[i].filePath());
        else
            dir.remove(entries[i].fileName());
    QDir().rmdir(dirname);
}

void tst_qdeclarativesqldatabase::initTestCase()
{
    QDeclarativeDataTest::initTestCase();
    removeRecursive(dbDir());
    QDir().mkpath(dbDir());
}

void tst_qdeclarativesqldatabase::cleanupTestCase()
{
    removeRecursive(dbDir());
}

QString tst_qdeclarativesqldatabase::dbDir() const
{
    static QString tmpd = QDir::tempPath()+"/tst_qdeclarativesqldatabase_output-"
        + QDateTime::currentDateTime().toString(QLatin1String("yyyyMMddhhmmss"));
    return tmpd;
}

void tst_qdeclarativesqldatabase::checkDatabasePath()
{
    // Check default storage path (we can't use it since we don't want to mess with user's data)
    QVERIFY(engine->offlineStoragePath().contains("tst_qdeclarativesqldatabase"));
    QVERIFY(engine->offlineStoragePath().contains("OfflineStorage"));
}

static const int total_databases_created_by_tests = 12;
void tst_qdeclarativesqldatabase::testQml_data()
{
    QTest::addColumn<QString>("jsfile"); // The input file

    // Each test should use a newly named DB to avoid inter-test dependencies
    QTest::newRow("creation") << "creation.js";
    QTest::newRow("creation-a") << "creation-a.js";
    QTest::newRow("creation") << "creation.js";
    QTest::newRow("error-creation") << "error-creation.js"; // re-uses above DB
    QTest::newRow("changeversion") << "changeversion.js";
    QTest::newRow("readonly") << "readonly.js";
    QTest::newRow("readonly-error") << "readonly-error.js";
    QTest::newRow("selection") << "selection.js";
    QTest::newRow("selection-bindnames") << "selection-bindnames.js";
    QTest::newRow("iteration") << "iteration.js";
    QTest::newRow("iteration-forwardonly") << "iteration-forwardonly.js";
    QTest::newRow("error-a") << "error-a.js";
    QTest::newRow("error-notransaction") << "error-notransaction.js";
    QTest::newRow("error-outsidetransaction") << "error-outsidetransaction.js"; // reuse above
    QTest::newRow("reopen1") << "reopen1.js";
    QTest::newRow("reopen2") << "reopen2.js"; // re-uses above DB

    // If you add a test, you should usually use a new database in the
    // test - in which case increment total_databases_created_by_tests above.
}

/*
class QWebPageWithJavaScriptConsoleMessages : public QWebPage {
public:
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID)
    {
        qWarning() << sourceID << ":" << lineNumber << ":" << message;
    }
};

void tst_qdeclarativesqldatabase::validateAgainstWebkit()
{
    // Validates tests against WebKit (HTML5) support.
    //
    QFETCH(QString, jsfile);
    QFETCH(QString, result);
    QFETCH(int, databases);

    QFile f(jsfile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    QString js=f.readAll();

    QWebPageWithJavaScriptConsoleMessages webpage;
    webpage.settings()->setOfflineStoragePath(dbDir());
    webpage.settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);

    QEXPECT_FAIL("","WebKit doesn't support openDatabaseSync yet", Continue);
    QCOMPARE(webpage.mainFrame()->evaluateJavaScript(js).toString(),result);

    QTest::qWait(100); // WebKit crashes if you quit it too fast

    QWebSecurityOrigin origin = webpage.mainFrame()->securityOrigin();
    QList<QWebDatabase> dbs = origin.databases();
    QCOMPARE(dbs.count(), databases);
}
*/

void tst_qdeclarativesqldatabase::testQml()
{
    // Tests QML SQL Database support with tests
    // that have been validated against Webkit.
    //
    QFETCH(QString, jsfile);

    QString qml=
        "import QtQuick 2.0\n"
        "import \""+jsfile+"\" as JS\n"
        "Text { text: JS.test() }";

    engine->setOfflineStoragePath(dbDir());
    QDeclarativeComponent component(engine);
    component.setData(qml.toUtf8(), testFileUrl("empty.qml")); // just a file for relative local imports
    QVERIFY(!component.isError());
    QQuickText *text = qobject_cast<QQuickText*>(component.create());
    QVERIFY(text != 0);
    QCOMPARE(text->text(),QString("passed"));
}

void tst_qdeclarativesqldatabase::testQml_cleanopen_data()
{
    QTest::addColumn<QString>("jsfile"); // The input file
    QTest::newRow("reopen1") << "reopen1.js";
    QTest::newRow("reopen2") << "reopen2.js";
    QTest::newRow("error-creation") << "error-creation.js"; // re-uses creation DB
}

void tst_qdeclarativesqldatabase::testQml_cleanopen()
{
    // Same as testQml, but clean connections between tests,
    // making it more like the tests are running in new processes.
    testQml();

    engine->collectGarbage();

    foreach (QString dbname, QSqlDatabase::connectionNames()) {
        QSqlDatabase::removeDatabase(dbname);
    }
}

void tst_qdeclarativesqldatabase::totalDatabases()
{
    QCOMPARE(QDir(dbDir()+"/Databases").entryInfoList(QDir::Files|QDir::NoDotAndDotDot).count(), total_databases_created_by_tests*2);
}

QTEST_MAIN(tst_qdeclarativesqldatabase)

#include "tst_qdeclarativesqldatabase.moc"
