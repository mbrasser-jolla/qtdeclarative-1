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

#include "qquickvisualitemmodel_p.h"
#include "qquickitem.h"

#include <QtCore/qcoreapplication.h>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativeengine.h>

#include <private/qdeclarativechangeset_p.h>
#include <private/qdeclarativeglobal_p.h>
#include <private/qobject_p.h>

#include <QtCore/qhash.h>
#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

QHash<QObject*, QQuickVisualItemModelAttached*> QQuickVisualItemModelAttached::attachedProperties;


class QQuickVisualItemModelPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickVisualItemModel)
public:
    QQuickVisualItemModelPrivate() : QObjectPrivate() {}

    static void children_append(QDeclarativeListProperty<QQuickItem> *prop, QQuickItem *item) {
        QDeclarative_setParent_noEvent(item, prop->object);
        static_cast<QQuickVisualItemModelPrivate *>(prop->data)->children.append(Item(item));
        static_cast<QQuickVisualItemModelPrivate *>(prop->data)->itemAppended();
        static_cast<QQuickVisualItemModelPrivate *>(prop->data)->emitChildrenChanged();
    }

    static int children_count(QDeclarativeListProperty<QQuickItem> *prop) {
        return static_cast<QQuickVisualItemModelPrivate *>(prop->data)->children.count();
    }

    static QQuickItem *children_at(QDeclarativeListProperty<QQuickItem> *prop, int index) {
        return static_cast<QQuickVisualItemModelPrivate *>(prop->data)->children.at(index).item;
    }

    void itemAppended() {
        Q_Q(QQuickVisualItemModel);
        QQuickVisualItemModelAttached *attached = QQuickVisualItemModelAttached::properties(children.last().item);
        attached->setIndex(children.count()-1);
        QDeclarativeChangeSet changeSet;
        changeSet.insert(children.count() - 1, 1);
        emit q->modelUpdated(changeSet, false);
        emit q->countChanged();
    }

    void emitChildrenChanged() {
        Q_Q(QQuickVisualItemModel);
        emit q->childrenChanged();
    }

    int indexOf(QQuickItem *item) const {
        for (int i = 0; i < children.count(); ++i)
            if (children.at(i).item == item)
                return i;
        return -1;
    }

    class Item {
    public:
        Item(QQuickItem *i) : item(i), ref(0) {}

        void addRef() { ++ref; }
        bool deref() { return --ref == 0; }

        QQuickItem *item;
        int ref;
    };

    QList<Item> children;
};


/*!
    \qmlclass VisualItemModel QQuickVisualItemModel
    \inqmlmodule QtQuick 2
    \ingroup qml-working-with-data
    \brief The VisualItemModel allows items to be provided to a view.

    A VisualItemModel contains the visual items to be used in a view.
    When a VisualItemModel is used in a view, the view does not require
    a delegate since the VisualItemModel already contains the visual
    delegate (items).

    An item can determine its index within the
    model via the \l{VisualItemModel::index}{index} attached property.

    The example below places three colored rectangles in a ListView.
    \code
    import QtQuick 1.0

    Rectangle {
        VisualItemModel {
            id: itemModel
            Rectangle { height: 30; width: 80; color: "red" }
            Rectangle { height: 30; width: 80; color: "green" }
            Rectangle { height: 30; width: 80; color: "blue" }
        }

        ListView {
            anchors.fill: parent
            model: itemModel
        }
    }
    \endcode

    \image visualitemmodel.png

    \sa {declarative/modelviews/visualitemmodel}{VisualItemModel example}
*/
QQuickVisualItemModel::QQuickVisualItemModel(QObject *parent)
    : QQuickVisualModel(*(new QQuickVisualItemModelPrivate), parent)
{
}

/*!
    \qmlattachedproperty int QtQuick2::VisualItemModel::index
    This attached property holds the index of this delegate's item within the model.

    It is attached to each instance of the delegate.
*/

QDeclarativeListProperty<QQuickItem> QQuickVisualItemModel::children()
{
    Q_D(QQuickVisualItemModel);
    return QDeclarativeListProperty<QQuickItem>(this, d, d->children_append,
                                                      d->children_count, d->children_at);
}

/*!
    \qmlproperty int QtQuick2::VisualItemModel::count

    The number of items in the model.  This property is readonly.
*/
int QQuickVisualItemModel::count() const
{
    Q_D(const QQuickVisualItemModel);
    return d->children.count();
}

bool QQuickVisualItemModel::isValid() const
{
    return true;
}

QQuickItem *QQuickVisualItemModel::item(int index, bool)
{
    Q_D(QQuickVisualItemModel);
    QQuickVisualItemModelPrivate::Item &item = d->children[index];
    item.addRef();
    emit initItem(index, item.item);
    emit createdItem(index, item.item);
    return item.item;
}

QQuickVisualModel::ReleaseFlags QQuickVisualItemModel::release(QQuickItem *item)
{
    Q_D(QQuickVisualItemModel);
    int idx = d->indexOf(item);
    if (idx >= 0) {
        if (d->children[idx].deref()) {
            // XXX todo - the original did item->scene()->removeItem().  Why?
            item->setParentItem(0);
            QDeclarative_setParent_noEvent(item, this);
        }
    }
    return 0;
}

QString QQuickVisualItemModel::stringValue(int index, const QString &name)
{
    Q_D(QQuickVisualItemModel);
    if (index < 0 || index >= d->children.count())
        return QString();
    return QDeclarativeEngine::contextForObject(d->children.at(index).item)->contextProperty(name).toString();
}

int QQuickVisualItemModel::indexOf(QQuickItem *item, QObject *) const
{
    Q_D(const QQuickVisualItemModel);
    return d->indexOf(item);
}

QQuickVisualItemModelAttached *QQuickVisualItemModel::qmlAttachedProperties(QObject *obj)
{
    return QQuickVisualItemModelAttached::properties(obj);
}

QT_END_NAMESPACE

