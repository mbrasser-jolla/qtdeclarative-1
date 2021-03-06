/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtScript module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** us via http://www.qt-project.org/.
** $QT_END_LICENSE$
**
****************************************************************************/

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


#ifndef QSCRIPTTOOLS_P_H
#define QSCRIPTTOOLS_P_H

#include <private/qintrusivelist_p.h>

QT_BEGIN_NAMESPACE

template<class N, QIntrusiveListNode N::*member>
class QScriptIntrusiveList : public QIntrusiveList<N, member>
{
public:
    inline void insert(N *n);
    inline void remove(N *n);
};

template<class N, QIntrusiveListNode N::*member>
void QScriptIntrusiveList<N, member>::insert(N *n)
{
    Q_ASSERT_X(!this->contains(n), Q_FUNC_INFO, "Can't insert a value which is in the list already");
    Q_ASSERT_X(!(n->*member).isInList(), Q_FUNC_INFO, "Can't insert a value which is in another list");
    QIntrusiveList<N, member>::insert(n);
}

template<class N, QIntrusiveListNode N::*member>
void QScriptIntrusiveList<N, member>::remove(N *n)
{
    Q_ASSERT_X(this->contains(n), Q_FUNC_INFO, "Can't remove a value which is not in the list");
    QIntrusiveList<N, member>::remove(n);
}

QT_END_NAMESPACE

#endif //QSCRIPTTOOLS_P_H
