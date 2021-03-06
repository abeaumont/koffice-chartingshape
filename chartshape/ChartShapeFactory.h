/* This file is part of the KDE project
   Copyright 2007 Stefan Nikolaus <stefan.nikolaus@kdemail.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KCHART_CHART_SHAPE_FACTORY
#define KCHART_CHART_SHAPE_FACTORY


// Own
#include "ChartShape.h"

// Qt
#include <QStringList>

// KOffice
#include <KShapeFactoryBase.h>
#include <QVariantList>


class KShape;
class KShapeConfigWidgetBase;


class ChartShapePlugin : public QObject
{
    Q_OBJECT
public:

    ChartShapePlugin(QObject * parent,  const QVariantList &);
    ~ChartShapePlugin() {}
};


class ChartShapeFactory : public KShapeFactoryBase
{
    Q_OBJECT
public:
    ChartShapeFactory(QObject* parent);
    ~ChartShapeFactory() {}

    bool supports(const KXmlElement &element, KShapeLoadingContext &context) const;

    virtual KShape *createDefaultShape(KResourceManager *documentResources = 0) const;
    virtual void newDocumentResourceManager(KResourceManager *manager);

    QList<KShapeConfigWidgetBase*> createShapeOptionPanels();
};


#endif // KCHART_CHART_SHAPE_FACTORY
