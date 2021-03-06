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

#ifndef QUICKTEST_H
#define QUICKTEST_H

#include <QtQuickTest/quicktestglobal.h>
#include <QtWidgets/qwidget.h>
#ifdef QT_OPENGL_LIB
#include <QtGui/qopengl.h>
#endif

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

typedef QWidget *(*quick_test_viewport_create)();

Q_QUICK_TEST_EXPORT int quick_test_main(int argc, char **argv, const char *name, quick_test_viewport_create createViewport, const char *sourceDir);

#ifdef QUICK_TEST_SOURCE_DIR

#define QUICK_TEST_MAIN(name) \
    int main(int argc, char **argv) \
    { \
        return quick_test_main(argc, argv, #name, 0, QUICK_TEST_SOURCE_DIR); \
    }

#define QUICK_TEST_OPENGL_MAIN(name) \
    static QWidget *name##_create_viewport() \
    { \
        return new QOpenGLWidget(); \
    } \
    int main(int argc, char **argv) \
    { \
        return quick_test_main(argc, argv, #name, name##_create_viewport, QUICK_TEST_SOURCE_DIR); \
    }

#else

#define QUICK_TEST_MAIN(name) \
    int main(int argc, char **argv) \
    { \
        return quick_test_main(argc, argv, #name, 0, 0); \
    }

#define QUICK_TEST_OPENGL_MAIN(name) \
    static QWidget *name##_create_viewport() \
    { \
        return new QOpenGLWidget(); \
    } \
    int main(int argc, char **argv) \
    { \
        return quick_test_main(argc, argv, #name, name##_create_viewport, 0); \
    }

#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif
