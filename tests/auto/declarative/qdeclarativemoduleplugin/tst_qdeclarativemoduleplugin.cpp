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
#include <qdir.h>
#include <QtDeclarative/qdeclarativeengine.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QDebug>

#include "../../shared/testhttpserver.h"
#include "../../shared/util.h"

#define SERVER_ADDR "http://127.0.0.1:14450"
#define SERVER_PORT 14450


class tst_qdeclarativemoduleplugin : public QDeclarativeDataTest
{
    Q_OBJECT
public:

private slots:
    virtual void initTestCase();
    void importsPlugin();
    void importsPlugin2();
    void importsPlugin21();
    void importsMixedQmlCppPlugin();
    void incorrectPluginCase();
    void importPluginWithQmlFile();
    void remoteImportWithQuotedUrl();
    void remoteImportWithUnquotedUri();
    void versionNotInstalled();
    void versionNotInstalled_data();
    void implicitQmldir();
    void implicitQmldir_data();

private:
    QString m_importsDirectory;
};

void tst_qdeclarativemoduleplugin::initTestCase()
{
    QDeclarativeDataTest::initTestCase();
    m_importsDirectory = directory() + QStringLiteral("/imports");
    QVERIFY2(QFileInfo(m_importsDirectory).isDir(),
             qPrintable(QString::fromLatin1("Imports directory '%1' does not exist.").arg(m_importsDirectory)));
}

#define VERIFY_ERRORS(errorfile) \
    if (!errorfile) { \
        if (qgetenv("DEBUG") != "" && !component.errors().isEmpty()) \
            qWarning() << "Unexpected Errors:" << component.errors(); \
        QVERIFY(!component.isError()); \
        QVERIFY(component.errors().isEmpty()); \
    } else { \
        QString verify_errors_file_name = testFile(errorfile); \
        QFile file(verify_errors_file_name); \
        QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text)); \
        QByteArray data = file.readAll(); \
        file.close(); \
        QList<QByteArray> expected = data.split('\n'); \
        expected.removeAll(QByteArray("")); \
        QList<QDeclarativeError> errors = component.errors(); \
        QList<QByteArray> actual; \
        for (int ii = 0; ii < errors.count(); ++ii) { \
            const QDeclarativeError &error = errors.at(ii); \
            QByteArray errorStr = QByteArray::number(error.line()) + ":" +  \
                                  QByteArray::number(error.column()) + ":" + \
                                  error.description().toUtf8(); \
            actual << errorStr; \
        } \
        if (qgetenv("DEBUG") != "" && expected != actual) { \
            qWarning() << "Expected:" << expected << "Actual:" << actual; \
        } \
        if (qgetenv("QDECLARATIVELANGUAGE_UPDATEERRORS") != "" && expected != actual) {\
            QFile file(testFile(errorfile)); \
            QVERIFY(file.open(QIODevice::WriteOnly)); \
            for (int ii = 0; ii < actual.count(); ++ii) { \
                file.write(actual.at(ii)); file.write("\n"); \
            } \
            file.close(); \
        } else { \
            QCOMPARE(expected, actual); \
        } \
    }

void tst_qdeclarativemoduleplugin::importsPlugin()
{
    QDeclarativeEngine engine;
    engine.addImportPath(m_importsDirectory);
    QTest::ignoreMessage(QtWarningMsg, "plugin created");
    QTest::ignoreMessage(QtWarningMsg, "import worked");
    QDeclarativeComponent component(&engine, testFileUrl(QStringLiteral("works.qml")));
    foreach (QDeclarativeError err, component.errors())
    	qWarning() << err;
    VERIFY_ERRORS(0);
    QObject *object = component.create();
    QVERIFY(object != 0);
    QCOMPARE(object->property("value").toInt(),123);
    delete object;
}

