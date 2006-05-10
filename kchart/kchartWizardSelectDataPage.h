#ifndef _kchartWIZARDSELECTDATAPAGE_H
#define _kchartWIZARDSELECTDATAPAGE_H

#include <QWidget>

class QLineEdit;

namespace KChart
{

class KChartWizardSelectDataPage : public QWidget
{
friend class KChartWizard;

public:
  KChartWizardSelectDataPage( QWidget* parent );

private:
  QLineEdit* rangeED;
};

}  //namespace KChart

#endif
