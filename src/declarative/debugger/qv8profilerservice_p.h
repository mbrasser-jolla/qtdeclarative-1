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

#ifndef QV8PROFILERSERVICE_P_H
#define QV8PROFILERSERVICE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qdeclarativedebugservice_p.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


struct Q_AUTOTEST_EXPORT QV8ProfilerData
{
    int messageType;
    QString filename;
    QString functionname;
    int lineNumber;
    double totalTime;
    double selfTime;
    int treeLevel;

    QByteArray toByteArray() const;
};

class QDeclarativeEngine;
class QV8ProfilerServicePrivate;

class Q_AUTOTEST_EXPORT QV8ProfilerService : public QDeclarativeDebugService
{
    Q_OBJECT
public:
    enum MessageType {
        V8Entry,
        V8Complete,
        V8SnapshotChunk,
        V8SnapshotComplete,

        V8MaximumMessage
    };

    QV8ProfilerService(QObject *parent = 0);
    ~QV8ProfilerService();

    static QV8ProfilerService *instance();
    static void initialize();

public slots:
    void startProfiling(const QString &title);
    void stopProfiling(const QString &title);
    void takeSnapshot();
    void deleteSnapshots();

    void sendProfilingData();

protected:
    void messageReceived(const QByteArray &);

private:
    Q_DISABLE_COPY(QV8ProfilerService)
    Q_DECLARE_PRIVATE(QV8ProfilerService)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QV8PROFILERSERVICE_P_H
