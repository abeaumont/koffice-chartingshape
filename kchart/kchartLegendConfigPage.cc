/*
 * Copyright 2001 by Laurent Montel, released under Artistic License.
 */

#include "kchartLegendConfigPage.h"
#include "kchartLegendConfigPage.moc"

#include <kapplication.h>
#include <klocale.h>
#include <kcolorbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include "kchart_params.h"
#include <kfontdialog.h>

KChartLegendConfigPage::KChartLegendConfigPage( KChartParams* params,
                                                QWidget* parent ) :
    QWidget( parent ),_params( params )
{
  QGridLayout* layout = new QGridLayout(this, 2, 2,15,7 );

  QButtonGroup* gb = new QButtonGroup( i18n("Legend Position"), this );
  QGridLayout *grid1 = new QGridLayout(gb,3,3,15,7);
  layout->addWidget(gb,0,0);


  noLegend=new QRadioButton(i18n("No legend"),gb);
  grid1->addWidget(noLegend,0,0);

  lTop=new QRadioButton(i18n("Top"),gb);
  grid1->addWidget(lTop,1,0);

  lBottom=new QRadioButton(i18n("Bottom"),gb);
  grid1->addWidget(lBottom,2,0);

  lLeft=new QRadioButton(i18n("Left"),gb);
  grid1->addWidget(lLeft,0,1);

  lRight=new QRadioButton(i18n("Right"),gb);
  grid1->addWidget(lRight,1,1);

  lTopLeft=new QRadioButton(i18n("Top-left"),gb);
  grid1->addWidget(lTopLeft,2,1);

  lTopRight=new QRadioButton(i18n("Top-right"),gb);
  grid1->addWidget(lTopRight,0,2);

  lBottomRight=new QRadioButton(i18n("Bottom-right"),gb);
  grid1->addWidget(lBottomRight,1,2);

  lBottomLeft=new QRadioButton(i18n("Bottom-left"),gb);
  grid1->addWidget(lBottomLeft,2,2);

  gb->setAlignment(Qt::AlignLeft);


  gb = new QButtonGroup( i18n("Title"), this );
  layout->addWidget(gb,1,0);

  QGridLayout *grid2 = new QGridLayout(gb,4,2,15,7);

  QLabel * lab=new QLabel(i18n("Legend title:"),gb);
  grid2->addWidget(lab,0,0);

  title=new QLineEdit(gb);
  grid2->addWidget(title,1,0);

  gb = new QButtonGroup( i18n("Color"), this );
  layout->addWidget(gb,0,1);

  QGridLayout *grid3 = new QGridLayout(gb,4,2,15,7);

  lab=new QLabel(i18n("Legend title color:"),gb);
  grid3->addWidget(lab,0,0);

  legendTitleColor=new KColorButton(gb);
  grid3->addWidget(legendTitleColor,1,0);


  lab=new QLabel(i18n("Legend text color:"),gb);
  grid3->addWidget(lab,2,0);

  legendTextColor=new KColorButton(gb);

  grid3->addWidget(legendTextColor,3,0);

  gb = new QButtonGroup( i18n("Font"), this );
  layout->addWidget(gb,1,1);

  QGridLayout *grid4 = new QGridLayout(gb,4,2,15,7);
  titleLegendFontButton=new QPushButton(gb);

  lab=new QLabel(i18n("Legend title font:"),gb);
  grid4->addWidget(lab,0,0);

  titleLegendFontButton->setText(i18n("Legend"));
  grid4->addWidget(titleLegendFontButton,1,0);

  lab=new QLabel(i18n("Legend text font:"),gb);
  grid4->addWidget(lab,2,0);
  textLegendFontButton=new QPushButton(gb);
  textLegendFontButton->setText(i18n("Text Legend"));
  grid4->addWidget(textLegendFontButton,3,0);

  connect( titleLegendFontButton, SIGNAL(clicked()), this, SLOT(changeTitleLegendFont()));
  connect( textLegendFontButton, SIGNAL(clicked()), this, SLOT(changeTextLegendFont()));

  //it's not good but I don't know how
  //to reduce space
  layout->addColSpacing(1,300);
}

