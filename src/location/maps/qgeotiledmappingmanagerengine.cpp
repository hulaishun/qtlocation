/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtLocation module of the Qt Toolkit.
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

#include "qgeotiledmappingmanagerengine.h"
#include "qgeotiledmappingmanagerengine_p.h"
#include "qgeotilefetcher.h"


#include "qgeotiledmapdata_p.h"
#include "qgeotilecache_p.h"
#include "qgeotilespec.h"

#include <QTimer>
#include <QNetworkProxy>
#include <QLocale>

QT_BEGIN_NAMESPACE

QGeoTiledMappingManagerEngine::QGeoTiledMappingManagerEngine(QObject *parent)
    : QGeoMappingManagerEngine(parent),
      d_ptr(new QGeoTiledMappingManagerEnginePrivate)
{
}

/*!
    Destroys this mapping manager.
*/
QGeoTiledMappingManagerEngine::~QGeoTiledMappingManagerEngine()
{
    delete d_ptr;
}


void QGeoTiledMappingManagerEngine::setTileFetcher(QGeoTileFetcher *fetcher)
{
    Q_D(QGeoTiledMappingManagerEngine);

    d->fetcher_ = fetcher;
    d->fetcher_->init();

    d->thread_= new QThread;

    qRegisterMetaType<QGeoTileSpec>();

    connect(d->fetcher_,
            SIGNAL(tileFinished(QGeoTileSpec,QByteArray,QString)),
            this,
            SLOT(engineTileFinished(QGeoTileSpec,QByteArray,QString)),
            Qt::QueuedConnection);
    connect(d->fetcher_,
            SIGNAL(tileError(QGeoTileSpec,QString)),
            this,
            SLOT(engineTileError(QGeoTileSpec,QString)),
            Qt::QueuedConnection);

    d->fetcher_->moveToThread(d_ptr->thread_);

    connect(d->thread_,
            SIGNAL(started()),
            d->fetcher_,
            SLOT(threadStarted()));

    connect(d->thread_,
            SIGNAL(finished()),
            d->fetcher_,
            SLOT(threadFinished()));

    connect(d->fetcher_,
            SIGNAL(destroyed()),
            d->thread_,
            SLOT(deleteLater()));

    engineInitialized();

    QTimer::singleShot(0, d->thread_, SLOT(start()));
}

QGeoTileFetcher *QGeoTiledMappingManagerEngine::tileFetcher()
{
    Q_D(QGeoTiledMappingManagerEngine);
    return d->fetcher_;
}

QGeoMap* QGeoTiledMappingManagerEngine::createMap(QObject *parent)
{
    Q_UNUSED(parent);
    return NULL;
}

void QGeoTiledMappingManagerEngine::registerMap(QGeoTiledMapData *map)
{
    QSet<QGeoTiledMapData*> maps = d_ptr->caches_.value(tileCache());
    maps.insert(map);
    d_ptr->caches_.insert(tileCache(), maps);
}

void QGeoTiledMappingManagerEngine::deregisterMap(QGeoTiledMapData *map)
{
    QGeoTileCache *cache = map->tileCache();
    QSet<QGeoTiledMapData*> maps = d_ptr->caches_.value(cache);
    maps.remove(map);
    if (maps.isEmpty()) {
        d_ptr->caches_.remove(cache);
    } else {
        d_ptr->caches_.insert(cache, maps);
    }

    d_ptr->mapHash_.remove(map);

    QHash<QGeoTileSpec, QSet<QGeoTiledMapData*> > newTileHash = d_ptr->tileHash_;
    typedef QHash<QGeoTileSpec, QSet<QGeoTiledMapData*> >::const_iterator h_iter;
    h_iter hi = d_ptr->tileHash_.constBegin();
    h_iter hend = d_ptr->tileHash_.constEnd();
    for (; hi != hend; ++hi) {
        QSet<QGeoTiledMapData*> maps = hi.value();
        if (maps.contains(map)) {
            maps.remove(map);
            if (maps.isEmpty())
                newTileHash.remove(hi.key());
            else
                newTileHash.insert(hi.key(), maps);
        }
    }
    d_ptr->tileHash_ = newTileHash;
}