void tst_qdeclarativemoduleplugin::importsPlugin2()
{
    QDeclarativeEngine engine;
    engine.addImportPath(m_importsDirectory);
    QTest::ignoreMessage(QtWarningMsg, "plugin2 created");
    QTest::ignoreMessage(QtWarningMsg, "import2 worked");
    QDeclarativeComponent component(&engine, testFileUrl(QStringLiteral("works2.qml")));
    foreach (QDeclarativeError err, component.errors())
        qWarning() << err;
    VERIFY_ERRORS(0);
    QObject *object = component.create();
    QVERIFY(object != 0);
    QCOMPARE(object->property("value").toInt(),123);
    delete object;
}

void tst_qdeclarativemoduleplugin::importsPlugin21()
{
    QDeclarativeEngine engine;
    engine.addImportPath(m_importsDirectory);
    QTest::ignoreMessage(QtWarningMsg, "plugin2.1 created");
    QTest::ignoreMessage(QtWarningMsg, "import2.1 worked");
    QDeclarativeComponent component(&engine, testFileUrl(QStringLiteral("works21.qml")));
    foreach (QDeclarativeError err, component.errors())
        qWarning() << err;
    VERIFY_ERRORS(0);
    QObject *object = component.create();
    QVERIFY(object != 0);
    QCOMPARE(object->property("value").toInt(),123);
    delete object;
}

void tst_qdeclarativemoduleplugin::incorrectPluginCase()
{
    QDeclarativeEngine engine;
    engine.addImportPath(m_importsDirectory);

    QDeclarativeComponent component(&engine, testFileUrl(QStringLiteral("incorrectCase.qml")));

    QList<QDeclarativeError> errors = component.errors();
    QCOMPARE(errors.count(), 1);

#if defined(Q_OS_MAC) || defined(Q_OS_WIN32)
#if defined(Q_OS_MAC)
    QString libname = "libPluGin.dylib";
#elif defined(Q_OS_WIN32)
    QString libname = "PluGin.dll";
#endif
    QString expectedError = QLatin1String("plugin cannot be loaded for module \"com.nokia.WrongCase\": File name case mismatch for \"") + QDir(m_importsDirectory).filePath("com/nokia/WrongCase/" + libname) + QLatin1String("\"");
#else
    QString expectedError = QLatin1String("module \"com.nokia.WrongCase\" plugin \"PluGin\" not found");
#endif

    QCOMPARE(errors.at(0).description(), expectedError);
}

void tst_qdeclarativemoduleplugin::importPluginWithQmlFile()
{
    QString path = m_importsDirectory;

    // QTBUG-16885: adding an import path with a lower-case "c:" causes assert failure
    // (this only happens if the plugin includes pure QML files)
    #ifdef Q_OS_WIN
        QVERIFY(path.at(0).isUpper() && path.at(1) == QLatin1Char(':'));
        path = path.at(0).toLower() + path.mid(1);
    #endif

    QDeclarativeEngine engine;
    engine.addImportPath(path);

    QDeclarativeComponent component(&engine, testFileUrl(QStringLiteral("pluginWithQmlFile.qml")));
    foreach (QDeclarativeError err, component.errors())
        qWarning() << err;
    VERIFY_ERRORS(0);
    QObject *object = component.create();
    QVERIFY(object != 0);
    delete object;
}

void tst_qdeclarativemoduleplugin::remoteImportWithQuotedUrl()
{
    TestHTTPServer server(SERVER_PORT);
    QVERIFY(server.isValid());
    server.serveDirectory(m_importsDirectory);

    QDeclarativeEngine engine;
    QDeclarativeComponent component(&engine);
    component.setData("import \"http://127.0.0.1:14450/com/nokia/PureQmlModule\" \nComponentA { width: 300; ComponentB{} }", QUrl());

    QTRY_COMPARE(component.status(), QDeclarativeComponent::Ready);
    QObject *object = component.create();
    QCOMPARE(object->property("width").toInt(), 300);
    QVERIFY(object != 0);
    delete object;

    foreach (QDeclarativeError err, component.errors())
        qWarning() << err;
    VERIFY_ERRORS(0);
}

