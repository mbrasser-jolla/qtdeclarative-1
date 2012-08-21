/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <qtest.h>
#include <QtQml/qqmlengine.h>
#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlproperty.h>
#include <QtQml/private/qqmlproperty_p.h>
#include <private/qqmlbinding_p.h>
#include <private/qqmlboundsignal_p.h>
#include <QtWidgets/QLineEdit>
#include <QtCore/qfileinfo.h>
#include <QtCore/qdir.h>
#include "../../shared/util.h"

#include <QDebug>
class MyQmlObject : public QObject
{
    Q_OBJECT
public:
    MyQmlObject() {}
};

QML_DECLARE_TYPE(MyQmlObject);

class MyAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int foo READ foo WRITE setFoo)
public:
    MyAttached(QObject *parent) : QObject(parent), m_foo(13) {}

    int foo() const { return m_foo; }
    void setFoo(int f) { m_foo = f; }

private:
    int m_foo;
};

class MyContainer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<MyQmlObject> children READ children)
public:
    MyContainer() {}

    QQmlListProperty<MyQmlObject> children() { return QQmlListProperty<MyQmlObject>(this, m_children); }

    static MyAttached *qmlAttachedProperties(QObject *o) {
        return new MyAttached(o);
    }

private:
    QList<MyQmlObject*> m_children;
};

QML_DECLARE_TYPE(MyContainer);
QML_DECLARE_TYPEINFO(MyContainer, QML_HAS_ATTACHED_PROPERTIES)

class tst_qqmlproperty : public QQmlDataTest
{
    Q_OBJECT
public:
    tst_qqmlproperty() {}

private slots:
    void initTestCase();

    // Constructors
    void qmlmetaproperty();
    void qmlmetaproperty_object();
    void qmlmetaproperty_object_string();
    void qmlmetaproperty_object_context();
    void qmlmetaproperty_object_string_context();

    // Methods
    void name();
    void read();
    void write();
    void reset();

    // Functionality
    void writeObjectToList();
    void writeListToList();

    //writeToReadOnly();

    void urlHandling_data();
    void urlHandling();

    void variantMapHandling_data();
    void variantMapHandling();

    // Bugs
    void crashOnValueProperty();
    void aliasPropertyBindings();
    void noContext();
    void assignEmptyVariantMap();
    void warnOnInvalidBinding();
    void registeredCompositeTypeProperty();

    void copy();
private:
    QQmlEngine engine;
};

void tst_qqmlproperty::qmlmetaproperty()
{
    QQmlProperty prop;

    QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
    QVERIFY(binding != 0);
    QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
    QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
    QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

    QObject *obj = new QObject;

    QCOMPARE(prop.name(), QString());
    QCOMPARE(prop.read(), QVariant());
    QCOMPARE(prop.write(QVariant()), false);
    QCOMPARE(prop.hasNotifySignal(), false);
    QCOMPARE(prop.needsNotifySignal(), false);
    QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
    QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
    QCOMPARE(prop.connectNotifySignal(obj, 0), false);
    QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
    QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
    QCOMPARE(prop.connectNotifySignal(obj, -1), false);
    QVERIFY(!prop.method().isValid());
    QCOMPARE(prop.type(), QQmlProperty::Invalid);
    QCOMPARE(prop.isProperty(), false);
    QCOMPARE(prop.isWritable(), false);
    QCOMPARE(prop.isDesignable(), false);
    QCOMPARE(prop.isResettable(), false);
    QCOMPARE(prop.isSignalProperty(), false);
    QCOMPARE(prop.isValid(), false);
    QCOMPARE(prop.object(), (QObject *)0);
    QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::InvalidCategory);
    QCOMPARE(prop.propertyType(), 0);
    QCOMPARE(prop.propertyTypeName(), (const char *)0);
    QVERIFY(prop.property().name() == 0);
    QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
    QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
    QVERIFY(binding == 0);
    QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
    QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
    QVERIFY(sigExprWatcher.wasDeleted());
    QCOMPARE(prop.index(), -1);
    QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

    delete obj;
}

// 1 = equal, 0 = unknown, -1 = not equal.
static int compareVariantAndListReference(const QVariant &v, QQmlListReference &r)
{
    if (QLatin1String(v.typeName()) != QLatin1String("QQmlListReference"))
        return -1;

    QQmlListReference lhs = v.value<QQmlListReference>();
    if (lhs.isValid() != r.isValid())
        return -1;

    if (lhs.canCount() != r.canCount())
        return -1;

    if (!lhs.canCount()) {
        if (lhs.canAt() != r.canAt())
            return -1; // not equal.
        return 0; // not sure if they're equal or not, and no way to tell.
    }

    // if we get here, we must be able to count.
    if (lhs.count() != r.count())
        return -1;

    if (lhs.canAt() != r.canAt())
        return -1;

    if (!lhs.canAt())
        return 0; // can count, but can't check element equality.

    for (int i = 0; i < lhs.count(); ++i) {
        if (lhs.at(i) != r.at(i)) {
            return -1; // different elements :. not equal.
        }
    }

    return 1; // equal.
}

void tst_qqmlproperty::registeredCompositeTypeProperty()
{
    // Composite type properties
    {
        QQmlEngine engine;
        QQmlComponent component(&engine, testFileUrl("registeredCompositeTypeProperty.qml"));
        QObject *obj = component.create();
        QVERIFY(obj);

        // create property accessors and check types.
        QQmlProperty p1(obj, "first");
        QQmlProperty p2(obj, "second");
        QQmlProperty p3(obj, "third");
        QQmlProperty p1e(obj, "first", &engine);
        QQmlProperty p2e(obj, "second", &engine);
        QQmlProperty p3e(obj, "third", &engine);
        QVERIFY(p1.propertyType() == p2.propertyType());
        QVERIFY(p1.propertyType() != p3.propertyType());

        // check that the values are retrievable from CPP
        QVariant first = obj->property("first");
        QVariant second = obj->property("second");
        QVariant third = obj->property("third");
        QVERIFY(first.isValid());
        QVERIFY(second.isValid());
        QVERIFY(third.isValid());
        // ensure that conversion from qobject-derived-ptr to qobject-ptr works.
        QVERIFY(first.value<QObject*>());
        QVERIFY(second.value<QObject*>());
        QVERIFY(third.value<QObject*>());

        // check that the values retrieved via QQmlProperty match those retrieved via QMetaProperty::read().
        QCOMPARE(p1.read().value<QObject*>(), first.value<QObject*>());
        QCOMPARE(p2.read().value<QObject*>(), second.value<QObject*>());
        QCOMPARE(p3.read().value<QObject*>(), third.value<QObject*>());
        QCOMPARE(p1e.read().value<QObject*>(), first.value<QObject*>());
        QCOMPARE(p2e.read().value<QObject*>(), second.value<QObject*>());
        QCOMPARE(p3e.read().value<QObject*>(), third.value<QObject*>());

        delete obj;
    }

    // List-of-composite-type type properties
    {
        QQmlEngine engine;
        QQmlComponent component(&engine, testFileUrl("registeredCompositeTypeProperty.qml"));
        QObject *obj = component.create();
        QVERIFY(obj);

        // create list property accessors and check types
        QQmlProperty lp1e(obj, "fclist", &engine);
        QQmlProperty lp2e(obj, "sclistOne", &engine);
        QQmlProperty lp3e(obj, "sclistTwo", &engine);
        QVERIFY(lp1e.propertyType() != lp2e.propertyType());
        QVERIFY(lp2e.propertyType() == lp3e.propertyType());

        // check that the list values are retrievable from CPP
        QVariant firstList = obj->property("fclist");
        QVariant secondList = obj->property("sclistOne");
        QVariant thirdList = obj->property("sclistTwo");
        QVERIFY(firstList.isValid());
        QVERIFY(secondList.isValid());
        QVERIFY(thirdList.isValid());

        // check that the value returned by QQmlProperty::read() is equivalent to the list reference.
        QQmlListReference r1(obj, "fclist", &engine);
        QQmlListReference r2(obj, "sclistOne", &engine);
        QQmlListReference r3(obj, "sclistTwo", &engine);
        QCOMPARE(compareVariantAndListReference(lp1e.read(), r1), 1);
        QCOMPARE(compareVariantAndListReference(lp2e.read(), r2), 1);
        QCOMPARE(compareVariantAndListReference(lp3e.read(), r3), 1);

        delete obj;
    }
}

class PropertyObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int defaultProperty READ defaultProperty)
    Q_PROPERTY(QRect rectProperty READ rectProperty)
    Q_PROPERTY(QRect wrectProperty READ wrectProperty WRITE setWRectProperty)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QVariantMap variantMap READ variantMap WRITE setVariantMap)
    Q_PROPERTY(int resettableProperty READ resettableProperty WRITE setResettableProperty RESET resetProperty)
    Q_PROPERTY(int propertyWithNotify READ propertyWithNotify WRITE setPropertyWithNotify NOTIFY oddlyNamedNotifySignal)
    Q_PROPERTY(MyQmlObject *qmlObject READ qmlObject)

    Q_CLASSINFO("DefaultProperty", "defaultProperty")
public:
    PropertyObject() : m_resetProperty(9) {}

    int defaultProperty() { return 10; }
    QRect rectProperty() { return QRect(10, 10, 1, 209); }

    QRect wrectProperty() { return m_rect; }
    void setWRectProperty(const QRect &r) { m_rect = r; }

    QUrl url() { return m_url; }
    void setUrl(const QUrl &u) { m_url = u; }

    QVariantMap variantMap() const { return m_variantMap; }
    void setVariantMap(const QVariantMap &variantMap) { m_variantMap = variantMap; }

    int resettableProperty() const { return m_resetProperty; }
    void setResettableProperty(int r) { m_resetProperty = r; }
    void resetProperty() { m_resetProperty = 9; }

    int propertyWithNotify() const { return m_propertyWithNotify; }
    void setPropertyWithNotify(int i) { m_propertyWithNotify = i; emit oddlyNamedNotifySignal(); }

    MyQmlObject *qmlObject() { return &m_qmlObject; }

signals:
    void clicked();
    void oddlyNamedNotifySignal();

private:
    int m_resetProperty;
    QRect m_rect;
    QUrl m_url;
    QVariantMap m_variantMap;
    int m_propertyWithNotify;
    MyQmlObject m_qmlObject;
};

QML_DECLARE_TYPE(PropertyObject);

void tst_qqmlproperty::qmlmetaproperty_object()
{
    QObject object; // Has no default property
    PropertyObject dobject; // Has default property

    {
        QQmlProperty prop(&object);

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString());
        QCOMPARE(prop.read(), QVariant());
        QCOMPARE(prop.write(QVariant()), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), false);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QVERIFY(!prop.method().isValid());
        QCOMPARE(prop.type(), QQmlProperty::Invalid);
        QCOMPARE(prop.isProperty(), false);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), false);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), false);
        QCOMPARE(prop.isValid(), false);
        QCOMPARE(prop.object(), (QObject *)0);
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::InvalidCategory);
        QCOMPARE(prop.propertyType(), 0);
        QCOMPARE(prop.propertyTypeName(), (const char *)0);
        QVERIFY(prop.property().name() == 0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding == 0);
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(sigExprWatcher.wasDeleted());
        QCOMPARE(prop.index(), -1);
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }

    {
        QQmlProperty prop(&dobject);

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        static_cast<QQmlBinding *>(binding.data())->setTarget(prop);
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString("defaultProperty"));
        QCOMPARE(prop.read(), QVariant(10));
        QCOMPARE(prop.write(QVariant()), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), true);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QVERIFY(!prop.method().isValid());
        QCOMPARE(prop.type(), QQmlProperty::Property);
        QCOMPARE(prop.isProperty(), true);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), true);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), false);
        QCOMPARE(prop.isValid(), true);
        QCOMPARE(prop.object(), qobject_cast<QObject*>(&dobject));
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::Normal);
        QCOMPARE(prop.propertyType(), (int)QVariant::Int);
        QCOMPARE(prop.propertyTypeName(), "int");
        QCOMPARE(QString(prop.property().name()), QString("defaultProperty"));
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QTest::ignoreMessage(QtWarningMsg, "<Unknown File>: Unable to assign null to int");
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding != 0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == binding.data());
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(sigExprWatcher.wasDeleted());
        QCOMPARE(prop.index(), dobject.metaObject()->indexOfProperty("defaultProperty"));
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }
}

