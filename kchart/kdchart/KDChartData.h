/* -*- Mode: C++ -*-

  $Id$

  KDChart - a multi-platform charting engine

  Copyright (C) 2001 by Klar�lvdalens Datakonsult AB

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __KDCHARTDATA_H__
#define __KDCHARTDATA_H__

#include <qstring.h>
#include <qdatetime.h>
#ifdef unix
#include <values.h>
#else
#include <float.h>
#ifdef MINDOUBLE
#undef MINDOUBLE
#endif
#define MINDOUBLE DBL_MIN
#endif

class KDChartData
{
public:
    enum ValueType { NoValue, String, Double, DateTime };
    KDChartData( double value ) {
        dValue = value;
        _valueType = Double;
    }

    KDChartData( const QString& value ) :
        sValue( value ),
        _valueType( String )
    {}

    KDChartData() {
        _valueType = NoValue;
    }


    ValueType valueType() const
    {
        return _valueType;
    }
    bool hasValue() const
    {
        return _valueType != NoValue;
    }
    bool isString() const
    {
        return _valueType == String;
    }
    bool isDouble() const
    {
        return _valueType == Double;
    }
    bool isDateTime() const
    {
        return _valueType == DateTime;
    }

    bool operator==( const KDChartData& it ) const
    {
        bool bRet = hasValue() == it.hasValue();
        if ( bRet && hasValue() ) {
        bRet = valueType() == it.valueType();
            if ( bRet ) {
                switch ( valueType() ) {
                case String:
                    bRet = stringValue() == it.stringValue();
                    break;
                case Double:
                    bRet = doubleValue() == it.doubleValue();
                    break;
                case DateTime:
                    bRet = dateTimeValue()
                           == it.dateTimeValue();
                    break;
                default:
                    bRet = false;
                }
            }
        }
        return bRet;
    }

    void clearValue()
    {
        _valueType = NoValue;
    }

    QString stringValue() const
    {
        return isString() ? sValue : QString::null;
    }
    double doubleValue() const
    {
        return isDouble() ? dValue : MINDOUBLE;
    }
    QDateTime dateTimeValue() const
    {
        return isDateTime() ? dtValue : QDateTime();
    }

private:
    ValueType _valueType;
    QDateTime dtValue;
    double dValue;
    QString sValue; // dValue and sValue should be a union,
    // but QString has a non-default constructor
}
;

#endif