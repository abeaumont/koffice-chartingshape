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

#ifndef __KDCHARTNOTENOUGHSPACEEXCEPTION_H__
#define __KDCHARTNOTENOUGHSPACEEXCEPTION_H__

#ifdef USE_EXCEPTIONS

#include <qstring.h>

/**
   This exception is thrown when there is not enough space for
   drawing all selected parts of a chart.
*/
class KDChartNotEnoughSpaceException
{
public:
    KDChartNotEnoughSpaceException();
};

#endif

#endif