void tst_qqmlproperty::qmlmetaproperty_object_string()
{
    QObject object;
    PropertyObject dobject;

    {
        QQmlProperty prop(&object, QString("defaultProperty"));

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString());
        QCOMPARE(prop.read(), QVariant());
        QCOMPARE(prop.write(QVariant()), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), false);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QVERIFY(!prop.method().isValid());
        QCOMPARE(prop.type(), QQmlProperty::Invalid);
        QCOMPARE(prop.isProperty(), false);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), false);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), false);
        QCOMPARE(prop.isValid(), false);
        QCOMPARE(prop.object(), (QObject *)0);
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::InvalidCategory);
        QCOMPARE(prop.propertyType(), 0);
        QCOMPARE(prop.propertyTypeName(), (const char *)0);
        QVERIFY(prop.property().name() == 0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding == 0);
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(sigExprWatcher.wasDeleted());
        QCOMPARE(prop.index(), -1);
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }

    {
        QQmlProperty prop(&dobject, QString("defaultProperty"));

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        static_cast<QQmlBinding *>(binding.data())->setTarget(prop);
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString("defaultProperty"));
        QCOMPARE(prop.read(), QVariant(10));
        QCOMPARE(prop.write(QVariant()), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), true);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QVERIFY(!prop.method().isValid());
        QCOMPARE(prop.type(), QQmlProperty::Property);
        QCOMPARE(prop.isProperty(), true);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), true);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), false);
        QCOMPARE(prop.isValid(), true);
        QCOMPARE(prop.object(), qobject_cast<QObject*>(&dobject));
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::Normal);
        QCOMPARE(prop.propertyType(), (int)QVariant::Int);
        QCOMPARE(prop.propertyTypeName(), "int");
        QCOMPARE(QString(prop.property().name()), QString("defaultProperty"));
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QTest::ignoreMessage(QtWarningMsg, "<Unknown File>: Unable to assign null to int");
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding != 0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == binding.data());
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(sigExprWatcher.wasDeleted());
        QCOMPARE(prop.index(), dobject.metaObject()->indexOfProperty("defaultProperty"));
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }

    {
        QQmlProperty prop(&dobject, QString("onClicked"));

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        static_cast<QQmlBinding *>(binding.data())->setTarget(prop);
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString("onClicked"));
        QCOMPARE(prop.read(), QVariant());
        QCOMPARE(prop.write(QVariant("Hello")), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), false);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QCOMPARE(QString(prop.method().methodSignature()), QString("clicked()"));
        QCOMPARE(prop.type(), QQmlProperty::SignalProperty);
        QCOMPARE(prop.isProperty(), false);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), false);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), true);
        QCOMPARE(prop.isValid(), true);
        QCOMPARE(prop.object(), qobject_cast<QObject*>(&dobject));
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::InvalidCategory);
        QCOMPARE(prop.propertyType(), 0);
        QCOMPARE(prop.propertyTypeName(), (const char *)0);
        QCOMPARE(prop.property().name(), (const char *)0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding == 0);
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(!sigExprWatcher.wasDeleted());
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == sigExpr);
        QCOMPARE(prop.index(), dobject.metaObject()->indexOfMethod("clicked()"));
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }

    {
        QQmlProperty prop(&dobject, QString("onPropertyWithNotifyChanged"));

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        static_cast<QQmlBinding *>(binding.data())->setTarget(prop);
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString("onOddlyNamedNotifySignal"));
        QCOMPARE(prop.read(), QVariant());
        QCOMPARE(prop.write(QVariant("Hello")), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), false);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QCOMPARE(QString(prop.method().methodSignature()), QString("oddlyNamedNotifySignal()"));
        QCOMPARE(prop.type(), QQmlProperty::SignalProperty);
        QCOMPARE(prop.isProperty(), false);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), false);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), true);
        QCOMPARE(prop.isValid(), true);
        QCOMPARE(prop.object(), qobject_cast<QObject*>(&dobject));
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::InvalidCategory);
        QCOMPARE(prop.propertyType(), 0);
        QCOMPARE(prop.propertyTypeName(), (const char *)0);
        QCOMPARE(prop.property().name(), (const char *)0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding == 0);
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(!sigExprWatcher.wasDeleted());
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == sigExpr);
        QCOMPARE(prop.index(), dobject.metaObject()->indexOfMethod("oddlyNamedNotifySignal()"));
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }
}

void tst_qqmlproperty::qmlmetaproperty_object_context()
{
    QObject object; // Has no default property
    PropertyObject dobject; // Has default property

    {
        QQmlProperty prop(&object, engine.rootContext());

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString());
        QCOMPARE(prop.read(), QVariant());
        QCOMPARE(prop.write(QVariant()), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), false);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QVERIFY(!prop.method().isValid());
        QCOMPARE(prop.type(), QQmlProperty::Invalid);
        QCOMPARE(prop.isProperty(), false);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), false);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), false);
        QCOMPARE(prop.isValid(), false);
        QCOMPARE(prop.object(), (QObject *)0);
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::InvalidCategory);
        QCOMPARE(prop.propertyType(), 0);
        QCOMPARE(prop.propertyTypeName(), (const char *)0);
        QVERIFY(prop.property().name() == 0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding == 0);
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(sigExprWatcher.wasDeleted());
        QCOMPARE(prop.index(), -1);
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }

    {
        QQmlProperty prop(&dobject, engine.rootContext());

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        static_cast<QQmlBinding *>(binding.data())->setTarget(prop);
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString("defaultProperty"));
        QCOMPARE(prop.read(), QVariant(10));
        QCOMPARE(prop.write(QVariant()), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), true);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QVERIFY(!prop.method().isValid());
        QCOMPARE(prop.type(), QQmlProperty::Property);
        QCOMPARE(prop.isProperty(), true);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), true);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), false);
        QCOMPARE(prop.isValid(), true);
        QCOMPARE(prop.object(), qobject_cast<QObject*>(&dobject));
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::Normal);
        QCOMPARE(prop.propertyType(), (int)QVariant::Int);
        QCOMPARE(prop.propertyTypeName(), "int");
        QCOMPARE(QString(prop.property().name()), QString("defaultProperty"));
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QTest::ignoreMessage(QtWarningMsg, "<Unknown File>: Unable to assign null to int");
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding != 0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == binding.data());
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(sigExprWatcher.wasDeleted());
        QCOMPARE(prop.index(), dobject.metaObject()->indexOfProperty("defaultProperty"));
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }
}

