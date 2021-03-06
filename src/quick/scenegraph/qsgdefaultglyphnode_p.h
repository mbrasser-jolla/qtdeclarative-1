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

#ifndef DEFAULT_GLYPHNODE_H
#define DEFAULT_GLYPHNODE_H

#include <private/qsgadaptationlayer_p.h>
#include <QtQuick/qsgnode.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QGlyphs;
class QSGTextMaskMaterial;
class QSGDefaultGlyphNode: public QSGGlyphNode
{
public:
    QSGDefaultGlyphNode();
    ~QSGDefaultGlyphNode();

    virtual QPointF baseLine() const { return m_baseLine; }
    virtual void setGlyphs(const QPointF &position, const QGlyphRun &glyphs);
    virtual void setColor(const QColor &color);

    virtual void setPreferredAntialiasingMode(AntialiasingMode) { }
    virtual void setStyle(QQuickText::TextStyle) { }
    virtual void setStyleColor(const QColor &) { }

    virtual void update() { }

private:
    QGlyphRun m_glyphs;
    QPointF m_position;
    QColor m_color;

    QPointF m_baseLine;
    QSGTextMaskMaterial *m_material;

    QSGGeometry m_geometry;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // DEFAULT_GLYPHNODE_H
