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

#include "qquickpainteditem.h"
#include <private/qquickpainteditem_p.h>

#include <QtQuick/private/qsgpainternode_p.h>
#include <QtQuick/private/qsgcontext_p.h>
#include <private/qsgadaptationlayer_p.h>

#include <qmath.h>

QT_BEGIN_NAMESPACE

/*!
    \class QQuickPaintedItem
    \brief The QQuickPaintedItem class provides a way to use the QPainter API in the
    QML Scene Graph.

    \inmodule QtQuick

    The QQuickPaintedItem makes it possible to use the QPainter API with the QML Scene Graph.
    It sets up a textured rectangle in the Scene Graph and uses a QPainter to paint
    onto the texture. The render target can be either a QImage or a QOpenGLFramebufferObject.
    When the render target is a QImage, QPainter first renders into the image then
    the content is uploaded to the texture.
    When a QOpenGLFramebufferObject is used, QPainter paints directly onto the texture.
    Call update() to trigger a repaint.

    To enable QPainter to do anti-aliased rendering, use setAntialiasing().

    QQuickPaintedItem is meant to make it easier to port old code that is using the
    QPainter API to the QML Scene Graph API and it should be used only for that purpose.

    To write your own painted item, you first create a subclass of QQuickPaintedItem, and then
    start by implementing its only pure virtual public function: paint(), which implements
    the actual painting. To get the size of the area painted by the item, use
    contentsBoundingRect().
*/

/*!
    \enum QQuickPaintedItem::RenderTarget

    This enum describes QQuickPaintedItem's render targets. The render target is the
    surface QPainter paints onto before the item is rendered on screen.

    \value Image The default; QPainter paints into a QImage using the raster paint engine.
    The image's content needs to be uploaded to graphics memory afterward, this operation
    can potentially be slow if the item is large. This render target allows high quality
    anti-aliasing and fast item resizing.

    \value FramebufferObject QPainter paints into a QOpenGLFramebufferObject using the GL
    paint engine. Painting can be faster as no texture upload is required, but anti-aliasing
    quality is not as good as if using an image. This render target allows faster rendering
    in some cases, but you should avoid using it if the item is resized often.

    \value InvertedYFramebufferObject Exactly as for FramebufferObject above, except once
    the painting is done, prior to rendering the painted image is flipped about the
    x-axis so that the top-most pixels are now at the bottom.  Since this is done with the
    OpenGL texture coordinates it is a much faster way to achieve this effect than using a
    painter transform.

    \sa setRenderTarget()
*/

/*!
    \enum QQuickPaintedItem::PerformanceHint

    This enum describes flags that you can enable to improve rendering
    performance in QQuickPaintedItem. By default, none of these flags are set.

    \value FastFBOResizing If your item gets resized often and you are using the
    QQuickPaintedItem::FramebufferObject render target, set this flag to true to reduce the
    item resizing time at the cost of using more graphics memory. Resizing a Framebuffer object
    is a costly operation, by enabling this property the Framebuffer Object will use a texture
    larger than the actual size of the item to avoid as much as possible resizing it.
*/

/*!
    \internal
*/
QQuickPaintedItemPrivate::QQuickPaintedItemPrivate()
    : QQuickItemPrivate()
    , contentsScale(1.0)
    , fillColor(Qt::transparent)
    , renderTarget(QQuickPaintedItem::Image)
    , performanceHints(0)
    , opaquePainting(false)
    , antialiasing(false)
    , mipmap(false)
{
}

/*!
    Constructs a QQuickPaintedItem with the given \a parent item.
 */
QQuickPaintedItem::QQuickPaintedItem(QQuickItem *parent)
    : QQuickItem(*(new QQuickPaintedItemPrivate), parent)
{
    setFlag(ItemHasContents);
}

/*!
    \internal
*/
QQuickPaintedItem::QQuickPaintedItem(QQuickPaintedItemPrivate &dd, QQuickItem *parent)
    : QQuickItem(dd, parent)
{
    setFlag(ItemHasContents);
}

/*!
    Destroys the QQuickPaintedItem.
*/
QQuickPaintedItem::~QQuickPaintedItem()
{
}

/*!
    Schedules a redraw of the area covered by \a rect in this item. You can call this function
    whenever your item needs to be redrawn, such as if it changes appearance or size.

    This function does not cause an immediate paint; instead it schedules a paint request that
    is processed by the QML Scene Graph when the next frame is rendered. The item will only be
    redrawn if it is visible.

    Note that calling this function will trigger a repaint of the whole scene.

    \sa paint()
*/
void QQuickPaintedItem::update(const QRect &rect)
{
    Q_D(QQuickPaintedItem);

    if (rect.isNull() && !d->dirtyRect.isNull())
        d->dirtyRect = contentsBoundingRect().toAlignedRect();
    else
        d->dirtyRect |= (contentsBoundingRect() & rect).toAlignedRect();
    QQuickItem::update();
}