void tst_qdeclarativemoduleplugin::remoteImportWithUnquotedUri()
{
    TestHTTPServer server(SERVER_PORT);
    QVERIFY(server.isValid());
    server.serveDirectory(m_importsDirectory);

    QDeclarativeEngine engine;
    engine.addImportPath(m_importsDirectory);
    QDeclarativeComponent component(&engine);
    component.setData("import com.nokia.PureQmlModule 1.0 \nComponentA { width: 300; ComponentB{} }", QUrl());


    QTRY_COMPARE(component.status(), QDeclarativeComponent::Ready);
    QObject *object = component.create();
    QVERIFY(object != 0);
    QCOMPARE(object->property("width").toInt(), 300);
    delete object;

    foreach (QDeclarativeError err, component.errors())
        qWarning() << err;
    VERIFY_ERRORS(0);
}

// QTBUG-17324

void tst_qdeclarativemoduleplugin::importsMixedQmlCppPlugin()
{
    QDeclarativeEngine engine;
    engine.addImportPath(m_importsDirectory);

    {
    QDeclarativeComponent component(&engine, testFileUrl(QStringLiteral("importsMixedQmlCppPlugin.qml")));

    QObject *o = component.create();
    QVERIFY2(o != 0, QDeclarativeDataTest::msgComponentError(component, &engine));
    QCOMPARE(o->property("test").toBool(), true);
    delete o;
    }

    {
    QDeclarativeComponent component(&engine, testFileUrl(QStringLiteral("importsMixedQmlCppPlugin.2.qml")));

    QObject *o = component.create();
    QVERIFY2(o != 0, QDeclarativeDataTest::msgComponentError(component, &engine));
    QCOMPARE(o->property("test").toBool(), true);
    QCOMPARE(o->property("test2").toBool(), true);
    delete o;
    }


}

void tst_qdeclarativemoduleplugin::versionNotInstalled_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("errorFile");

    QTest::newRow("versionNotInstalled") << "versionNotInstalled.qml" << "versionNotInstalled.errors.txt";
    QTest::newRow("versionNotInstalled") << "versionNotInstalled.2.qml" << "versionNotInstalled.2.errors.txt";
}

void tst_qdeclarativemoduleplugin::versionNotInstalled()
{
    QFETCH(QString, file);
    QFETCH(QString, errorFile);

    QDeclarativeEngine engine;
    engine.addImportPath(m_importsDirectory);

    QDeclarativeComponent component(&engine, testFileUrl(file));
    VERIFY_ERRORS(errorFile.toLatin1().constData());
}


// test that errors are reporting correctly for plugin loading and qmldir parsing
void tst_qdeclarativemoduleplugin::implicitQmldir_data()
{
    QTest::addColumn<QString>("directory");
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("errorFile");

    // parsing qmldir succeeds, but plugin specified in the qmldir file doesn't exist
    QTest::newRow("implicitQmldir") << "implicit1" << "temptest.qml" << "implicitQmldir.errors.txt";

    // parsing qmldir fails due to syntax errors, etc.
    QTest::newRow("implicitQmldir2") << "implicit2" << "temptest2.qml" << "implicitQmldir.2.errors.txt";
}
void tst_qdeclarativemoduleplugin::implicitQmldir()
{
    QFETCH(QString, directory);
    QFETCH(QString, file);
    QFETCH(QString, errorFile);

    QString importPath = testFile(directory);
    QString fileName = directory + QDir::separator() + file;
    QString errorFileName = directory + QDir::separator() + errorFile;
    QUrl testUrl = testFileUrl(fileName);

    QDeclarativeEngine engine;
    engine.addImportPath(importPath);

    QDeclarativeComponent component(&engine, testUrl);
    QList<QDeclarativeError> errors = component.errors();
    VERIFY_ERRORS(errorFileName.toLatin1().constData());
    QTest::ignoreMessage(QtWarningMsg, "QDeclarativeComponent: Component is not ready");
    QObject *obj = component.create();
    QVERIFY(!obj);
    delete obj;
}


QTEST_MAIN(tst_qdeclarativemoduleplugin)

#include "tst_qdeclarativemoduleplugin.moc"