void tst_qqmlproperty::qmlmetaproperty_object_string_context()
{
    QObject object;
    PropertyObject dobject;

    {
        QQmlProperty prop(&object, QString("defaultProperty"), engine.rootContext());

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString());
        QCOMPARE(prop.read(), QVariant());
        QCOMPARE(prop.write(QVariant()), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), false);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QVERIFY(!prop.method().isValid());
        QCOMPARE(prop.type(), QQmlProperty::Invalid);
        QCOMPARE(prop.isProperty(), false);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), false);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), false);
        QCOMPARE(prop.isValid(), false);
        QCOMPARE(prop.object(), (QObject *)0);
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::InvalidCategory);
        QCOMPARE(prop.propertyType(), 0);
        QCOMPARE(prop.propertyTypeName(), (const char *)0);
        QVERIFY(prop.property().name() == 0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding == 0);
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(sigExprWatcher.wasDeleted());
        QCOMPARE(prop.index(), -1);
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }

    {
        QQmlProperty prop(&dobject, QString("defaultProperty"), engine.rootContext());

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        static_cast<QQmlBinding *>(binding.data())->setTarget(prop);
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString("defaultProperty"));
        QCOMPARE(prop.read(), QVariant(10));
        QCOMPARE(prop.write(QVariant()), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), true);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QVERIFY(!prop.method().isValid());
        QCOMPARE(prop.type(), QQmlProperty::Property);
        QCOMPARE(prop.isProperty(), true);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), true);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), false);
        QCOMPARE(prop.isValid(), true);
        QCOMPARE(prop.object(), qobject_cast<QObject*>(&dobject));
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::Normal);
        QCOMPARE(prop.propertyType(), (int)QVariant::Int);
        QCOMPARE(prop.propertyTypeName(), "int");
        QCOMPARE(QString(prop.property().name()), QString("defaultProperty"));
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QTest::ignoreMessage(QtWarningMsg, "<Unknown File>: Unable to assign null to int");
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding != 0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == binding.data());
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(sigExprWatcher.wasDeleted());
        QCOMPARE(prop.index(), dobject.metaObject()->indexOfProperty("defaultProperty"));
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }

    {
        QQmlProperty prop(&dobject, QString("onClicked"), engine.rootContext());

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        static_cast<QQmlBinding *>(binding.data())->setTarget(prop);
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString("onClicked"));
        QCOMPARE(prop.read(), QVariant());
        QCOMPARE(prop.write(QVariant("Hello")), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), false);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QCOMPARE(QString(prop.method().methodSignature()), QString("clicked()"));
        QCOMPARE(prop.type(), QQmlProperty::SignalProperty);
        QCOMPARE(prop.isProperty(), false);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), false);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), true);
        QCOMPARE(prop.isValid(), true);
        QCOMPARE(prop.object(), qobject_cast<QObject*>(&dobject));
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::InvalidCategory);
        QCOMPARE(prop.propertyType(), 0);
        QCOMPARE(prop.propertyTypeName(), (const char *)0);
        QCOMPARE(prop.property().name(), (const char *)0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding == 0);
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(!sigExprWatcher.wasDeleted());
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == sigExpr);
        QCOMPARE(prop.index(), dobject.metaObject()->indexOfMethod("clicked()"));
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }

    {
        QQmlProperty prop(&dobject, QString("onPropertyWithNotifyChanged"), engine.rootContext());

        QWeakPointer<QQmlAbstractBinding> binding(QQmlAbstractBinding::getPointer(new QQmlBinding(QLatin1String("null"), 0, engine.rootContext())));
        static_cast<QQmlBinding *>(binding.data())->setTarget(prop);
        QVERIFY(binding != 0);
        QQmlBoundSignalExpression *sigExpr = new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1);
        QQmlAbstractExpression::DeleteWatcher sigExprWatcher(sigExpr);
        QVERIFY(sigExpr != 0 && !sigExprWatcher.wasDeleted());

        QObject *obj = new QObject;

        QCOMPARE(prop.name(), QString("onOddlyNamedNotifySignal"));
        QCOMPARE(prop.read(), QVariant());
        QCOMPARE(prop.write(QVariant("Hello")), false);
        QCOMPARE(prop.hasNotifySignal(), false);
        QCOMPARE(prop.needsNotifySignal(), false);
        QCOMPARE(prop.connectNotifySignal(0, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, SLOT(deleteLater())), false);
        QCOMPARE(prop.connectNotifySignal(obj, 0), false);
        QCOMPARE(prop.connectNotifySignal(0, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, obj->metaObject()->indexOfMethod("deleteLater()")), false);
        QCOMPARE(prop.connectNotifySignal(obj, -1), false);
        QCOMPARE(QString(prop.method().methodSignature()), QString("oddlyNamedNotifySignal()"));
        QCOMPARE(prop.type(), QQmlProperty::SignalProperty);
        QCOMPARE(prop.isProperty(), false);
        QCOMPARE(prop.isWritable(), false);
        QCOMPARE(prop.isDesignable(), false);
        QCOMPARE(prop.isResettable(), false);
        QCOMPARE(prop.isSignalProperty(), true);
        QCOMPARE(prop.isValid(), true);
        QCOMPARE(prop.object(), qobject_cast<QObject*>(&dobject));
        QCOMPARE(prop.propertyTypeCategory(), QQmlProperty::InvalidCategory);
        QCOMPARE(prop.propertyType(), 0);
        QCOMPARE(prop.propertyTypeName(), (const char *)0);
        QCOMPARE(prop.property().name(), (const char *)0);
        QVERIFY(QQmlPropertyPrivate::binding(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::setBinding(prop, binding.data()) == 0);
        QVERIFY(binding == 0);
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == 0);
        QVERIFY(QQmlPropertyPrivate::takeSignalExpression(prop, sigExpr) == 0);
        QVERIFY(!sigExprWatcher.wasDeleted());
        QVERIFY(QQmlPropertyPrivate::signalExpression(prop) == sigExpr);
        QCOMPARE(prop.index(), dobject.metaObject()->indexOfMethod("oddlyNamedNotifySignal()"));
        QCOMPARE(QQmlPropertyPrivate::valueTypeCoreIndex(prop), -1);

        delete obj;
    }
}

void tst_qqmlproperty::name()
{
    {
        QQmlProperty p;
        QCOMPARE(p.name(), QString());
    }

    {
        PropertyObject o;
        QQmlProperty p(&o);
        QCOMPARE(p.name(), QString("defaultProperty"));
    }

    {
        QObject o;
        QQmlProperty p(&o, QString("objectName"));
        QCOMPARE(p.name(), QString("objectName"));
    }

    {
        PropertyObject o;
        QQmlProperty p(&o, "onClicked");
        QCOMPARE(p.name(), QString("onClicked"));
    }

    {
        QObject o;
        QQmlProperty p(&o, "onClicked");
        QCOMPARE(p.name(), QString());
    }

    {
        PropertyObject o;
        QQmlProperty p(&o, "onPropertyWithNotifyChanged");
        QCOMPARE(p.name(), QString("onOddlyNamedNotifySignal"));
    }

    {
        QObject o;
        QQmlProperty p(&o, "onPropertyWithNotifyChanged");
        QCOMPARE(p.name(), QString());
    }

    {
        QObject o;
        QQmlProperty p(&o, "foo");
        QCOMPARE(p.name(), QString());
    }

    {
        QQmlProperty p(0, "foo");
        QCOMPARE(p.name(), QString());
    }

    {
        PropertyObject o;
        QQmlProperty p(&o, "rectProperty");
        QCOMPARE(p.name(), QString("rectProperty"));
    }

    {
        PropertyObject o;
        QQmlProperty p(&o, "rectProperty.x");
        QCOMPARE(p.name(), QString("rectProperty.x"));
    }

    {
        PropertyObject o;
        QQmlProperty p(&o, "rectProperty.foo");
        QCOMPARE(p.name(), QString());
    }
}

void tst_qqmlproperty::read()
{
    // Invalid
    {
        QQmlProperty p;
        QCOMPARE(p.read(), QVariant());
    }

    // Default prop
    {
        PropertyObject o;
        QQmlProperty p(&o);
        QCOMPARE(p.read(), QVariant(10));
    }

    // Invalid default prop
    {
        QObject o;
        QQmlProperty p(&o);
        QCOMPARE(p.read(), QVariant());
    }

    // Value prop by name
    {
        QObject o;

        QQmlProperty p(&o, "objectName");
        QCOMPARE(p.read(), QVariant(QString()));

        o.setObjectName("myName");

        QCOMPARE(p.read(), QVariant("myName"));
    }

    // Value prop by name (static)
    {
        QObject o;

        QCOMPARE(QQmlProperty::read(&o, "objectName"), QVariant(QString()));

        o.setObjectName("myName");

        QCOMPARE(QQmlProperty::read(&o, "objectName"), QVariant("myName"));
    }

    // Value-type prop
    {
        PropertyObject o;
        QQmlProperty p(&o, "rectProperty.x");
        QCOMPARE(p.read(), QVariant(10));
    }

    // Invalid value-type prop
    {
        PropertyObject o;
        QQmlProperty p(&o, "rectProperty.foo");
        QCOMPARE(p.read(), QVariant());
    }

    // Signal property
    {
        PropertyObject o;
        QQmlProperty p(&o, "onClicked");
        QCOMPARE(p.read(), QVariant());

        QVERIFY(0 == QQmlPropertyPrivate::takeSignalExpression(p, new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1)));
        QVERIFY(0 != QQmlPropertyPrivate::signalExpression(p));

        QCOMPARE(p.read(), QVariant());
    }

    // Automatic signal property
    {
        PropertyObject o;
        QQmlProperty p(&o, "onPropertyWithNotifyChanged");
        QCOMPARE(p.read(), QVariant());

        QVERIFY(0 == QQmlPropertyPrivate::takeSignalExpression(p, new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1)));
        QVERIFY(0 != QQmlPropertyPrivate::signalExpression(p));

        QCOMPARE(p.read(), QVariant());
    }

    // Deleted object
    {
        PropertyObject *o = new PropertyObject;
        QQmlProperty p(o, "rectProperty.x");
        QCOMPARE(p.read(), QVariant(10));
        delete o;
        QCOMPARE(p.read(), QVariant());
    }

    // Object property
    {
        PropertyObject o;
        QQmlProperty p(&o, "qmlObject");
        QCOMPARE(p.propertyTypeCategory(), QQmlProperty::Object);
        QCOMPARE(p.propertyType(), qMetaTypeId<MyQmlObject*>());
        QVariant v = p.read();
        QVERIFY(v.userType() == QMetaType::QObjectStar);
        QVERIFY(qvariant_cast<QObject *>(v) == o.qmlObject());
    }
    {
        QQmlComponent component(&engine, testFileUrl("readSynthesizedObject.qml"));
        QObject *object = component.create();
        QVERIFY(object != 0);

        QQmlProperty p(object, "test", &engine);

        QCOMPARE(p.propertyTypeCategory(), QQmlProperty::Object);
        QVERIFY(p.propertyType() != QMetaType::QObjectStar);

        QVariant v = p.read();
        QVERIFY(v.userType() == QMetaType::QObjectStar);
        QCOMPARE(qvariant_cast<QObject *>(v)->property("a").toInt(), 10);
        QCOMPARE(qvariant_cast<QObject *>(v)->property("b").toInt(), 19);
    }
    {   // static
        QQmlComponent component(&engine, testFileUrl("readSynthesizedObject.qml"));
        QObject *object = component.create();
        QVERIFY(object != 0);

        QVariant v = QQmlProperty::read(object, "test", &engine);
        QVERIFY(v.userType() == QMetaType::QObjectStar);
        QCOMPARE(qvariant_cast<QObject *>(v)->property("a").toInt(), 10);
        QCOMPARE(qvariant_cast<QObject *>(v)->property("b").toInt(), 19);
    }

    // Attached property
    {
        QQmlComponent component(&engine);
        component.setData("import Test 1.0\nMyContainer { }", QUrl());
        QObject *object = component.create();
        QVERIFY(object != 0);

        QQmlProperty p(object, "MyContainer.foo", qmlContext(object));
        QCOMPARE(p.read(), QVariant(13));
        delete object;
    }
    {
        QQmlComponent component(&engine);
        component.setData("import Test 1.0\nMyContainer { MyContainer.foo: 10 }", QUrl());
        QObject *object = component.create();
        QVERIFY(object != 0);

        QQmlProperty p(object, "MyContainer.foo", qmlContext(object));
        QCOMPARE(p.read(), QVariant(10));
        delete object;
    }
    {
        QQmlComponent component(&engine);
        component.setData("import Test 1.0 as Foo\nFoo.MyContainer { Foo.MyContainer.foo: 10 }", QUrl());
        QObject *object = component.create();
        QVERIFY(object != 0);

        QQmlProperty p(object, "Foo.MyContainer.foo", qmlContext(object));
        QCOMPARE(p.read(), QVariant(10));
        delete object;
    }
    {   // static
        QQmlComponent component(&engine);
        component.setData("import Test 1.0 as Foo\nFoo.MyContainer { Foo.MyContainer.foo: 10 }", QUrl());
        QObject *object = component.create();
        QVERIFY(object != 0);

        QCOMPARE(QQmlProperty::read(object, "Foo.MyContainer.foo", qmlContext(object)), QVariant(10));
        delete object;
    }
}

