/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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


import QtQuick 2.0
import "widgets"

Rectangle {
    id: page
    width: 640
    height: 480
    color: "white"
    Rectangle {
        id: header
        color: "#c0c0c0"
        height: usage.height + chkClip.height
        anchors.left: parent.left
        anchors.right: parent.right
        Text {
            id: usage
            text: "Use an a11y inspect tool to see if all visible rectangles can be found with hit testing."
        }
        Rectangle {
            id: chkClip
            property bool checked: true

            color: (checked ? "#f0f0f0" : "#c0c0c0")
            height: label.height
            width: label.width
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            MouseArea {
                anchors.fill: parent
                onClicked: chkClip.checked = !chkClip.checked
            }
            Text {
                id: label
                text: "Click here to toggle clipping"
            }
        }
    }
    TextRect {
        clip: chkClip.checked
        z: 2
        id: rect1
        text: "rect1"
        width: 100
        height: 100
        color: "#ffc0c0"
        anchors.top: header.bottom
        TextRect {
            id: rect10
            text: "rect10"
            width: 100
            height: 100
            x: 50
            y: 50
            color: "#ffa0a0"
            TextRect {
                id: rect100
                text: "rect100"
                width: 100
                height: 100
                x: 80
                y: 80
                color: "#ff8080"
            }
            TextRect {
                id: rect101
                text: "rect101"
                x: 100
                y: 70
                z: 3
                width: 100
                height: 100
                color: "#e06060"
            }
            TextRect {
                id: rect102
                text: "rect102"
                width: 100
                height: 100
                x: 150
                y: 60
                color: "#c04040"
            }
        }
    }

    TextRect {
        x: 0
        y: 50
        id: rect2
        text: "rect2"
        width: 100
        height: 100
        color: "#c0c0ff"
        TextRect {
            id: rect20
            text: "rect20"
            width: 100
            height: 100
            x: 50
            y: 50
            color: "#a0a0ff"
            TextRect {
                id: rect200
                text: "rect200"
                width: 100
                height: 100
                x: 80
                y: 80
                color: "#8080ff"
            }
            TextRect {
                id: rect201
                text: "rect201"
                x: 100
                y: 70
                z: 100
                width: 100
                height: 100
                color: "#6060e0"
            }
            TextRect {
                id: rect202
                text: "rect202"
                width: 100
                height: 100
                x: 150
                y: 60
                color: "#4040c0"
            }
        }
    }

}
