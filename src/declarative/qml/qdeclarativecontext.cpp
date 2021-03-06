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

#include "qdeclarativecontext.h"
#include "qdeclarativecontext_p.h"
#include "qdeclarativecomponentattached_p.h"

#include "qdeclarativecomponent_p.h"
#include "qdeclarativeexpression_p.h"
#include "qdeclarativeengine_p.h"
#include "qdeclarativeengine.h"
#include "qdeclarativeinfo.h"
#include <private/qv4bindings_p.h>
#include <private/qv8bindings_p.h>

#include <qjsengine.h>
#include <QtCore/qvarlengtharray.h>
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

QDeclarativeContextPrivate::QDeclarativeContextPrivate()
: data(0), notifyIndex(-1)
{
}

/*!
    \class QDeclarativeContext
    \since 4.7
    \brief The QDeclarativeContext class defines a context within a QML engine.
    \mainclass

    Contexts allow data to be exposed to the QML components instantiated by the
    QML engine.

    Each QDeclarativeContext contains a set of properties, distinct from its QObject
    properties, that allow data to be explicitly bound to a context by name.  The
    context properties are defined and updated by calling
    QDeclarativeContext::setContextProperty().  The following example shows a Qt model
    being bound to a context and then accessed from a QML file.

    \code
    QDeclarativeEngine engine;
    QStringListModel modelData;
    QDeclarativeContext *context = new QDeclarativeContext(engine.rootContext());
    context->setContextProperty("myModel", &modelData);

    QDeclarativeComponent component(&engine);
    component.setData("import QtQuick 1.0\nListView { model: myModel }", QUrl());
    QObject *window = component.create(context);
    \endcode

    Note it is the responsibility of the creator to delete any QDeclarativeContext it
    constructs. If the \c context object in the example is no longer needed when the
    \c window component instance is destroyed, the \c context must be destroyed explicitly.
    The simplest way to ensure this is to set \c window as the parent of \c context.

    To simplify binding and maintaining larger data sets, a context object can be set
    on a QDeclarativeContext.  All the properties of the context object are available
    by name in the context, as though they were all individually added through calls
    to QDeclarativeContext::setContextProperty().  Changes to the property's values are
    detected through the property's notify signal.  Setting a context object is both
    faster and easier than manually adding and maintaing context property values.

    The following example has the same effect as the previous one, but it uses a context
    object.

    \code
    class MyDataSet : ... {
        ...
        Q_PROPERTY(QAbstractItemModel *myModel READ model NOTIFY modelChanged)
        ...
    };

    MyDataSet myDataSet;
    QDeclarativeEngine engine;
    QDeclarativeContext *context = new QDeclarativeContext(engine.rootContext());
    context->setContextObject(&myDataSet);

    QDeclarativeComponent component(&engine);
    component.setData("import QtQuick 1.0\nListView { model: myModel }", QUrl());
    component.create(context);
    \endcode

    All properties added explicitly by QDeclarativeContext::setContextProperty() take
    precedence over the context object's properties.

    \section2 The Context Hierarchy

    Contexts form a hierarchy. The root of this hierarchy is the QML engine's
    \l {QDeclarativeEngine::rootContext()}{root context}. Child contexts inherit
    the context properties of their parents; if a child context sets a context property
    that already exists in its parent, the new context property overrides that of the
    parent.

    The following example defines two contexts - \c context1 and \c context2.  The
    second context overrides the "b" context property inherited from the first with a
    new value.

    \code
    QDeclarativeEngine engine;
    QDeclarativeContext *context1 = new QDeclarativeContext(engine.rootContext());
    QDeclarativeContext *context2 = new QDeclarativeContext(context1);

    context1->setContextProperty("a", 12);
    context1->setContextProperty("b", 12);

    context2->setContextProperty("b", 15);
    \endcode

    While QML objects instantiated in a context are not strictly owned by that
    context, their bindings are.  If a context is destroyed, the property bindings of
    outstanding QML objects will stop evaluating.

    \warning Setting the context object or adding new context properties after an object
    has been created in that context is an expensive operation (essentially forcing all bindings
    to reevaluate). Thus whenever possible you should complete "setup" of the context
    before using it to create any objects.

    \sa {Using QML Bindings in C++ Applications}
*/

/*! \internal */
QDeclarativeContext::QDeclarativeContext(QDeclarativeEngine *e, bool)
: QObject(*(new QDeclarativeContextPrivate))
{
    Q_D(QDeclarativeContext);
    d->data = new QDeclarativeContextData(this);

    d->data->engine = e;
}

