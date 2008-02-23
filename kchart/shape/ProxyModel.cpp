/* This file is part of the KDE project

   Copyright 2007 Johannes Simon <johannes.simon@gmail.com>

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

// Local
#include "ProxyModel.h"
#include "DataSet.h"

// KDE
#include <KDebug>

namespace KChart {

class ProxyModel::Private {
public:
    Private();

    bool             firstRowIsLabel;
    bool             firstColumnIsLabel;
    Qt::Orientation  dataDirection;
    
    QList<DataSet*> dataSets;
};

ProxyModel::Private::Private()
{
    firstRowIsLabel    = false;
    firstColumnIsLabel = false;

    dataDirection = Qt::Vertical;
}

ProxyModel::ProxyModel( QObject *parent /* = 0 */ )
    : QAbstractProxyModel( parent ),
      d( new Private )
{
}

ProxyModel::~ProxyModel()
{
}

void ProxyModel::setSourceModel( QAbstractItemModel *sourceModel )
{
    connect( sourceModel, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ),
             this,        SLOT( dataChanged( const QModelIndex&, const QModelIndex& ) ) );

    QAbstractProxyModel::setSourceModel( sourceModel );

    // Update the entire data set
    reset();
}

QVariant ProxyModel::data( const QModelIndex &index,
                                int role /* = Qt::DisplayRole */ ) const
{
    if ( sourceModel() == 0 )
        return QVariant();

    return sourceModel()->data( mapToSource( index ), role );
}

bool ProxyModel::setData( const QModelIndex &index,
                                   const QVariant &data,
                                   int role /* = Qt::EditRole */ )
{
    if ( sourceModel() == 0 )
        return false;
    
    return sourceModel()->setData( mapToSource( index ), data, role );
}

void ProxyModel::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    emit QAbstractProxyModel::dataChanged( topLeft, bottomRight );
}

void ProxyModel::dataChanged()
{
    // Update the entire data set
    dataChanged( index( 0, 0 ), index( rowCount() - 1, columnCount() - 1 ) );
}

QVariant ProxyModel::headerData( int section,
                                      Qt::Orientation orientation,
                                      int role /* = Qt::DisplayRole */ ) const
{
    if ( sourceModel() == 0 )
        return QVariant();

    orientation = mapToSource( orientation );

    int row    = 0;
    int column = 0;

    if ( orientation == Qt::Vertical ) {
        if ( !d->firstColumnIsLabel )
            return QVariant();

        // Return the first column in the section-th row
        row = section;
        if ( d->firstRowIsLabel )
            row++;
    }
    else if ( orientation == Qt::Horizontal ) {
        if( !d->firstRowIsLabel )
            return QVariant();

        // Return the section-th column in the first row
        column = section;
        if ( d->firstColumnIsLabel )
            column++;
    }

    return sourceModel()->data( sourceModel()->index( row, column ), role );
}

QMap<int, QVariant> ProxyModel::itemData( const QModelIndex &index ) const
{
    return sourceModel()->itemData( mapToSource( index ) );
}

QModelIndex ProxyModel::index( int row,
                                    int column,
                                    const QModelIndex &parent /* = QModelIndex() */ ) const
{
    Q_UNUSED( parent );

    return QAbstractItemModel::createIndex( row, column, 0 );
}

QModelIndex ProxyModel::parent( const QModelIndex &index ) const
{
    Q_UNUSED( index );

    return QModelIndex();
}

QModelIndex ProxyModel::mapFromSource( const QModelIndex &sourceIndex ) const
{
    int row, column;
    if ( d->dataDirection == Qt::Vertical ) {
        row = sourceIndex.row();
        column = sourceIndex.column();

        if ( d->firstRowIsLabel )
            row--;
        if ( d->firstColumnIsLabel )
            column--;
    }
    else {
        row = sourceIndex.column();
        column = sourceIndex.row();

        if ( d->firstRowIsLabel )
            row--;
        if ( d->firstColumnIsLabel )
            column--;
    }
    return sourceModel()->index( row, column );
}

QModelIndex ProxyModel::mapToSource( const QModelIndex &proxyIndex ) const
{
    int row, column;
    if ( d->dataDirection == Qt::Vertical ) {
        row = proxyIndex.row();
        column = proxyIndex.column();

        if ( d->firstRowIsLabel )
            row++;
        if ( d->firstColumnIsLabel )
            column++;
    }
    else {
        row = proxyIndex.column();
        column = proxyIndex.row();

        if ( d->firstRowIsLabel )
            row++;
        if ( d->firstColumnIsLabel )
            column++;
    }
    return sourceModel()->index( row, column );
}

