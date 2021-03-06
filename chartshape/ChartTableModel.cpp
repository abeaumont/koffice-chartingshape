/* This file is part of the KDE project

   Copyright 2008 Johannes Simon <johannes.simon@gmail.com>
   Copyright 2009 Inge Wallin    <inge@lysator.liu.se>

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


// Own
#include "ChartTableModel.h"

// C
#include <cmath>

// Qt
#include <QDomNode>
#include <QDomDocument>

// KDE
#include <KDebug>

// KOffice
#include <KXmlReader.h>
#include <KXmlWriter.h>
#include <KOdfGenericStyles.h>
#include <KOdfXmlNS.h>
#include <KOdfLoadingContext.h>
#include <KOdfStylesReader.h>
#include <KShapeLoadingContext.h>

// KChart
#include "CellRegion.h"
#include "OdfLoadingHelper.h"


ChartTableModel::ChartTableModel(QObject *parent /* = 0 */)
    : QStandardItemModel(parent)
{
}

ChartTableModel::~ChartTableModel()
{
}


QHash<QString, QVector<QRect> > ChartTableModel::cellRegion() const
{
    // FIXME: Unimplemented?
    return QHash<QString, QVector<QRect> >();
}

bool ChartTableModel::setCellRegion(const QString& /*regionName*/)
{
#if 0 // FIXME: What does this code do?
    int result = 0;

    const int size = regionName.size();
    for (int i = 0; i < size; i++) {
        result += (CellRegion::rangeCharToInt(regionName[i].toAscii())
                    * std::pow(10.0, (size - i - 1)));
    }

    return result;
#endif
    return true;
}

bool ChartTableModel::isCellRegionValid(const QString& regionName) const
{
    Q_UNUSED(regionName);

    return true;
}

bool ChartTableModel::loadOdf(const KXmlElement &tableElement,
                               KShapeLoadingContext &context)
{
    Q_UNUSED(context);

    setRowCount(0);
    setColumnCount(0);

    ///const QDomNode &node = tableElement.asQDomNode(QDomDocument());

    //QTextStream stream(stdout);
    //stream << node;

    // FIXME: Rewrite this without the for loop.  I think there can
    //        only be one table-rows and one table-header-rows element
    //        in each table.
    int           row = 0;
    KXmlElement  n;
    int           found = false;
    forEachElement (n, tableElement) {
        if (n.namespaceURI() != KOdfXmlNS::table)
            continue;

        if (n.localName() == "table-rows"
             || n.localName() == "table-header-rows")
        {
            found = true;

            KXmlElement  _n;
            forEachElement (_n, n) {

                // Must be a table:table-row, else go to next element.
                if (_n.namespaceURI() != KOdfXmlNS::table
                     || _n.localName() != "table-row")
                    continue;

                // Add a row to the internal representation.
                setRowCount(rowCount() + 1);

                // Loop through all cells in a table row.
                int           column = 0;
                KXmlElement  __n;
                forEachElement (__n, _n) {

                    // Must be a table:table-cell, otherwise go to
                    // next element.
                    if (__n.namespaceURI() == KOdfXmlNS::table
                         && __n.localName() == "table-cell")
                    {
//                         continue;

                    // If this row is wider than any previous one,
                    // then add another column.
                    // Is this efficient enough?
                    if (column >= columnCount())
                        setColumnCount(columnCount() + 1);

                    const QString valueType = __n.attributeNS(KOdfXmlNS::office, "value-type");

                    QString valueString = __n.attributeNS(KOdfXmlNS::office, "value");
                    const KXmlElement valueElement = __n.namedItemNS(KOdfXmlNS::text, "p").toElement();
                    if ((valueElement.isNull() || !valueElement.isElement()) && valueString.isEmpty())
                    {
                        qWarning() << "ChartTableModel::loadOdf(): Cell contains no valid <text:p> element, cannnot load cell data.";
                        // Even if it doesn't contain any value, it's still a cell.
                        column++;
                        continue;
                    }

                    // Read the actual value in the cell.
                    QVariant value;
                    if (valueString.isEmpty())
                        valueString = valueElement.text();
                    if (valueType == "float")
                        value = valueString.toDouble();
                    else if (valueType == "boolean")
                        value = (bool)valueString.toInt();
                    else // if (valueType == "string")
                        value = valueString;

                    setData(index(row, column), value);

                    ++column;
                    }

                } // foreach table:table-cell
                ++row;

            } // foreach table:table-row
        }
    }

    return found;
}

bool ChartTableModel::saveOdf(KXmlWriter &bodyWriter, KOdfGenericStyles &mainStyles) const
{
    Q_UNUSED(bodyWriter);
    Q_UNUSED(mainStyles);
    // The save logic is in ChartShape::saveOdf
    return true;
}
