/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEGEOMAP_H
#define QDECLARATIVEGEOMAP_H

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

#include <QtLocation/private/qlocationglobal_p.h>
#include <QtLocation/private/qdeclarativegeomapitemview_p.h>
#include <QtLocation/private/qquickgeomapgesturearea_p.h>
#include <QtLocation/private/qdeclarativegeomapitemgroup_p.h>
#include <QtLocation/qgeoserviceprovider.h>
#include <QtLocation/private/qgeocameradata_p.h>
#include <QtQuick/QQuickItem>
#include <QtCore/QPointer>
#include <QtCore/QSet>
#include <QtGui/QColor>
#include <QtPositioning/qgeoshape.h>
#include <QtLocation/private/qgeomap_p.h>

QT_BEGIN_NAMESPACE

class QDeclarativeGeoServiceProvider;
class QDeclarativeGeoMapType;
class QDeclarativeGeoMapCopyrightNotice;
class QDeclarativeGeoMapParameter;

class Q_LOCATION_PRIVATE_EXPORT QDeclarativeGeoMap : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(QGeoServiceProvider::Error)
    Q_PROPERTY(QQuickGeoMapGestureArea *gesture READ gesture CONSTANT)
    Q_PROPERTY(QDeclarativeGeoServiceProvider *plugin READ plugin WRITE setPlugin NOTIFY pluginChanged)
    Q_PROPERTY(qreal minimumZoomLevel READ minimumZoomLevel WRITE setMinimumZoomLevel NOTIFY minimumZoomLevelChanged)
    Q_PROPERTY(qreal maximumZoomLevel READ maximumZoomLevel WRITE setMaximumZoomLevel NOTIFY maximumZoomLevelChanged)
    Q_PROPERTY(qreal zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)

    Q_PROPERTY(bool bearingSupported READ isBearingSupported NOTIFY bearingSupportChanged)
    Q_PROPERTY(bool tiltingSupported READ isTiltingSupported NOTIFY tiltingSupportChanged)
    Q_PROPERTY(qreal minimumTilt READ minimumTilt NOTIFY minimumTiltChanged)
    Q_PROPERTY(qreal maximumTilt READ maximumTilt NOTIFY maximumTiltChanged)
    Q_PROPERTY(qreal bearing READ bearing WRITE setBearing NOTIFY bearingChanged)
    Q_PROPERTY(qreal tilt READ tilt WRITE setTilt NOTIFY tiltChanged)
    Q_PROPERTY(qreal fieldOfView READ fieldOfView WRITE setFieldOfView NOTIFY fieldOfViewChanged)
    Q_PROPERTY(qreal minimumFieldOfView READ minimumFieldOfView NOTIFY minimumFieldOfViewChanged)
    Q_PROPERTY(qreal maximumFieldOfView READ maximumFieldOfView NOTIFY minimumFieldOfViewChanged)

    Q_PROPERTY(QDeclarativeGeoMapType *activeMapType READ activeMapType WRITE setActiveMapType NOTIFY activeMapTypeChanged)
    Q_PROPERTY(QQmlListProperty<QDeclarativeGeoMapType> supportedMapTypes READ supportedMapTypes NOTIFY supportedMapTypesChanged)
    Q_PROPERTY(QGeoCoordinate center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(QList<QObject *> mapItems READ mapItems NOTIFY mapItemsChanged)
    Q_PROPERTY(QList<QObject *> mapParameters READ mapParameters)
    Q_PROPERTY(QGeoServiceProvider::Error error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorChanged)
    Q_PROPERTY(QGeoShape visibleRegion READ visibleRegion WRITE setVisibleRegion)
    Q_PROPERTY(bool copyrightsVisible READ copyrightsVisible WRITE setCopyrightsVisible NOTIFY copyrightsVisibleChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_INTERFACES(QQmlParserStatus)

public:

    explicit QDeclarativeGeoMap(QQuickItem *parent = 0);
    ~QDeclarativeGeoMap();

    void setPlugin(QDeclarativeGeoServiceProvider *plugin);
    QDeclarativeGeoServiceProvider *plugin() const;

    void setActiveMapType(QDeclarativeGeoMapType *mapType);
    QDeclarativeGeoMapType *activeMapType() const;

    void setMinimumZoomLevel(qreal minimumZoomLevel);
    qreal minimumZoomLevel() const;

    void setMaximumZoomLevel(qreal maximumZoomLevel);
    qreal maximumZoomLevel() const;

    void setZoomLevel(qreal zoomLevel);
    qreal zoomLevel() const;

    void setBearing(qreal bearing);
    qreal bearing() const;

    void setTilt(qreal tilt);
    qreal tilt() const;

    void setFieldOfView(qreal fieldOfView);
    qreal fieldOfView() const;
    qreal minimumFieldOfView() const;
    qreal maximumFieldOfView() const;

    bool isBearingSupported() const;
    bool isTiltingSupported() const;
    qreal minimumTilt() const;
    qreal maximumTilt() const;

    void setCenter(const QGeoCoordinate &center);
    QGeoCoordinate center() const;

    void setVisibleRegion(const QGeoShape &shape);
    QGeoShape visibleRegion() const;

    void setCopyrightsVisible(bool visible);
    bool copyrightsVisible() const;

    void setColor(const QColor &color);
    QColor color() const;

    QQmlListProperty<QDeclarativeGeoMapType> supportedMapTypes();

    Q_INVOKABLE void removeMapItem(QDeclarativeGeoMapItemBase *item);
    Q_INVOKABLE void addMapItem(QDeclarativeGeoMapItemBase *item);

    Q_INVOKABLE void addMapItemGroup(QDeclarativeGeoMapItemGroup *itemGroup);
    Q_INVOKABLE void removeMapItemGroup(QDeclarativeGeoMapItemGroup *itemGroup);

    Q_INVOKABLE void clearMapItems();
    QList<QObject *> mapItems();

    Q_INVOKABLE void addMapParameter(QDeclarativeGeoMapParameter *parameter);
    Q_INVOKABLE void removeMapParameter(QDeclarativeGeoMapParameter *parameter);
    Q_INVOKABLE void clearMapParameters();
    QList<QObject *> mapParameters();

    Q_INVOKABLE QGeoCoordinate toCoordinate(const QPointF &position, bool clipToViewPort = true) const;
    Q_INVOKABLE QPointF fromCoordinate(const QGeoCoordinate &coordinate, bool clipToViewPort = true) const;

    QQuickGeoMapGestureArea *gesture();

    Q_INVOKABLE void fitViewportToMapItems();
    Q_INVOKABLE void fitViewportToVisibleMapItems();
    Q_INVOKABLE void pan(int dx, int dy);
    Q_INVOKABLE void prefetchData(); // optional hint for prefetch
    Q_INVOKABLE void clearData();

    QString errorString() const;
    QGeoServiceProvider::Error error() const;

Q_SIGNALS:
    void pluginChanged(QDeclarativeGeoServiceProvider *plugin);
    void zoomLevelChanged(qreal zoomLevel);
    void centerChanged(const QGeoCoordinate &coordinate);
    void activeMapTypeChanged();
    void supportedMapTypesChanged();
    void minimumZoomLevelChanged();
    void maximumZoomLevelChanged();
    void mapItemsChanged();
    void errorChanged();
    void copyrightLinkActivated(const QString &link);
    void copyrightsVisibleChanged(bool visible);
    void colorChanged(const QColor &color);
    void bearingChanged(qreal bearing);
    void tiltChanged(qreal tilt);
    void fieldOfViewChanged(qreal fieldOfView);
    void bearingSupportChanged(bool bearingSupport);
    void tiltingSupportChanged(bool tiltingSupport);
    void minimumTiltChanged(qreal minimumTilt);
    void maximumTiltChanged(qreal maximumTilt);
    void minimumFieldOfViewChanged(qreal minimumFieldOfView);
    void maximumFieldOfViewChanged(qreal maximumFieldOfView);
    void copyrightsChanged(const QImage &copyrightsImage);
    void copyrightsChanged(const QString &copyrightsHtml);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE ;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE ;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE ;
    void mouseUngrabEvent() Q_DECL_OVERRIDE ;
    void touchUngrabEvent() Q_DECL_OVERRIDE;
    void touchEvent(QTouchEvent *event) Q_DECL_OVERRIDE ;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE ;

    bool childMouseEventFilter(QQuickItem *item, QEvent *event) Q_DECL_OVERRIDE;
    bool sendMouseEvent(QMouseEvent *event);
    bool sendTouchEvent(QTouchEvent *event);

    void componentComplete() Q_DECL_OVERRIDE;
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) Q_DECL_OVERRIDE;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) Q_DECL_OVERRIDE;

    void setError(QGeoServiceProvider::Error error, const QString &errorString);
    void initialize();