/*!
    Returns true if this item is opaque; otherwise, false is returned.

    By default, painted items are not opaque.

    \sa setOpaquePainting()
*/
bool QQuickPaintedItem::opaquePainting() const
{
    Q_D(const QQuickPaintedItem);
    return d->opaquePainting;
}

/*!
    If \a opaque is true, the item is opaque; otherwise, it is considered as translucent.

    Opaque items are not blended with the rest of the scene, you should set this to true
    if the content of the item is opaque to speed up rendering.

    By default, painted items are not opaque.

    \sa opaquePainting()
*/
void QQuickPaintedItem::setOpaquePainting(bool opaque)
{
    Q_D(QQuickPaintedItem);

    if (d->opaquePainting == opaque)
        return;

    d->opaquePainting = opaque;
    QQuickItem::update();
}

/*!
    Returns true if antialiased painting is enabled; otherwise, false is returned.

    By default, antialiasing is not enabled.

    \sa setAntialiasing()
*/
bool QQuickPaintedItem::antialiasing() const
{
    Q_D(const QQuickPaintedItem);
    return d->antialiasing;
}

/*!
    If \a enable is true, antialiased painting is enabled.

    By default, antialiasing is not enabled.

    \sa antialiasing()
*/
void QQuickPaintedItem::setAntialiasing(bool enable)
{
    Q_D(QQuickPaintedItem);

    if (d->antialiasing == enable)
        return;

    d->antialiasing = enable;
    update();
}

/*!
    Returns true if mipmaps are enabled; otherwise, false is returned.

    By default, mipmapping is not enabled.

    \sa setMipmap()
*/
bool QQuickPaintedItem::mipmap() const
{
    Q_D(const QQuickPaintedItem);
    return d->mipmap;
}

/*!
    If \a enable is true, mipmapping is enabled on the associated texture.

    Mipmapping increases rendering speed and reduces aliasing artifacts when the item is
    scaled down.

    By default, mipmapping is not enabled.

    \sa mipmap()
*/
void QQuickPaintedItem::setMipmap(bool enable)
{
    Q_D(QQuickPaintedItem);

    if (d->mipmap == enable)
        return;

    d->mipmap = enable;
    update();
}

/*!
    Returns the performance hints.

    By default, no performance hint is enabled/

    \sa setPerformanceHint(), setPerformanceHints()
*/
QQuickPaintedItem::PerformanceHints QQuickPaintedItem::performanceHints() const
{
    Q_D(const QQuickPaintedItem);
    return d->performanceHints;
}

/*!
    Sets the given performance \a hint on the item if \a enabled is true;
    otherwise clears the performance hint.

    By default, no performance hint is enabled/

    \sa setPerformanceHints(), performanceHints()
*/
void QQuickPaintedItem::setPerformanceHint(QQuickPaintedItem::PerformanceHint hint, bool enabled)
{
    Q_D(QQuickPaintedItem);
    PerformanceHints oldHints = d->performanceHints;
    if (enabled)
        d->performanceHints |= hint;
    else
        d->performanceHints &= ~hint;
    if (oldHints != d->performanceHints)
       update();
}

/*!
    Sets the performance hints to \a hints

    By default, no performance hint is enabled/

    \sa setPerformanceHint(), performanceHints()
*/
void QQuickPaintedItem::setPerformanceHints(QQuickPaintedItem::PerformanceHints hints)
{
    Q_D(QQuickPaintedItem);
    if (d->performanceHints == hints)
        return;
    d->performanceHints = hints;
    update();
}

/*!
    This function returns the outer bounds of the item as a rectangle; all painting must be
    restricted to inside an item's bounding rect.

    If the contents size has not been set it reflects the size of the item; otherwise
    it reflects the contents size scaled by the contents scale.

    Use this function to know the area painted by the item.

    \sa QQuickItem::width(), QQuickItem::height(), contentsSize(), contentsScale()
*/
QRectF QQuickPaintedItem::contentsBoundingRect() const
{
    Q_D(const QQuickPaintedItem);

    qreal w = d->width;
    QSizeF sz = d->contentsSize * d->contentsScale;
    if (w < sz.width())
        w = sz.width();
    qreal h = d->height;
    if (h < sz.height())
        h = sz.height();

    return QRectF(0, 0, w, h);
}

