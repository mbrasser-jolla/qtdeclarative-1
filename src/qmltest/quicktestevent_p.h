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

#ifndef QUICKTESTEVENT_P_H
#define QUICKTESTEVENT_P_H

#include <QtQuickTest/quicktestglobal.h>
#include <QtCore/qobject.h>
#include <QtGui/QWindow>
QT_BEGIN_NAMESPACE

class Q_QUICK_TEST_EXPORT QuickTestEvent : public QObject
{
    Q_OBJECT
public:
    QuickTestEvent(QObject *parent = 0);
    ~QuickTestEvent();

public Q_SLOTS:
    bool keyPress(int key, int modifiers, int delay);
    bool keyRelease(int key, int modifiers, int delay);
    bool keyClick(int key, int modifiers, int delay);

    bool mousePress(QObject *item, qreal x, qreal y, int button,
                    int modifiers, int delay);
    bool mouseRelease(QObject *item, qreal x, qreal y, int button,
                      int modifiers, int delay);
    bool mouseClick(QObject *item, qreal x, qreal y, int button,
                    int modifiers, int delay);
    bool mouseDoubleClick(QObject *item, qreal x, qreal y, int button,
                          int modifiers, int delay);
    bool mouseMove(QObject *item, qreal x, qreal y, int delay, int buttons);

    bool mouseWheel(QObject *item, qreal x, qreal y, int buttons,
               int modifiers, int delta, int delay, int orientation);

private:
    QWindow *eventWindow();
};

QT_END_NAMESPACE

#endif