private Q_SLOTS:
    void mappingManagerInitialized();
    void pluginReady();
    void onMapChildrenChanged();
    void onSupportedMapTypesChanged();

private:
    void setupMapView(QDeclarativeGeoMapItemView *view);
    void populateMap();
    void populateParameters();
    void fitViewportToMapItemsRefine(bool refine, bool onlyVisible);
    void fitViewportToGeoShape();
    bool isInteractive();

private:
    QDeclarativeGeoServiceProvider *m_plugin;
    QGeoMappingManager *m_mappingManager;
    QDeclarativeGeoMapType *m_activeMapType;
    QList<QDeclarativeGeoMapType *> m_supportedMapTypes;
    QList<QDeclarativeGeoMapItemView *> m_mapViews;
    QQuickGeoMapGestureArea *m_gestureArea;
    QGeoMap *m_map;
    QPointer<QDeclarativeGeoMapCopyrightNotice> m_copyrights;
    QList<QPointer<QDeclarativeGeoMapItemBase> > m_mapItems;
    QList<QPointer<QDeclarativeGeoMapItemGroup> > m_mapItemGroups;
    QString m_errorString;
    QGeoServiceProvider::Error m_error;
    QGeoRectangle m_visibleRegion;
    QColor m_color;
    QGeoCameraData m_cameraData;
    bool m_componentCompleted;
    bool m_pendingFitViewport;
    bool m_copyrightsVisible;
    double m_maximumViewportLatitude;
    bool m_initialized;
    QSet<QDeclarativeGeoMapParameter *> m_mapParameters;

    friend class QDeclarativeGeoMapItem;
    friend class QDeclarativeGeoMapItemView;
    friend class QQuickGeoMapGestureArea;
    friend class QDeclarativeGeoMapCopyrightNotice;
    Q_DISABLE_COPY(QDeclarativeGeoMap)
};


QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeGeoMap)

#endif
