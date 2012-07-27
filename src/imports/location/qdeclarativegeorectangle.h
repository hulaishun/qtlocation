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
***************************************************************************/

#ifndef QDECLARATIVEGEORECTANGLE_H
#define QDECLARATIVEGEORECTANGLE_H

#include "qdeclarativegeoshape.h"
#include "qdeclarativecoordinate_p.h"

#include <QtQml/qqml.h>
#include <QtLocation/QGeoRectangle>

QT_BEGIN_NAMESPACE

class QDeclarativeGeoRectangle : public QDeclarativeGeoShape
{
    Q_OBJECT

    Q_PROPERTY(QGeoRectangle rectangle READ rectangle WRITE setRectangle)
    Q_PROPERTY(QGeoCoordinate bottomLeft READ bottomLeft WRITE setBottomLeft NOTIFY bottomLeftChanged)
    Q_PROPERTY(QGeoCoordinate bottomRight READ bottomRight WRITE setBottomRight NOTIFY bottomRightChanged)
    Q_PROPERTY(QGeoCoordinate topLeft READ topLeft WRITE setTopLeft NOTIFY topLeftChanged)
    Q_PROPERTY(QGeoCoordinate topRight READ topRight WRITE setTopRight NOTIFY topRightChanged)
    Q_PROPERTY(QGeoCoordinate center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(double width READ width WRITE setWidth NOTIFY widthChanged)

public:
    explicit QDeclarativeGeoRectangle(QObject *parent = 0);
    explicit QDeclarativeGeoRectangle(const QGeoRectangle &rectangle, QObject *parent = 0);
    void setRectangle(const QGeoRectangle &rectangle);
    QGeoRectangle rectangle() const;
    QGeoShape shape() const;

    Q_INVOKABLE bool contains(const QGeoCoordinate &coordinate);
    QGeoCoordinate bottomLeft();
    void setBottomLeft(const QGeoCoordinate &coordinate);
    QGeoCoordinate bottomRight();
    void setBottomRight(const QGeoCoordinate &coordinate);
    QGeoCoordinate topLeft();
    void setTopLeft(const QGeoCoordinate &coordinate);
    QGeoCoordinate topRight();
    void setTopRight(QGeoCoordinate &coordinate);
    QGeoCoordinate center();
    void setCenter(const QGeoCoordinate &coordinate);
    double height();
    void setHeight(double height);
    double width();
    void setWidth(double width);

Q_SIGNALS:
    void bottomLeftChanged();
    void bottomRightChanged();
    void topLeftChanged();
    void topRightChanged();
    void centerChanged();
    void heightChanged();
    void widthChanged();

private:
    void emitChanged(const QGeoRectangle &old);

    QGeoRectangle m_box;
    double m_width;
    double m_height;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeGeoRectangle)

#endif