Qt::Orientation ProxyModel::mapFromSource( Qt::Orientation orientation ) const
{
    // In fact, this method does exactly the same thing as mapToSource( Qt::Orientation ),
    // but replacing the code with a call to mapToSource() would just confuse at this point.
    if ( d->dataDirection == Qt::Vertical )
        return orientation;

    // orientation is Qt::Horizontal
    // Thus, we need to return the opposite of orientation.
    if ( orientation == Qt::Vertical )
        return Qt::Horizontal;
    return Qt::Vertical;
}

Qt::Orientation ProxyModel::mapToSource( Qt::Orientation orientation ) const
{
    if ( d->dataDirection == Qt::Vertical )
        return orientation;

    // orientation is Qt::Horizontal
    // Thus, we need to return the opposite of orientation.
    if ( orientation == Qt::Vertical )
        return Qt::Horizontal;
    return Qt::Vertical;
}

int ProxyModel::rowCount( const QModelIndex &parent /* = QModelIndex() */ ) const
{
    if ( sourceModel() == 0 )
        return 0;

    int rowCount;
    if ( d->dataDirection == Qt::Vertical )
        rowCount = sourceModel()->rowCount( parent );
    else
        rowCount = sourceModel()->columnCount( parent );
    // Even if the first row is a header - if the data table is empty,
    // we still have 0 rows, not -1

    bool firstRowIsLabel = d->firstRowIsLabel;
    if ( d->dataDirection == Qt::Horizontal )
        firstRowIsLabel = d->firstColumnIsLabel;

    if ( rowCount > 0 && firstRowIsLabel )
        rowCount--;

    return rowCount;
}

int ProxyModel::columnCount( const QModelIndex &parent /* = QModelIndex() */ ) const
{
    if ( sourceModel() == 0 )
        return 0;

    int columnCount;
    if ( d->dataDirection == Qt::Vertical )
        columnCount = sourceModel()->columnCount( parent );
    else
        columnCount = sourceModel()->rowCount( parent );
    // Even if the first column is a header - if the data table is empty,
    // we still have 0 columns, not -1

    bool firstColumnIsLabel = d->firstColumnIsLabel;
    if ( d->dataDirection == Qt::Horizontal )
        firstColumnIsLabel = d->firstRowIsLabel;

    if ( columnCount > 0 && firstColumnIsLabel )
        columnCount--;

    return columnCount;
}

void ProxyModel::setFirstRowIsLabel( bool b )
{
    if ( b == d->firstRowIsLabel )
        return;
    
    if ( b ) {
        if ( d->dataDirection == Qt::Horizontal )
            beginRemoveColumns( QModelIndex(), 0, 0 );
        else
            beginRemoveRows( QModelIndex(), 0, 0 );
    } else {
        if ( d->dataDirection == Qt::Horizontal )
            beginInsertColumns( QModelIndex(), 0, 0 );
        else
            beginInsertRows( QModelIndex(), 0, 0 );
    }
    d->firstRowIsLabel = b;
    if ( b ) {
        if ( d->dataDirection == Qt::Horizontal )
            endRemoveColumns();
        else
            endRemoveRows();
    } else {
        if ( d->dataDirection == Qt::Horizontal )
            endInsertColumns();
        else
            endInsertRows();
    }
    
    dataChanged();
}
 
void ProxyModel::setFirstColumnIsLabel( bool b )
{
    if ( b == d->firstColumnIsLabel )
        return;
    
    if ( b ) {
        if ( d->dataDirection == Qt::Vertical )
            beginRemoveColumns( QModelIndex(), 0, 0 );
        else
            beginRemoveRows( QModelIndex(), 0, 0 );
    } else {
        if ( d->dataDirection == Qt::Vertical )
            beginInsertColumns( QModelIndex(), 0, 0 );
        else
            beginInsertRows( QModelIndex(), 0, 0 );
    }
    d->firstColumnIsLabel = b;
    if ( b ) {
        if ( d->dataDirection == Qt::Vertical )
            endRemoveColumns();
        else
            endRemoveRows();
    } else {
        if ( d->dataDirection == Qt::Vertical )
            endInsertColumns();
        else
            endInsertRows();
    }
    
    dataChanged();
}

Qt::Orientation ProxyModel::dataDirection()
{
    return d->dataDirection;
}

void ProxyModel::setDataDirection( Qt::Orientation orientation )
{
    
    d->dataDirection = orientation;
    reset();
    
    dataChanged();
}

bool ProxyModel::firstRowIsLabel() const
{
    return d->firstRowIsLabel;
}

bool ProxyModel::firstColumnIsLabel() const
{
    return d->firstColumnIsLabel;
}

QList<DataSet*> ProxyModel::dataSets() const
{
    return d->dataSets;
}

DataSet *ProxyModel::dataSet( int dataSet ) const
{
    if ( dataSet >= d->dataSets.size() )
        return 0;
    return d->dataSets[dataSet];
}

int ProxyModel::dataSet( DataSet *dataSet ) const
{
    return d->dataSets.indexOf( dataSet );
}

} // namespace KChart
