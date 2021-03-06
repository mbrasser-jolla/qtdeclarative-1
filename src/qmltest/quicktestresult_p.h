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

#ifndef QUICKTESTRESULT_P_H
#define QUICKTESTRESULT_P_H

#include <QtQuickTest/quicktestglobal.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qscopedpointer.h>

QT_BEGIN_NAMESPACE

class QUrl;
class QuickTestResultPrivate;

class Q_QUICK_TEST_EXPORT QuickTestResult : public QObject
{
    Q_OBJECT
    Q_ENUMS(FunctionType RunMode)
    Q_PROPERTY(QString testCaseName READ testCaseName WRITE setTestCaseName NOTIFY testCaseNameChanged)
    Q_PROPERTY(QString functionName READ functionName WRITE setFunctionName NOTIFY functionNameChanged)
    Q_PROPERTY(FunctionType functionType READ functionType WRITE setFunctionType NOTIFY functionTypeChanged)
    Q_PROPERTY(QString dataTag READ dataTag WRITE setDataTag NOTIFY dataTagChanged)
    Q_PROPERTY(bool failed READ isFailed)
    Q_PROPERTY(bool dataFailed READ isDataFailed)
    Q_PROPERTY(bool skipped READ isSkipped WRITE setSkipped NOTIFY skippedChanged)
    Q_PROPERTY(int passCount READ passCount)
    Q_PROPERTY(int failCount READ failCount)
    Q_PROPERTY(int skipCount READ skipCount)
    Q_PROPERTY(QStringList functionsToRun READ functionsToRun)
public:
    QuickTestResult(QObject *parent = 0);
    ~QuickTestResult();

    // Values must match QTestResult::TestLocation.
    enum FunctionType
    {
        NoWhere = 0,
        DataFunc = 1,
        InitFunc = 2,
        Func = 3,
        CleanupFunc = 4
    };

    // Values must match QBenchmarkIterationController::RunMode.
    enum RunMode
    {
        RepeatUntilValidMeasurement,
        RunOnce
    };

    QString testCaseName() const;
    void setTestCaseName(const QString &name);

    QString functionName() const;
    void setFunctionName(const QString &name);

    FunctionType functionType() const;
    void setFunctionType(FunctionType type);

    QString dataTag() const;
    void setDataTag(const QString &tag);

    bool isFailed() const;
    bool isDataFailed() const;

    bool isSkipped() const;
    void setSkipped(bool skip);

    int passCount() const;
    int failCount() const;
    int skipCount() const;

    QStringList functionsToRun() const;

public Q_SLOTS:
    void reset();

    void startLogging();
    void stopLogging();

    void initTestTable();
    void clearTestTable();

    void finishTestFunction();

    void fail(const QString &message, const QUrl &location, int line);
    bool verify(bool success, const QString &message,
                const QUrl &location, int line);
    bool compare(bool success, const QString &message,
                 const QString &val1, const QString &val2,
                 const QUrl &location, int line);
    void skip(const QString &message, const QUrl &location, int line);
    bool expectFail(const QString &tag, const QString &comment,
                    const QUrl &location, int line);
    bool expectFailContinue(const QString &tag, const QString &comment,
                            const QUrl &location, int line);
    void warn(const QString &message, const QUrl &location, int line);

    void ignoreWarning(const QString &message);

    void wait(int ms);
    void sleep(int ms);

    void startMeasurement();
    void beginDataRun();
    void endDataRun();
    bool measurementAccepted();
    bool needsMoreMeasurements();

    void startBenchmark(RunMode runMode, const QString &tag);
    bool isBenchmarkDone() const;
    void nextBenchmark();
    void stopBenchmark();

public:
    // Helper functions for the C++ main() shell.
    static void parseArgs(int argc, char *argv[]);
    static void setProgramName(const char *name);
    static int exitCode();

Q_SIGNALS:
    void programNameChanged();
    void testCaseNameChanged();
    void functionNameChanged();
    void functionTypeChanged();
    void dataTagChanged();
    void skippedChanged();

private:
    QScopedPointer<QuickTestResultPrivate> d_ptr;

    Q_DECLARE_PRIVATE(QuickTestResult)
    Q_DISABLE_COPY(QuickTestResult)
};

QT_END_NAMESPACE

#endif