void QGeoTiledMappingManagerEngine::updateTileRequests(QGeoTiledMapData *map,
                                            const QSet<QGeoTileSpec> &tilesAdded,
                                            const QSet<QGeoTileSpec> &tilesRemoved)
{
    Q_D(QGeoTiledMappingManagerEngine);

    typedef QSet<QGeoTileSpec>::const_iterator tile_iter;

    // add and remove tiles from tileset for this map

    QSet<QGeoTileSpec> oldTiles = d->mapHash_.value(map);

    tile_iter rem = tilesRemoved.constBegin();
    tile_iter remEnd = tilesRemoved.constEnd();
    for (; rem != remEnd; ++rem) {
        oldTiles.remove(*rem);
    }

    tile_iter add = tilesAdded.constBegin();
    tile_iter addEnd = tilesAdded.constEnd();
    for (; add != addEnd; ++add) {
        oldTiles.insert(*add);
    }

    d->mapHash_.insert(map, oldTiles);

    // add and remove map from mapset for the tiles

    QSet<QGeoTileSpec> reqTiles;
    QSet<QGeoTileSpec> cancelTiles;

    rem = tilesRemoved.constBegin();
    for (; rem != remEnd; ++rem) {
        QSet<QGeoTiledMapData*> mapSet = d->tileHash_.value(*rem);
        mapSet.remove(map);
        if (mapSet.isEmpty()) {
            cancelTiles.insert(*rem);
            d->tileHash_.remove(*rem);
        } else {
            d->tileHash_.insert(*rem, mapSet);
        }
    }

    add = tilesAdded.constBegin();
    for (; add != addEnd; ++add) {
        QSet<QGeoTiledMapData*> mapSet = d->tileHash_.value(*add);
        if (mapSet.isEmpty()) {
            reqTiles.insert(*add);
        }
        mapSet.insert(map);
        d->tileHash_.insert(*add, mapSet);
    }

    cancelTiles -= reqTiles;

    QMetaObject::invokeMethod(d->fetcher_, "updateTileRequests",
                              Qt::QueuedConnection,
                              Q_ARG(QSet<QGeoTileSpec>, reqTiles),
                              Q_ARG(QSet<QGeoTileSpec>, cancelTiles));
}

void QGeoTiledMappingManagerEngine::engineTileFinished(const QGeoTileSpec &spec, const QByteArray &bytes, const QString &format)
{
    Q_D(QGeoTiledMappingManagerEngine);

    QSet<QGeoTileCache*> caches;

    QSet<QGeoTiledMapData*> maps = d->tileHash_.value(spec);

    typedef QSet<QGeoTiledMapData*>::const_iterator map_iter;

    map_iter map = maps.constBegin();
    map_iter mapEnd = maps.constEnd();
    for (; map != mapEnd; ++map) {
        caches.insert((*map)->tileCache());

        QSet<QGeoTileSpec> tileSet = d->mapHash_.value(*map);
        tileSet.remove(spec);
        if (tileSet.isEmpty())
            d->mapHash_.remove(*map);
        else
            d->mapHash_.insert(*map, tileSet);
    }

    d->tileHash_.remove(spec);

    typedef QSet<QGeoTileCache*>::const_iterator cache_iter;

    cache_iter cache = caches.constBegin();
    cache_iter cacheEnd = caches.constEnd();
    for (; cache != cacheEnd; ++cache) {
        (*cache)->insert(spec, bytes, format, d->cacheHint_);
    }

    map = maps.constBegin();
    for (; map != mapEnd; ++map) {
        (*map)->tileFetched(spec);
    }
}

void QGeoTiledMappingManagerEngine::engineTileError(const QGeoTileSpec &spec, const QString &errorString)
{
    emit tileError(spec, errorString);
}

void QGeoTiledMappingManagerEngine::setTileSize(const QSize &tileSize)
{
    Q_D(QGeoTiledMappingManagerEngine);
    d->tileSize_ = tileSize;
}

QSize QGeoTiledMappingManagerEngine::tileSize() const
{
    Q_D(const QGeoTiledMappingManagerEngine);
    return d->tileSize_;
}

QGeoTiledMappingManagerEngine::CacheAreas QGeoTiledMappingManagerEngine::cacheHint() const
{
    Q_D(const QGeoTiledMappingManagerEngine);
    return d->cacheHint_;
}

void QGeoTiledMappingManagerEngine::setCacheHint(QGeoTiledMappingManagerEngine::CacheAreas cacheHint)
{
    Q_D(QGeoTiledMappingManagerEngine);
    d->cacheHint_ = cacheHint;
}

QGeoTileCache *QGeoTiledMappingManagerEngine::createTileCacheWithDir(const QString &cacheDirectory)
{
    Q_D(QGeoTiledMappingManagerEngine);
    Q_ASSERT_X(!d->tileCache_, Q_FUNC_INFO, "This should be called only once");
    d->tileCache_ = new QGeoTileCache(cacheDirectory);
    return d->tileCache_;
}

QGeoTileCache *QGeoTiledMappingManagerEngine::tileCache()
{
    Q_D(QGeoTiledMappingManagerEngine);
    if (!d->tileCache_)
        d->tileCache_ = new QGeoTileCache();
    return d->tileCache_;
}

/*******************************************************************************
*******************************************************************************/

QGeoTiledMappingManagerEnginePrivate::QGeoTiledMappingManagerEnginePrivate()
  : thread_(0),
    cacheHint_(QGeoTiledMappingManagerEngine::AllCaches),
    tileCache_(0),
    fetcher_(0) {}

QGeoTiledMappingManagerEnginePrivate::~QGeoTiledMappingManagerEnginePrivate()
{
    if (tileCache_)
        delete tileCache_;

    if (fetcher_)
        delete fetcher_;

    thread_->quit();
}

QT_END_NAMESPACE