/*!
    \property QQuickPaintedItem::contentsSize
    \brief The size of the contents

    The contents size is the size of the item in regards to how it is painted
    using the paint() function.  This is distinct from the size of the
    item in regards to height() and width().
*/
QSize QQuickPaintedItem::contentsSize() const
{
    Q_D(const QQuickPaintedItem);
    return d->contentsSize;
}

void QQuickPaintedItem::setContentsSize(const QSize &size)
{
    Q_D(QQuickPaintedItem);

    if (d->contentsSize == size)
        return;

    d->contentsSize = size;
    update();
}

/*!
    This convenience function is equivalent to calling setContentsSize(QSize()).
*/
void QQuickPaintedItem::resetContentsSize()
{
    setContentsSize(QSize());
}

/*!
    \property QQuickPaintedItem::contentsScale
    \brief The scale of the contents

    All painting happening in paint() is scaled by the contents scale. This is distinct
    from the scale of the item in regards to scale().

    The default value is 1.
*/
qreal QQuickPaintedItem::contentsScale() const
{
    Q_D(const QQuickPaintedItem);
    return d->contentsScale;
}

void QQuickPaintedItem::setContentsScale(qreal scale)
{
    Q_D(QQuickPaintedItem);

    if (d->contentsScale == scale)
        return;

    d->contentsScale = scale;
    update();
}

/*!
    \property QQuickPaintedItem::fillColor
    \brief The item's background fill color.

    By default, the fill color is set to Qt::transparent.
*/
QColor QQuickPaintedItem::fillColor() const
{
    Q_D(const QQuickPaintedItem);
    return d->fillColor;
}

void QQuickPaintedItem::setFillColor(const QColor &c)
{
    Q_D(QQuickPaintedItem);

    if (d->fillColor == c)
        return;

    d->fillColor = c;
    update();

    emit fillColorChanged();
}

/*!
    \property QQuickPaintedItem::renderTarget
    \brief The item's render target.

    This property defines which render target the QPainter renders into, it can be either
    QSGPaintedItem::Image, QSGPaintedItem::FramebufferObject or QSGPaintedItem::InvertedYFramebufferObject.

    Each has certain benefits, typically performance versus quality. Using a framebuffer
    object avoids a costly upload of the image contents to the texture in graphics memory,
    while using an image enables high quality anti-aliasing.

    \warning Resizing a framebuffer object is a costly operation, avoid using
    the QQuickPaintedItem::FramebufferObject render target if the item gets resized often.

    By default, the render target is QQuickPaintedItem::Image.
*/
QQuickPaintedItem::RenderTarget QQuickPaintedItem::renderTarget() const
{
    Q_D(const QQuickPaintedItem);
    return d->renderTarget;
}

void QQuickPaintedItem::setRenderTarget(RenderTarget target)
{
    Q_D(QQuickPaintedItem);

    if (d->renderTarget == target)
        return;

    d->renderTarget = target;
    update();

    emit renderTargetChanged();
}

/*!
    \fn virtual void QQuickPaintedItem::paint(QPainter *painter) = 0

    This function, which is usually called by the QML Scene Graph, paints the
    contents of an item in local coordinates.

    The function is called after the item has been filled with the fillColor.

    Reimplement this function in a QQuickPaintedItem subclass to provide the
    item's painting implementation, using \a painter.

    \note The QML Scene Graph uses two separate threads, the main thread does things such as
    processing events or updating animations while a second thread does the actual OpenGL rendering.
    As a consequence, paint() is not called from the main GUI thread but from the GL enabled
    renderer thread. At the moment paint() is called, the GUI thread is blocked and this is
    therefore thread-safe.
*/

/*!
    This function is called when the Scene Graph node associated to the item needs to
    be updated.
*/
QSGNode *QQuickPaintedItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);
    Q_D(QQuickPaintedItem);

    if (width() <= 0 || height() <= 0) {
        delete oldNode;
        return 0;
    }

    QSGPainterNode *node = static_cast<QSGPainterNode *>(oldNode);
    if (!node)
        node = new QSGPainterNode(this);

    QRectF br = contentsBoundingRect();

    node->setPreferredRenderTarget(d->renderTarget);
    node->setFastFBOResizing(d->performanceHints & FastFBOResizing);
    node->setSize(QSize(qRound(br.width()), qRound(br.height())));
    node->setSmoothPainting(d->antialiasing);
    node->setLinearFiltering(d->smooth);
    node->setMipmapping(d->mipmap);
    node->setOpaquePainting(d->opaquePainting);
    node->setFillColor(d->fillColor);
    node->setContentsScale(d->contentsScale);
    node->setDirty(d->dirtyRect);
    node->update();

    d->dirtyRect = QRect();

    return node;
}

QT_END_NAMESPACE