/*!
    Create a new QDeclarativeContext as a child of \a engine's root context, and the
    QObject \a parent.
*/
QDeclarativeContext::QDeclarativeContext(QDeclarativeEngine *engine, QObject *parent)
: QObject(*(new QDeclarativeContextPrivate), parent)
{
    Q_D(QDeclarativeContext);
    d->data = new QDeclarativeContextData(this);

    d->data->setParent(engine?QDeclarativeContextData::get(engine->rootContext()):0);
}

/*!
    Create a new QDeclarativeContext with the given \a parentContext, and the
    QObject \a parent.
*/
QDeclarativeContext::QDeclarativeContext(QDeclarativeContext *parentContext, QObject *parent)
: QObject(*(new QDeclarativeContextPrivate), parent)
{
    Q_D(QDeclarativeContext);
    d->data = new QDeclarativeContextData(this);

    d->data->setParent(parentContext?QDeclarativeContextData::get(parentContext):0);
}

/*!
    \internal
*/
QDeclarativeContext::QDeclarativeContext(QDeclarativeContextData *data)
: QObject(*(new QDeclarativeContextPrivate), 0)
{
    Q_D(QDeclarativeContext);
    d->data = data;
}

/*!
    Destroys the QDeclarativeContext.

    Any expressions, or sub-contexts dependent on this context will be
    invalidated, but not destroyed (unless they are parented to the QDeclarativeContext
    object).
 */
QDeclarativeContext::~QDeclarativeContext()
{
    Q_D(QDeclarativeContext);

    if (!d->data->isInternal)
        d->data->destroy();
}

/*!
    Returns whether the context is valid.

    To be valid, a context must have a engine, and it's contextObject(), if any,
    must not have been deleted.
*/
bool QDeclarativeContext::isValid() const
{
    Q_D(const QDeclarativeContext);
    return d->data && d->data->isValid();
}

/*!
    Return the context's QDeclarativeEngine, or 0 if the context has no QDeclarativeEngine or the
    QDeclarativeEngine was destroyed.
*/
QDeclarativeEngine *QDeclarativeContext::engine() const
{
    Q_D(const QDeclarativeContext);
    return d->data->engine;
}

/*!
    Return the context's parent QDeclarativeContext, or 0 if this context has no
    parent or if the parent has been destroyed.
*/
QDeclarativeContext *QDeclarativeContext::parentContext() const
{
    Q_D(const QDeclarativeContext);
    return d->data->parent?d->data->parent->asQDeclarativeContext():0;
}

/*!
    Return the context object, or 0 if there is no context object.
*/
QObject *QDeclarativeContext::contextObject() const
{
    Q_D(const QDeclarativeContext);
    return d->data->contextObject;
}

/*!
    Set the context \a object.
*/
void QDeclarativeContext::setContextObject(QObject *object)
{
    Q_D(QDeclarativeContext);

    QDeclarativeContextData *data = d->data;

    if (data->isInternal) {
        qWarning("QDeclarativeContext: Cannot set context object for internal context.");
        return;
    }

    if (!isValid()) {
        qWarning("QDeclarativeContext: Cannot set context object on invalid context.");
        return;
    }

    data->contextObject = object;
}

/*!
    Set a the \a value of the \a name property on this context.
*/
void QDeclarativeContext::setContextProperty(const QString &name, const QVariant &value)
{
    Q_D(QDeclarativeContext);
    if (d->notifyIndex == -1)
        d->notifyIndex = this->metaObject()->methodCount();

    QDeclarativeContextData *data = d->data;

    if (data->isInternal) {
        qWarning("QDeclarativeContext: Cannot set property on internal context.");
        return;
    }

    if (!isValid()) {
        qWarning("QDeclarativeContext: Cannot set property on invalid context.");
        return;
    }

    if (data->engine) {
        bool ok;
        QObject *o = QDeclarativeEnginePrivate::get(data->engine)->toQObject(value, &ok);
        if (ok) {
            setContextProperty(name, o);
            return;
        }
    }

    if (!data->propertyNames) data->propertyNames = new QDeclarativeIntegerCache();

    int idx = data->propertyNames->value(name);
    if (idx == -1) {
        data->propertyNames->add(name, data->idValueCount + d->propertyValues.count());
        d->propertyValues.append(value);

        data->refreshExpressions();
    } else {
        d->propertyValues[idx] = value;
        QMetaObject::activate(this, idx + d->notifyIndex, 0);
    }
}

/*!
    Set the \a value of the \a name property on this context.

    QDeclarativeContext does \bold not take ownership of \a value.
*/
void QDeclarativeContext::setContextProperty(const QString &name, QObject *value)
{
    Q_D(QDeclarativeContext);
    if (d->notifyIndex == -1)
        d->notifyIndex = this->metaObject()->methodCount();

    QDeclarativeContextData *data = d->data;

    if (data->isInternal) {
        qWarning("QDeclarativeContext: Cannot set property on internal context.");
        return;
    }

    if (!isValid()) {
        qWarning("QDeclarativeContext: Cannot set property on invalid context.");
        return;
    }

    if (!data->propertyNames) data->propertyNames = new QDeclarativeIntegerCache();
    int idx = data->propertyNames->value(name);

    if (idx == -1) {
        data->propertyNames->add(name, data->idValueCount + d->propertyValues.count());
        d->propertyValues.append(QVariant::fromValue(value));

        data->refreshExpressions();
    } else {
        d->propertyValues[idx] = QVariant::fromValue(value);
        QMetaObject::activate(this, idx + d->notifyIndex, 0);
    }
}

/*!
  Returns the value of the \a name property for this context
  as a QVariant.
 */
QVariant QDeclarativeContext::contextProperty(const QString &name) const
{
    Q_D(const QDeclarativeContext);
    QVariant value;
    int idx = -1;

    QDeclarativeContextData *data = d->data;

    if (data->propertyNames)
        idx = data->propertyNames->value(name);

    if (idx == -1) {
        QByteArray utf8Name = name.toUtf8();
        if (data->contextObject) {
            QObject *obj = data->contextObject;
            QDeclarativePropertyData local;
            QDeclarativePropertyData *property =
                QDeclarativePropertyCache::property(data->engine, obj, name, local);

            if (property) value = obj->metaObject()->property(property->coreIndex).read(obj);
        }
        if (!value.isValid() && parentContext())
            value = parentContext()->contextProperty(name);
    } else {
        if (idx >= d->propertyValues.count())
            value = QVariant::fromValue(data->idValues[idx - d->propertyValues.count()].data());
        else
            value = d->propertyValues[idx];
    }

    return value;
}

/*!
Returns the name of \a object in this context, or an empty string if \a object 
is not named in the context.  Objects are named by setContextProperty(), or by ids in
the case of QML created contexts.

If the object has multiple names, the first is returned.
*/
QString QDeclarativeContext::nameForObject(QObject *object) const
{
    Q_D(const QDeclarativeContext);

    return d->data->findObjectId(object);
}

/*!
    Resolves the URL \a src relative to the URL of the
    containing component.

    \sa QDeclarativeEngine::baseUrl(), setBaseUrl()
*/
QUrl QDeclarativeContext::resolvedUrl(const QUrl &src)
{
    Q_D(QDeclarativeContext);
    return d->data->resolvedUrl(src);
}

QUrl QDeclarativeContextData::resolvedUrl(const QUrl &src)
{
    QDeclarativeContextData *ctxt = this;

    if (src.isRelative() && !src.isEmpty()) {
        if (ctxt) {
            while(ctxt) {
                if(ctxt->url.isValid())
                    break;
                else
                    ctxt = ctxt->parent;
            }

            if (ctxt)
                return ctxt->url.resolved(src);
            else if (engine)
                return engine->baseUrl().resolved(src);
        }
        return QUrl();
    } else {
        return src;
    }
}


/*!
    Explicitly sets the url resolvedUrl() will use for relative references to \a baseUrl.

    Calling this function will override the url of the containing
    component used by default.

    \sa resolvedUrl()
*/
void QDeclarativeContext::setBaseUrl(const QUrl &baseUrl)
{
    Q_D(QDeclarativeContext);

    d->data->url = baseUrl;
}

/*!
    Returns the base url of the component, or the containing component
    if none is set.
*/
QUrl QDeclarativeContext::baseUrl() const
{
    Q_D(const QDeclarativeContext);
    const QDeclarativeContextData* data = d->data;
    while (data && data->url.isEmpty())
        data = data->parent;

    if (data)
        return data->url;
    else
        return QUrl();
}

int QDeclarativeContextPrivate::context_count(QDeclarativeListProperty<QObject> *prop)
{
    QDeclarativeContext *context = static_cast<QDeclarativeContext*>(prop->object);
    QDeclarativeContextPrivate *d = QDeclarativeContextPrivate::get(context);
    int contextProperty = (int)(quintptr)prop->data;

    if (d->propertyValues.at(contextProperty).userType() != qMetaTypeId<QList<QObject*> >()) {
        return 0;
    } else {
        return ((const QList<QObject> *)d->propertyValues.at(contextProperty).constData())->count();
    }
}

QObject *QDeclarativeContextPrivate::context_at(QDeclarativeListProperty<QObject> *prop, int index)
{
    QDeclarativeContext *context = static_cast<QDeclarativeContext*>(prop->object);
    QDeclarativeContextPrivate *d = QDeclarativeContextPrivate::get(context);
    int contextProperty = (int)(quintptr)prop->data;

    if (d->propertyValues.at(contextProperty).userType() != qMetaTypeId<QList<QObject*> >()) {
        return 0;
    } else {
        return ((const QList<QObject*> *)d->propertyValues.at(contextProperty).constData())->at(index);
    }
}


QDeclarativeContextData::QDeclarativeContextData()
: parent(0), engine(0), isInternal(false), ownedByParent(false), isJSContext(false), 
  isPragmaLibraryContext(false), unresolvedNames(false), publicContext(0), activeVMEData(0),
  propertyNames(0), contextObject(0), imports(0), childContexts(0), nextChild(0), prevChild(0),
  expressions(0), contextObjects(0), contextGuards(0), idValues(0), idValueCount(0), linkedContext(0),
  componentAttached(0), v4bindings(0), v8bindings(0)
{
}

QDeclarativeContextData::QDeclarativeContextData(QDeclarativeContext *ctxt)
: parent(0), engine(0), isInternal(false), ownedByParent(false), isJSContext(false), 
  isPragmaLibraryContext(false), unresolvedNames(false), publicContext(ctxt), activeVMEData(0),
  propertyNames(0), contextObject(0), imports(0), childContexts(0), nextChild(0), prevChild(0),
  expressions(0), contextObjects(0), contextGuards(0), idValues(0), idValueCount(0), linkedContext(0),
  componentAttached(0), v4bindings(0), v8bindings(0)
{
}

void QDeclarativeContextData::invalidate()
{
    while (componentAttached) {
        QDeclarativeComponentAttached *a = componentAttached;
        componentAttached = a->next;
        if (componentAttached) componentAttached->prev = &componentAttached;

        a->next = 0;
        a->prev = 0;

        emit a->destruction();
    }

    while (childContexts) {
        if (childContexts->ownedByParent) {
            childContexts->destroy();
        } else {
            childContexts->invalidate();
        }
    }

    if (prevChild) {
        *prevChild = nextChild;
        if (nextChild) nextChild->prevChild = prevChild;
        nextChild = 0;
        prevChild = 0;
    }

    engine = 0;
    parent = 0;
}

void QDeclarativeContextData::clearContext()
{
    if (engine) {
        while (componentAttached) {
            QDeclarativeComponentAttached *a = componentAttached;
            componentAttached = a->next;
            if (componentAttached) componentAttached->prev = &componentAttached;

            a->next = 0;
            a->prev = 0;

            emit a->destruction();
        }
    }

    QDeclarativeAbstractExpression *expression = expressions;
    while (expression) {
        QDeclarativeAbstractExpression *nextExpression = expression->m_nextExpression;

        expression->m_context = 0;
        expression->m_prevExpression = 0;
        expression->m_nextExpression = 0;

        expression = nextExpression;
    }
    expressions = 0;
}

void QDeclarativeContextData::destroy()
{
    if (linkedContext)
        linkedContext->destroy();

    if (engine) invalidate();

    clearContext();

    while (contextObjects) {
        QDeclarativeData *co = contextObjects;
        contextObjects = contextObjects->nextContextObject;

        co->context = 0;
        co->outerContext = 0;
        co->nextContextObject = 0;
        co->prevContextObject = 0;
    }

    QDeclarativeGuardedContextData *contextGuard = contextGuards;
    while (contextGuard) {
        QDeclarativeGuardedContextData *next = contextGuard->m_next;
        contextGuard->m_next = 0;
        contextGuard->m_prev = 0;
        contextGuard->m_contextData = 0;
        contextGuard = next;
    }
    contextGuards = 0;

    if (propertyNames)
        propertyNames->release();

    if (imports)
        imports->release();

    if (v4bindings)
        v4bindings->release();

    if (v8bindings)
        v8bindings->release();

    for (int ii = 0; ii < importedScripts.count(); ++ii) {
        qPersistentDispose(importedScripts[ii]);
    }

    delete [] idValues;

    if (isInternal)
        delete publicContext;

    delete this;
}

void QDeclarativeContextData::setParent(QDeclarativeContextData *p, bool parentTakesOwnership)
{
    if (p) {
        parent = p;
        engine = p->engine;
        nextChild = p->childContexts;
        if (nextChild) nextChild->prevChild = &nextChild;
        prevChild = &p->childContexts;
        p->childContexts = this;
        ownedByParent = parentTakesOwnership;
    }
}

void QDeclarativeContextData::refreshExpressionsRecursive(QDeclarativeAbstractExpression *expression)
{
    QDeleteWatcher w(expression);

    if (expression->m_nextExpression)
        refreshExpressionsRecursive(expression->m_nextExpression);

    if (!w.wasDeleted())
        expression->refresh();
}

static inline bool expressions_to_run(QDeclarativeContextData *ctxt, bool isGlobalRefresh)
{
    return ctxt->expressions && (!isGlobalRefresh || ctxt->unresolvedNames);
}

void QDeclarativeContextData::refreshExpressionsRecursive(bool isGlobal)
{
    // For efficiency, we try and minimize the number of guards we have to create
    if (expressions_to_run(this, isGlobal) && (nextChild || childContexts)) {
        QDeclarativeGuardedContextData guard(this);

        if (childContexts)
            childContexts->refreshExpressionsRecursive(isGlobal);

        if (guard.isNull()) return;

        if (nextChild)
            nextChild->refreshExpressionsRecursive(isGlobal);

        if (guard.isNull()) return;

        if (expressions_to_run(this, isGlobal))
            refreshExpressionsRecursive(expressions);

    } else if (expressions_to_run(this, isGlobal)) {

        refreshExpressionsRecursive(expressions);

    } else if (nextChild && childContexts) {

        QDeclarativeGuardedContextData guard(this);

        childContexts->refreshExpressionsRecursive(isGlobal);

        if (!guard.isNull() && nextChild)
            nextChild->refreshExpressionsRecursive(isGlobal);

    } else if (nextChild) {

        nextChild->refreshExpressionsRecursive(isGlobal);

    } else if (childContexts) {

        childContexts->refreshExpressionsRecursive(isGlobal);

    }
}

// Refreshes all expressions that could possibly depend on this context.  Refreshing flushes all
// context-tree dependent caches in the expressions, and should occur every time the context tree
// *structure* (not values) changes.
void QDeclarativeContextData::refreshExpressions()
{
    bool isGlobal = (parent == 0);

    // For efficiency, we try and minimize the number of guards we have to create
    if (expressions_to_run(this, isGlobal) && childContexts) {
        QDeclarativeGuardedContextData guard(this);

        childContexts->refreshExpressionsRecursive(isGlobal);

        if (!guard.isNull() && expressions_to_run(this, isGlobal))
            refreshExpressionsRecursive(expressions);

    } else if (expressions_to_run(this, isGlobal)) {

        refreshExpressionsRecursive(expressions);

    } else if (childContexts) {

        childContexts->refreshExpressionsRecursive(isGlobal);

    }
}

void QDeclarativeContextData::addObject(QObject *o)
{
    QDeclarativeData *data = QDeclarativeData::get(o, true);

    Q_ASSERT(data->context == 0);

    data->context = this;
    data->outerContext = this;

    data->nextContextObject = contextObjects;
    if (data->nextContextObject)
        data->nextContextObject->prevContextObject = &data->nextContextObject;
    data->prevContextObject = &contextObjects;
    contextObjects = data;
}

void QDeclarativeContextData::setIdProperty(int idx, QObject *obj)
{
    idValues[idx] = obj;
    idValues[idx].context = this;
}

void QDeclarativeContextData::setIdPropertyData(QDeclarativeIntegerCache *data)
{
    Q_ASSERT(!propertyNames);
    propertyNames = data;
    propertyNames->addref();

    idValueCount = data->count();
    idValues = new ContextGuard[idValueCount];
}

QString QDeclarativeContextData::findObjectId(const QObject *obj) const
{
    if (!propertyNames)
        return QString();

    for (int ii = 0; ii < idValueCount; ii++) {
        if (idValues[ii] == obj)
            return propertyNames->findId(ii);
    }

    if (publicContext) {
        QDeclarativeContextPrivate *p = QDeclarativeContextPrivate::get(publicContext);
        for (int ii = 0; ii < p->propertyValues.count(); ++ii)
            if (p->propertyValues.at(ii) == QVariant::fromValue((QObject *)obj))
                return propertyNames->findId(ii);
    }

    if (linkedContext)
        return linkedContext->findObjectId(obj);
    return QString();
}

QDeclarativeContext *QDeclarativeContextData::asQDeclarativeContext()
{
    if (!publicContext)
        publicContext = new QDeclarativeContext(this);
    return publicContext;
}

QDeclarativeContextPrivate *QDeclarativeContextData::asQDeclarativeContextPrivate()
{
    return QDeclarativeContextPrivate::get(asQDeclarativeContext());
}

QT_END_NAMESPACE
