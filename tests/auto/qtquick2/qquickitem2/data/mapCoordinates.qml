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
    id: root; objectName: "root"
    width: 200; height: 200

    Item { id: itemA; objectName: "itemA"; x: 50; y: 50 }

    Item {
        x: 50; y: 50
        Item { id: itemB; objectName: "itemB"; x: 100; y: 100 }
    }

    function mapAToB(x, y) {
        var pos = itemA.mapToItem(itemB, x, y)
        return Qt.point(pos.x, pos.y)
    }

    function mapAFromB(x, y) {
        var pos = itemA.mapFromItem(itemB, x, y)
        return Qt.point(pos.x, pos.y)
    }

    function mapAToNull(x, y) {
        var pos = itemA.mapToItem(null, x, y)
        return Qt.point(pos.x, pos.y)
    }

    function mapAFromNull(x, y) {
        var pos = itemA.mapFromItem(null, x, y)
        return Qt.point(pos.x, pos.y)
    }

    function checkMapAToInvalid(x, y) {
        var pos = itemA.mapToItem(1122, x, y)
        return pos == undefined;
    }

    function checkMapAFromInvalid(x, y) {
        var pos = itemA.mapFromItem(1122, x, y)
        return pos == undefined;
    }
}