void KChartLegendConfigPage::init()
{
    switch( _params->legendPosition())
    {
    case KDChartParams::NoLegend:
        noLegend->setChecked(true);
        break;
    case KDChartParams::LegendTop:
        lTop->setChecked(true);
        break;
    case KDChartParams::LegendBottom:
        lBottom->setChecked(true);
        break;
    case KDChartParams::LegendLeft:
        lLeft->setChecked(true);
        break;
    case KDChartParams::LegendRight:
        lRight->setChecked(true);
        break;
    case KDChartParams::LegendTopLeft:
        lTopLeft->setChecked(true);
        break;
    case KDChartParams::LegendTopRight:
        lTopRight->setChecked(true);
        break;
    case KDChartParams::LegendBottomRight:
        lBottomRight->setChecked(true);
        break;
    case  KDChartParams::LegendBottomLeft:
        lBottomLeft->setChecked(true);
        break;
    default:
        lRight->setChecked(true);
        break;
    }
    title->setText(_params->legendTitleText());
    legendTitleColor->setColor(_params->legendTitleTextColor());
    legendTextColor->setColor(_params->legendTextColor());

    titleLegend = _params->legendTitleFont();
    titleLegendIsRelative = _params->legendTitleFontUseRelSize()
                          ? QButton::On
                          : QButton::Off;
    if( QButton::On == titleLegendIsRelative )
        titleLegend.setPointSize( _params->legendTitleFontRelSize() );

    textLegend=_params->legendFont();
    textLegendIsRelative = _params->legendFontUseRelSize()
                         ? QButton::On
                         : QButton::Off;
    if( QButton::On == textLegendIsRelative )
        textLegend.setPointSize(_params->legendFontRelSize());
}

void KChartLegendConfigPage::changeTitleLegendFont()
{
    QButton::ToggleState state = titleLegendIsRelative;
    if (    KFontDialog::getFont( titleLegend,false,this, true,&state ) != QDialog::Rejected
         && QButton::NoChange != state )
        titleLegendIsRelative = state;
}

void KChartLegendConfigPage::changeTextLegendFont()
{
    QButton::ToggleState state = textLegendIsRelative;
    if (    KFontDialog::getFont( textLegend,false,this, true,&state ) != QDialog::Rejected
         && QButton::NoChange != state )
        textLegendIsRelative = state;
}

void KChartLegendConfigPage::apply()
{
    if(noLegend->isChecked())
        _params->setLegendPosition(KDChartParams::NoLegend);
    else if(lTop->isChecked())
        _params->setLegendPosition(KDChartParams::LegendTop);
    else if(lBottom->isChecked())
        _params->setLegendPosition(KDChartParams::LegendBottom);
    else if(lLeft->isChecked())
        _params->setLegendPosition(KDChartParams::LegendLeft);
    else if(lRight->isChecked())
        _params->setLegendPosition(KDChartParams::LegendRight);
    else if(lTopLeft->isChecked())
        _params->setLegendPosition(KDChartParams::LegendTopLeft);
    else if(lTopRight->isChecked())
        _params->setLegendPosition(KDChartParams::LegendTopRight);
    else if(lBottomRight->isChecked())
        _params->setLegendPosition(KDChartParams::LegendBottomRight);
    else if(lBottomLeft->isChecked())
        _params->setLegendPosition(KDChartParams::LegendBottomLeft);
    else
        _params->setLegendPosition(KDChartParams::LegendRight);

    _params->setLegendTitleText(title->text());
    _params->setLegendTitleTextColor(legendTitleColor->color());
    _params->setLegendTextColor(legendTextColor->color());

    _params->setLegendTitleFont(titleLegend, QButton::Off == titleLegendIsRelative);
    if( QButton::On == titleLegendIsRelative )
        _params->setLegendTitleFontRelSize(titleLegend.pointSize());
    _params->setLegendFont(textLegend, QButton::Off == textLegendIsRelative);
    if( QButton::On == textLegendIsRelative )
        _params->setLegendFontRelSize(textLegend.pointSize());

}
