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
    id: keyselementtest
    anchors.fill: parent
    property string testtext: ""
    property int sidelength: 1500
    focus: true

    Rectangle {
        id: keyselementbox
        color: "lightgray"; border.color: "gray"; radius: 5; clip: true; opacity: .7
        height: 250; width: parent.width *.8
        anchors.centerIn: parent
        focus: true
        Keys.enabled: true
        Keys.onLeftPressed: { if (statenum == 2) { advance(); } }
        Keys.onRightPressed: { if (statenum == 1) { advance(); } }
        Keys.onUpPressed: { if (statenum == 3) { advance(); } }
        Keys.onDownPressed: { if (statenum == 4) { advance(); } }
        Keys.onPressed: {
            if ((event.key == Qt.Key_Space) && (event.modifiers & Qt.ControlModifier) && (event.modifiers & Qt.AltModifier)) {
                if (statenum == 5) { advance(); }
            }
        }
        Rectangle { id: leftone; height: 50; width: 50; color: "green"; anchors{ left: parent.left; verticalCenter: parent.verticalCenter } }
        Rectangle { id: rightone; height: 50; width: 50; color: "green"; anchors{ right: parent.right; verticalCenter: parent.verticalCenter } }
        Rectangle { id: topone; height: 50; width: 50; color: "green"; anchors{ top: parent.top; horizontalCenter: parent.horizontalCenter } }
        Rectangle { id: bottomone; height: 50; width: 50; color: "green"; anchors{ bottom: parent.bottom; horizontalCenter: parent.horizontalCenter } }
    }

    SystemTestHelp { id: helpbubble; visible: statenum != 0
        anchors { top: parent.top; horizontalCenter: parent.horizontalCenter; topMargin: 50 }
    }
    BugPanel { id: bugpanel }

    states: [
        State { name: "start"; when: statenum == 1
            PropertyChanges { target: helpbubble; showadvance: false }
            PropertyChanges { target: keyselementtest
                testtext: "This is a Keys element. At present there should be four squares\n"+
                "Next, please press the right button on the keypad/board" }
        },
        State { name: "right"; when: statenum == 2
            PropertyChanges { target: helpbubble; showadvance: false }
            PropertyChanges { target: rightone; color: "orange" }
            PropertyChanges { target: keyselementtest; testtext: "Good. Now press left." }
        },
        State { name: "left"; when: statenum == 3
            PropertyChanges { target: helpbubble; showadvance: false }
            PropertyChanges { target: leftone; color: "orange" }
            PropertyChanges { target: keyselementtest; testtext: "Press up." }
        },
        State { name: "up"; when: statenum == 4
            PropertyChanges { target: helpbubble; showadvance: false }
            PropertyChanges { target: topone; color: "orange" }
            PropertyChanges { target: keyselementtest; testtext: "And then press down" }
        },
        State { name: "down"; when: statenum == 5
            PropertyChanges { target: helpbubble; showadvance: false }
            PropertyChanges { target: bottomone; color: "orange" }
            PropertyChanges { target: keyselementtest; testtext: "Excellent. Now hold Ctrl+Alt and press Space" }
        },
        State { name: "modifiers"; when: statenum == 6
            PropertyChanges { target: helpbubble; showadvance: true }
            PropertyChanges { target: keyselementbox; color: "orange" }
            PropertyChanges { target: keyselementtest
                testtext: "Test has completed\n"+
                "Advance to restart the test." }
        }
    ]

}
