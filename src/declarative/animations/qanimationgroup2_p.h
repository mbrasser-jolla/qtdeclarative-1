/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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

#ifndef QANIMATIONGROUP2_P_H
#define QANIMATIONGROUP2_P_H

#include "private/qabstractanimation2_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Declarative)

class Q_DECLARATIVE_EXPORT QAnimationGroup2 : public QAbstractAnimation2
{
    Q_DISABLE_COPY(QAnimationGroup2)
public:
    QAnimationGroup2();
    ~QAnimationGroup2();

    void appendAnimation(QAbstractAnimation2 *animation);
    void prependAnimation(QAbstractAnimation2 *animation);
    void removeAnimation(QAbstractAnimation2 *animation);

    QAbstractAnimation2 *firstChild() const { return m_firstChild; }
    QAbstractAnimation2 *lastChild() const { return m_lastChild; }

    void clear();

    //called by QAbstractAnimation2
    virtual void uncontrolledAnimationFinished(QAbstractAnimation2 *animation);
protected:
    void topLevelAnimationLoopChanged();

    virtual void animationInserted(QAbstractAnimation2*) { }
    virtual void animationRemoved(QAbstractAnimation2*, QAbstractAnimation2*, QAbstractAnimation2*);

    //TODO: confirm location of these (should any be moved into QAbstractAnimation2?)
    void resetUncontrolledAnimationsFinishTime();
    void resetUncontrolledAnimationFinishTime(QAbstractAnimation2 *anim);
    int uncontrolledAnimationFinishTime(QAbstractAnimation2 *anim) const { return anim->m_uncontrolledFinishTime; }
    void setUncontrolledAnimationFinishTime(QAbstractAnimation2 *anim, int time);

private:
    //definition
    QAbstractAnimation2 *m_firstChild;
    QAbstractAnimation2 *m_lastChild;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif //QANIMATIONGROUP2_P_H