void tst_qqmlproperty::write()
{
    // Invalid
    {
        QQmlProperty p;
        QCOMPARE(p.write(QVariant(10)), false);
    }

    // Read-only default prop
    {
        PropertyObject o;
        QQmlProperty p(&o);
        QCOMPARE(p.write(QVariant(10)), false);
    }

    // Invalid default prop
    {
        QObject o;
        QQmlProperty p(&o);
        QCOMPARE(p.write(QVariant(10)), false);
    }

    // Read-only prop by name
    {
        PropertyObject o;
        QQmlProperty p(&o, QString("defaultProperty"));
        QCOMPARE(p.write(QVariant(10)), false);
    }

    // Writable prop by name
    {
        PropertyObject o;
        QQmlProperty p(&o, QString("objectName"));
        QCOMPARE(o.objectName(), QString());
        QCOMPARE(p.write(QVariant(QString("myName"))), true);
        QCOMPARE(o.objectName(), QString("myName"));
    }

    // Writable prop by name (static)
    {
        PropertyObject o;
        QCOMPARE(QQmlProperty::write(&o, QString("objectName"), QVariant(QString("myName"))), true);
        QCOMPARE(o.objectName(), QString("myName"));
    }

    // Deleted object
    {
        PropertyObject *o = new PropertyObject;
        QQmlProperty p(o, QString("objectName"));
        QCOMPARE(p.write(QVariant(QString("myName"))), true);
        QCOMPARE(o->objectName(), QString("myName"));

        delete o;

        QCOMPARE(p.write(QVariant(QString("myName"))), false);
    }

    // Signal property
    {
        PropertyObject o;
        QQmlProperty p(&o, "onClicked");
        QCOMPARE(p.write(QVariant("console.log(1921)")), false);

        QVERIFY(0 == QQmlPropertyPrivate::takeSignalExpression(p, new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1)));
        QVERIFY(0 != QQmlPropertyPrivate::signalExpression(p));

        QCOMPARE(p.write(QVariant("console.log(1921)")), false);

        QVERIFY(0 != QQmlPropertyPrivate::signalExpression(p));
    }

    // Automatic signal property
    {
        PropertyObject o;
        QQmlProperty p(&o, "onPropertyWithNotifyChanged");
        QCOMPARE(p.write(QVariant("console.log(1921)")), false);

        QVERIFY(0 == QQmlPropertyPrivate::takeSignalExpression(p, new QQmlBoundSignalExpression(QQmlContextData::get(engine.rootContext()), 0, QLatin1String("null"), false, QString(), -1, -1)));
        QVERIFY(0 != QQmlPropertyPrivate::signalExpression(p));

        QCOMPARE(p.write(QVariant("console.log(1921)")), false);

        QVERIFY(0 != QQmlPropertyPrivate::signalExpression(p));
    }

    // Value-type property
    {
        PropertyObject o;
        QQmlProperty p(&o, "wrectProperty");

        QCOMPARE(o.wrectProperty(), QRect());
        QCOMPARE(p.write(QRect(1, 13, 99, 8)), true);
        QCOMPARE(o.wrectProperty(), QRect(1, 13, 99, 8));

        QQmlProperty p2(&o, "wrectProperty.x");
        QCOMPARE(p2.read(), QVariant(1));
        QCOMPARE(p2.write(QVariant(6)), true);
        QCOMPARE(p2.read(), QVariant(6));
        QCOMPARE(o.wrectProperty(), QRect(6, 13, 99, 8));
    }

    // URL-property
    {
        PropertyObject o;
        QQmlProperty p(&o, "url");

        QCOMPARE(p.write(QUrl("main.qml")), true);
        QCOMPARE(o.url(), QUrl("main.qml"));

        QQmlProperty p2(&o, "url", engine.rootContext());

        QUrl result = engine.baseUrl().resolved(QUrl("main.qml"));
        QVERIFY(result != QUrl("main.qml"));

        QCOMPARE(p2.write(QUrl("main.qml")), true);
        QCOMPARE(o.url(), result);
    }
    {   // static
        PropertyObject o;

        QCOMPARE(QQmlProperty::write(&o, "url", QUrl("main.qml")), true);
        QCOMPARE(o.url(), QUrl("main.qml"));

        QUrl result = engine.baseUrl().resolved(QUrl("main.qml"));
        QVERIFY(result != QUrl("main.qml"));

        QCOMPARE(QQmlProperty::write(&o, "url", QUrl("main.qml"), engine.rootContext()), true);
        QCOMPARE(o.url(), result);
    }

    // VariantMap-property
    QVariantMap vm;
    vm.insert("key", "value");

    {
        PropertyObject o;
        QQmlProperty p(&o, "variantMap");

        QCOMPARE(p.write(vm), true);
        QCOMPARE(o.variantMap(), vm);

        QQmlProperty p2(&o, "variantMap", engine.rootContext());

        QCOMPARE(p2.write(vm), true);
        QCOMPARE(o.variantMap(), vm);
    }
    {   // static
        PropertyObject o;

        QCOMPARE(QQmlProperty::write(&o, "variantMap", vm), true);
        QCOMPARE(o.variantMap(), vm);

        QCOMPARE(QQmlProperty::write(&o, "variantMap", vm, engine.rootContext()), true);
        QCOMPARE(o.variantMap(), vm);
    }

    // Attached property
    {
        QQmlComponent component(&engine);
        component.setData("import Test 1.0\nMyContainer { }", QUrl());
        QObject *object = component.create();
        QVERIFY(object != 0);

        QQmlProperty p(object, "MyContainer.foo", qmlContext(object));
        p.write(QVariant(99));
        QCOMPARE(p.read(), QVariant(99));
        delete object;
    }
    {
        QQmlComponent component(&engine);
        component.setData("import Test 1.0 as Foo\nFoo.MyContainer { Foo.MyContainer.foo: 10 }", QUrl());
        QObject *object = component.create();
        QVERIFY(object != 0);

        QQmlProperty p(object, "Foo.MyContainer.foo", qmlContext(object));
        p.write(QVariant(99));
        QCOMPARE(p.read(), QVariant(99));
        delete object;
    }
}

void tst_qqmlproperty::reset()
{
    // Invalid
    {
        QQmlProperty p;
        QCOMPARE(p.isResettable(), false);
        QCOMPARE(p.reset(), false);
    }

    // Read-only default prop
    {
        PropertyObject o;
        QQmlProperty p(&o);
        QCOMPARE(p.isResettable(), false);
        QCOMPARE(p.reset(), false);
    }

    // Invalid default prop
    {
        QObject o;
        QQmlProperty p(&o);
        QCOMPARE(p.isResettable(), false);
        QCOMPARE(p.reset(), false);
    }

    // Non-resettable-only prop by name
    {
        PropertyObject o;
        QQmlProperty p(&o, QString("defaultProperty"));
        QCOMPARE(p.isResettable(), false);
        QCOMPARE(p.reset(), false);
    }

    // Resettable prop by name
    {
        PropertyObject o;
        QQmlProperty p(&o, QString("resettableProperty"));

        QCOMPARE(p.read(), QVariant(9));
        QCOMPARE(p.write(QVariant(11)), true);
        QCOMPARE(p.read(), QVariant(11));

        QCOMPARE(p.isResettable(), true);
        QCOMPARE(p.reset(), true);

        QCOMPARE(p.read(), QVariant(9));
    }

    // Deleted object
    {
        PropertyObject *o = new PropertyObject;

        QQmlProperty p(o, QString("resettableProperty"));

        QCOMPARE(p.isResettable(), true);
        QCOMPARE(p.reset(), true);

        delete o;

        QCOMPARE(p.isResettable(), false);
        QCOMPARE(p.reset(), false);
    }

    // Signal property
    {
        PropertyObject o;
        QQmlProperty p(&o, "onClicked");

        QCOMPARE(p.isResettable(), false);
        QCOMPARE(p.reset(), false);
    }

    // Automatic signal property
    {
        PropertyObject o;
        QQmlProperty p(&o, "onPropertyWithNotifyChanged");

        QCOMPARE(p.isResettable(), false);
        QCOMPARE(p.reset(), false);
    }
}

void tst_qqmlproperty::writeObjectToList()
{
    QQmlComponent containerComponent(&engine);
    containerComponent.setData("import Test 1.0\nMyContainer { children: MyQmlObject {} }", QUrl());
    MyContainer *container = qobject_cast<MyContainer*>(containerComponent.create());
    QVERIFY(container != 0);
    QQmlListReference list(container, "children");
    QVERIFY(list.count() == 1);

    MyQmlObject *object = new MyQmlObject;
    QQmlProperty prop(container, "children");
    prop.write(qVariantFromValue(object));
    QCOMPARE(list.count(), 1);
    QCOMPARE(list.at(0), qobject_cast<QObject*>(object));
}

void tst_qqmlproperty::writeListToList()
{
    QQmlComponent containerComponent(&engine);
    containerComponent.setData("import Test 1.0\nMyContainer { children: MyQmlObject {} }", QUrl());
    MyContainer *container = qobject_cast<MyContainer*>(containerComponent.create());
    QVERIFY(container != 0);
    QQmlListReference list(container, "children");
    QVERIFY(list.count() == 1);

    QList<QObject*> objList;
    objList << new MyQmlObject() << new MyQmlObject() << new MyQmlObject() << new MyQmlObject();
    QQmlProperty prop(container, "children");
    prop.write(qVariantFromValue(objList));
    QCOMPARE(list.count(), 4);

    //XXX need to try this with read/write prop (for read-only it correctly doesn't write)
    /*QList<MyQmlObject*> typedObjList;
    typedObjList << new MyQmlObject();
    prop.write(qVariantFromValue(&typedObjList));
    QCOMPARE(container->children()->size(), 1);*/
}

