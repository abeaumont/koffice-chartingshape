/* This file is part of the KDE project

   Copyright 1999-2007  Kalle Dalheimer <kalle@kde.org>
   Copyright 2005-2007  Inge Wallin <inge@lysator.liu.se>

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


#ifndef KCHART_VIEW
#define KCHART_VIEW


#include <KoView.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QPaintEvent>

//#include "KDChartAxisParams.h"


class KAction;
class KToggleAction;
class QPaintEvent;


namespace KChart
{

class KChartPart;
class ViewAdaptor;

class KChartView : public KoView
{
    Q_OBJECT
public:
    explicit KChartView( KChartPart* part, QWidget* parent = 0 );
    ~KChartView();

    void updateGuiTypeOfChart();
    virtual ViewAdaptor* dbusObject();
    void config(int flag);

public slots:
    void  saveConfig();
    void  loadConfig();
    void  defaultConfig();

    void  pieChart();
    void  barsChart();
    void  lineChart();
    void  areasChart();
#if 0
    void  hiLoChart();
#endif
    void  ringChart();
    void  radarChart();
    void  stockChart();

    void  print(KPrinter &);
    void  setupPrinter(KPrinter &);

    void  slotRepaint();

    void  slotConfigPageLayout();
    void  importData();
    void  extraCreateTemplate();

protected:
    void          paintEvent( QPaintEvent* );

    virtual void  updateReadWrite( bool readwrite );

    virtual void  mousePressEvent ( QMouseEvent * );
    void          updateButton();

private:
    KAction  *m_importData;
    KAction  *m_edit;
    KAction  *m_saveconfig;
    KAction  *m_loadconfig;
    KAction  *m_defaultconfig;

    KToggleAction  *m_chartpie;
    KToggleAction  *m_chartareas;
    KToggleAction  *m_chartbars;
    KToggleAction  *m_chartline;
    KToggleAction  *m_charthilo;
    KToggleAction  *m_chartring;
    KToggleAction  *m_chartpolar;
    KToggleAction  *m_chartbw;
    ViewAdaptor *m_dbus;

    // This is used for a workaround for a bug in the kdchart code, see #101490.
    bool m_logarithmicScale;
    void forceAxisParams(bool lineMode);
};

}  //KChart namespace

#endif