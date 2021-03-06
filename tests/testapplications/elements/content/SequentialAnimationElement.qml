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

import QtQuick 2.0

Item {
    id: sequentialanimationelementtest
    anchors.fill: parent
    property string testtext: ""
    property int firstduration: 1000
    property int secondduration: 3000
    SequentialAnimation {
        id: sequentialanimationelement
        running: false
        NumberAnimation { id: movement; target: animatedrect; properties: "y"; to: 500; duration: firstduration }
        ColorAnimation { id: recolor; target: animatedrect; properties: "color"; to: "green"; duration: secondduration }
    }

    Rectangle {
        id: animatedrect
        width: 50; height: 50; color: "blue"; y: 300
        anchors.horizontalCenter: parent.horizontalCenter
    }

    SystemTestHelp { id: helpbubble; visible: statenum != 0
        anchors { top: parent.top; horizontalCenter: parent.horizontalCenter; topMargin: 50 }
    }
    BugPanel { id: bugpanel }

    states: [
        State { name: "start"; when: statenum == 1
            PropertyChanges { target: animatedrect; color: "blue"; y: 300 }
            PropertyChanges { target: sequentialanimationelementtest
                testtext: "This square will be animated in a sequence.\n"+
                "The next step will see it move quickly down the display, then slowly change its color to green";
            }
        },
        State { name: "firstchange"; when: statenum == 2
            PropertyChanges { target: sequentialanimationelement; running: true }
            PropertyChanges { target: sequentialanimationelementtest
                testtext: "The square should have moved quickly and also recolored slowly\n"+
                "Next, it will recolor quickly and move slowly back to it's original position at the same time"
            }
        },
        State { name: "secondchange"; when: statenum == 3
            StateChangeScript { script: { firstduration = 3000; secondduration = 1000 } }
            PropertyChanges { target: movement; to: 300 }
            PropertyChanges { target: recolor; to: "blue" }
            PropertyChanges { target: sequentialanimationelement; running: true }
            PropertyChanges { target: sequentialanimationelementtest
                testtext: "The square should have moved slowly and also recolored quickly\n"+
                "Advance to restart the test"
            }
        }
    ]
}