void tst_qqmlproperty::urlHandling_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("scheme");
    QTest::addColumn<QString>("path");
    QTest::addColumn<QByteArray>("encoded");

    QTest::newRow("unspecifiedFile")
        << QByteArray("main.qml")
        << QString("")
        << QString("main.qml")
        << QByteArray("main.qml");

    QTest::newRow("specifiedFile")
        << QByteArray("file:///main.qml")
        << QString("file")
        << QString("/main.qml")
        << QByteArray("file:///main.qml");

    QTest::newRow("httpFile")
        << QByteArray("http://www.example.com/main.qml")
        << QString("http")
        << QString("/main.qml")
        << QByteArray("http://www.example.com/main.qml");

    QTest::newRow("pathFile")
        << QByteArray("http://www.example.com/resources/main.qml")
        << QString("http")
        << QString("/resources/main.qml")
        << QByteArray("http://www.example.com/resources/main.qml");

    QTest::newRow("encodableName")
        << QByteArray("http://www.example.com/main file.qml")
        << QString("http")
        << QString("/main file.qml")
        << QByteArray("http://www.example.com/main%20file.qml");

    QTest::newRow("preencodedName")
        << QByteArray("http://www.example.com/resources%7Cmain%20file.qml")
        << QString("http")
        << QString("/resources|main file.qml")
        << QByteArray("http://www.example.com/resources%7Cmain%20file.qml");

    QTest::newRow("encodableQuery")
        << QByteArray("http://www.example.com/main.qml?type=text/qml&comment=now working?")
        << QString("http")
        << QString("/main.qml")
        << QByteArray("http://www.example.com/main.qml?type=text/qml&comment=now%20working?");

    // Although 'text%2Fqml' is pre-encoded, it will be decoded to allow correct QUrl classification
    QTest::newRow("preencodedQuery")
        << QByteArray("http://www.example.com/main.qml?type=text%2Fqml&comment=now working%3F")
        << QString("http")
        << QString("/main.qml")
        << QByteArray("http://www.example.com/main.qml?type=text/qml&comment=now%20working%3F");

    QTest::newRow("encodableFragment")
        << QByteArray("http://www.example.com/main.qml?type=text/qml#start+30000|volume+50%")
        << QString("http")
        << QString("/main.qml")
        << QByteArray("http://www.example.com/main.qml?type=text/qml#start+30000%7Cvolume+50%25");

    QTest::newRow("improperlyEncodedFragment")
        << QByteArray("http://www.example.com/main.qml?type=text/qml#start+30000%7Cvolume%2B50%")
        << QString("http")
        << QString("/main.qml")
        << QByteArray("http://www.example.com/main.qml?type=text/qml#start+30000%257Cvolume%252B50%25");
}

void tst_qqmlproperty::urlHandling()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, scheme);
    QFETCH(QString, path);
    QFETCH(QByteArray, encoded);

    QString inputString(QString::fromUtf8(input));

    {
        PropertyObject o;
        QQmlProperty p(&o, "url");

        // Test url written as QByteArray
        QCOMPARE(p.write(input), true);
        QUrl byteArrayResult(o.url());

        QCOMPARE(byteArrayResult.scheme(), scheme);
        QCOMPARE(byteArrayResult.path(), path);
        QCOMPARE(byteArrayResult.toString(QUrl::FullyEncoded), QString::fromUtf8(encoded));
        QCOMPARE(byteArrayResult.toEncoded(), encoded);
    }

    {
        PropertyObject o;
        QQmlProperty p(&o, "url");

        // Test url written as QString
        QCOMPARE(p.write(inputString), true);
        QUrl stringResult(o.url());

        QCOMPARE(stringResult.scheme(), scheme);
        QCOMPARE(stringResult.path(), path);
        QCOMPARE(stringResult.toString(QUrl::FullyEncoded), QString::fromUtf8(encoded));
        QCOMPARE(stringResult.toEncoded(), encoded);
    }
}

void tst_qqmlproperty::variantMapHandling_data()
{
    QTest::addColumn<QVariantMap>("vm");

    // Object literals
    {
        QVariantMap m;
        QTest::newRow("{}") << m;
    }
    {
        QVariantMap m;
        m["a"] = QVariantMap();
        QTest::newRow("{ a:{} }") << m;
    }
    {
        QVariantMap m, m2;
        m2["b"] = 10;
        m2["c"] = 20;
        m["a"] = m2;
        QTest::newRow("{ a:{b:10, c:20} }") << m;
    }
    {
        QVariantMap m;
        m["a"] = 10;
        m["b"] = QVariantList() << 20 << 30;
        QTest::newRow("{ a:10, b:[20, 30]}") << m;
    }

    // Cyclic objects
    {
        QVariantMap m;
        m["p"] = QVariantMap();
        QTest::newRow("var o={}; o.p=o") << m;
    }
    {
        QVariantMap m;
        m["p"] = 123;
        m["q"] = QVariantMap();
        QTest::newRow("var o={}; o.p=123; o.q=o") << m;
    }
}

void tst_qqmlproperty::variantMapHandling()
{
    QFETCH(QVariantMap, vm);

    PropertyObject o;
    QQmlProperty p(&o, "variantMap");

    QCOMPARE(p.write(vm), true);
    QCOMPARE(o.variantMap(), vm);
}

void tst_qqmlproperty::crashOnValueProperty()
{
    QQmlEngine *engine = new QQmlEngine;
    QQmlComponent component(engine);

    component.setData("import Test 1.0\nPropertyObject { wrectProperty.x: 10 }", QUrl());
    PropertyObject *obj = qobject_cast<PropertyObject*>(component.create());
    QVERIFY(obj != 0);

    QQmlProperty p(obj, "wrectProperty.x", qmlContext(obj));
    QCOMPARE(p.name(), QString("wrectProperty.x"));

    QCOMPARE(p.read(), QVariant(10));

    //don't crash once the engine is deleted
    delete engine;
    engine = 0;

    QCOMPARE(p.propertyTypeName(), "int");
    QCOMPARE(p.read(), QVariant(10));
    p.write(QVariant(20));
    QCOMPARE(p.read(), QVariant(20));
}

// QTBUG-13719
void tst_qqmlproperty::aliasPropertyBindings()
{
    QQmlComponent component(&engine, testFileUrl("aliasPropertyBindings.qml"));

    QObject *object = component.create();
    QVERIFY(object != 0);

    QCOMPARE(object->property("realProperty").toReal(), 90.);
    QCOMPARE(object->property("aliasProperty").toReal(), 90.);

    object->setProperty("test", 10);

    QCOMPARE(object->property("realProperty").toReal(), 110.);
    QCOMPARE(object->property("aliasProperty").toReal(), 110.);

    QQmlProperty realProperty(object, QLatin1String("realProperty"));
    QQmlProperty aliasProperty(object, QLatin1String("aliasProperty"));

    // Check there is a binding on these two properties
    QVERIFY(QQmlPropertyPrivate::binding(realProperty) != 0);
    QVERIFY(QQmlPropertyPrivate::binding(aliasProperty) != 0);

    // Check that its the same binding on these two properties
    QCOMPARE(QQmlPropertyPrivate::binding(realProperty),
             QQmlPropertyPrivate::binding(aliasProperty));

    // Change the binding
    object->setProperty("state", QString("switch"));

    QVERIFY(QQmlPropertyPrivate::binding(realProperty) != 0);
    QVERIFY(QQmlPropertyPrivate::binding(aliasProperty) != 0);
    QCOMPARE(QQmlPropertyPrivate::binding(realProperty),
             QQmlPropertyPrivate::binding(aliasProperty));

    QCOMPARE(object->property("realProperty").toReal(), 96.);
    QCOMPARE(object->property("aliasProperty").toReal(), 96.);

    // Check the old binding really has not effect any more
    object->setProperty("test", 4);

    QCOMPARE(object->property("realProperty").toReal(), 96.);
    QCOMPARE(object->property("aliasProperty").toReal(), 96.);

    object->setProperty("test2", 9);

    QCOMPARE(object->property("realProperty").toReal(), 288.);
    QCOMPARE(object->property("aliasProperty").toReal(), 288.);

    // Revert
    object->setProperty("state", QString(""));

    QVERIFY(QQmlPropertyPrivate::binding(realProperty) != 0);
    QVERIFY(QQmlPropertyPrivate::binding(aliasProperty) != 0);
    QCOMPARE(QQmlPropertyPrivate::binding(realProperty),
             QQmlPropertyPrivate::binding(aliasProperty));

    QCOMPARE(object->property("realProperty").toReal(), 20.);
    QCOMPARE(object->property("aliasProperty").toReal(), 20.);

    object->setProperty("test2", 3);

    QCOMPARE(object->property("realProperty").toReal(), 20.);
    QCOMPARE(object->property("aliasProperty").toReal(), 20.);

    delete object;
}

void tst_qqmlproperty::copy()
{
    PropertyObject object;

    QQmlProperty *property = new QQmlProperty(&object, QLatin1String("defaultProperty"));
    QCOMPARE(property->name(), QString("defaultProperty"));
    QCOMPARE(property->read(), QVariant(10));
    QCOMPARE(property->type(), QQmlProperty::Property);
    QCOMPARE(property->propertyTypeCategory(), QQmlProperty::Normal);
    QCOMPARE(property->propertyType(), (int)QVariant::Int);

    QQmlProperty p1(*property);
    QCOMPARE(p1.name(), QString("defaultProperty"));
    QCOMPARE(p1.read(), QVariant(10));
    QCOMPARE(p1.type(), QQmlProperty::Property);
    QCOMPARE(p1.propertyTypeCategory(), QQmlProperty::Normal);
    QCOMPARE(p1.propertyType(), (int)QVariant::Int);

    QQmlProperty p2(&object, QLatin1String("url"));
    QCOMPARE(p2.name(), QString("url"));
    p2 = *property;
    QCOMPARE(p2.name(), QString("defaultProperty"));
    QCOMPARE(p2.read(), QVariant(10));
    QCOMPARE(p2.type(), QQmlProperty::Property);
    QCOMPARE(p2.propertyTypeCategory(), QQmlProperty::Normal);
    QCOMPARE(p2.propertyType(), (int)QVariant::Int);

    delete property; property = 0;

    QCOMPARE(p1.name(), QString("defaultProperty"));
    QCOMPARE(p1.read(), QVariant(10));
    QCOMPARE(p1.type(), QQmlProperty::Property);
    QCOMPARE(p1.propertyTypeCategory(), QQmlProperty::Normal);
    QCOMPARE(p1.propertyType(), (int)QVariant::Int);

    QCOMPARE(p2.name(), QString("defaultProperty"));
    QCOMPARE(p2.read(), QVariant(10));
    QCOMPARE(p2.type(), QQmlProperty::Property);
    QCOMPARE(p2.propertyTypeCategory(), QQmlProperty::Normal);
    QCOMPARE(p2.propertyType(), (int)QVariant::Int);
}

void tst_qqmlproperty::noContext()
{
    QQmlComponent compA(&engine, testFileUrl("NoContextTypeA.qml"));
    QQmlComponent compB(&engine, testFileUrl("NoContextTypeB.qml"));

    QObject *a = compA.create();
    QVERIFY(a != 0);
    QObject *b = compB.create();
    QVERIFY(b != 0);

    QVERIFY(QQmlProperty::write(b, "myTypeA", QVariant::fromValue(a), &engine));

    delete a;
    delete b;
}

void tst_qqmlproperty::assignEmptyVariantMap()
{
    PropertyObject o;

    QVariantMap map;
    map.insert("key", "value");
    o.setVariantMap(map);
    QCOMPARE(o.variantMap().count(), 1);
    QCOMPARE(o.variantMap().isEmpty(), false);

    QQmlContext context(&engine);
    context.setContextProperty("o", &o);

    QQmlComponent component(&engine, testFileUrl("assignEmptyVariantMap.qml"));
    QObject *obj = component.create(&context);
    QVERIFY(obj);

    QCOMPARE(o.variantMap().count(), 0);
    QCOMPARE(o.variantMap().isEmpty(), true);

    delete obj;
}

void tst_qqmlproperty::warnOnInvalidBinding()
{
    QUrl testUrl(testFileUrl("invalidBinding.qml"));
    QString expectedWarning;

    // V4 error message for property-to-property binding
    expectedWarning = testUrl.toString() + QString::fromLatin1(":6:36: Unable to assign QQuickText to QQuickRectangle");
    QTest::ignoreMessage(QtWarningMsg, expectedWarning.toLatin1().constData());

    // V8 error message for function-to-property binding
    expectedWarning = testUrl.toString() + QString::fromLatin1(":7:36: Unable to assign QQuickText to QQuickRectangle");
    QTest::ignoreMessage(QtWarningMsg, expectedWarning.toLatin1().constData());

    // V8 error message for invalid binding to anchor
    expectedWarning = testUrl.toString() + QString::fromLatin1(":14: Unable to assign QQuickItem_QML_6 to QQuickAnchorLine");
    QTest::ignoreMessage(QtWarningMsg, expectedWarning.toLatin1().constData());

    QQmlComponent component(&engine, testUrl);
    QObject *obj = component.create();
    QVERIFY(obj);
    delete obj;
}

void tst_qqmlproperty::initTestCase()
{
    QQmlDataTest::initTestCase();
    qmlRegisterType<MyQmlObject>("Test",1,0,"MyQmlObject");
    qmlRegisterType<PropertyObject>("Test",1,0,"PropertyObject");
    qmlRegisterType<MyContainer>("Test",1,0,"MyContainer");
}

QTEST_MAIN(tst_qqmlproperty)

#include "tst_qqmlproperty.moc"
