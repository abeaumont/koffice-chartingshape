/**
 * Kalle Dalheimer <kalle@kde.org>
 */

#ifndef KCHARTDOCUMENT_H
#define KCHARTDOCUMENT_H


#include "kchart_global.h"

#include <QPixmap>
#include <QStandardItemModel>

#include <kconfig.h>
#include <KoXmlReader.h>

#include <KoDocument.h>

#include "KDChartChart.h"

#include "kchart_export.h"


class QStandardItemModel;
class KoXmlWriter;
class KoGenStyles;


namespace KChart
{


class KCHART_EXPORT KChartDocument : public KoDocument
{
    Q_OBJECT

public:
    explicit KChartDocument(QWidget *parentWidget = 0,
                            QObject* parent = 0,
                            bool singleViewMode = false);
    ~KChartDocument();

    // Methods inherited from KoDocument:

    virtual void  paintContent( QPainter& painter, const QRect& rect);

    // Methods unique to KChart, and available in the new interface
    // (see /interfaces/koChart.h.)

    virtual void  resizeData( int rows, int columns );
    virtual void  setCellData( int row, int column, const QVariant &);
    virtual void  analyzeHeaders( );
    virtual void  setCanChangeValue( bool b )  { m_bCanChangeValue = b;    }

    // ----------------------------------------------------------------

    void  analyzeHeaders( const QStandardItemModel &data );
    void  doSetData( const QStandardItemModel &data,
		     bool  firstRowHeader,
		     bool  firstColHeader );

    void initLabelAndLegend();
    void loadConfig(KConfig *conf);
    void saveConfig(KConfig *conf);
    void defaultConfig();

    OdfChartType         chartType() const       { return m_type;       }
    QStandardItemModel  *data()                  { return m_currentData; }
    KDChart::Chart      *chart()     const       { return m_chart;      }

    // Data in rows or columns.
    DataDirection  dataDirection() const    { return m_dataDirection; }
    void           setDataDirection( DataDirection _dir ) {
	m_dataDirection = _dir;
    }

    // First row / column as data or label?
    bool       firstRowAsLabel() const { return m_firstRowAsLabel; }
    void       setFirstRowAsLabel( bool _val );
    bool       firstColAsLabel() const { return m_firstColAsLabel; }
    void       setFirstColAsLabel( bool _val );

    // 
    QStringList       &rowLabelTexts()         { return m_rowLabels;  }
    QStringList       &colLabelTexts()         { return m_colLabels;  }

    // Save and load
    virtual QDomDocument  saveXML();
    virtual bool          loadXML( QIODevice *, const KoXmlDocument& doc );
    virtual bool          loadOasis( const KoXmlDocument& doc,
				     KoOasisStyles& oasisStyles,
				     const KoXmlDocument& settings,
				     KoStore *store );
    virtual bool          saveOasis( KoStore* store,
                                     KoXmlWriter* manifestWriter );

    bool  canChangeValue()   const             { return m_bCanChangeValue; }

    void  initNullChart();

    // Functions that generate templates (not used yet):
    void  generateBarChartTemplate();

    virtual bool showEmbedInitDialog(QWidget* parent);

public slots:
    void  slotModified();
    virtual void initEmpty();

signals:
    void docChanged();

protected:
    //virtual KoView* createViewInstance( QWidget* parent );
    bool  loadOasisData( const KoXmlElement& tableElem );
    void  saveOasisData( KoXmlWriter* bodyWriter, KoGenStyles& mainStyles ) const;
    void writeAutomaticStyles( KoXmlWriter& contentWriter, KoGenStyles& mainStyles ) const;

private:
    // Helper methods for painting.
    int          createDisplayData();
    void         createLabelsAndLegend( QStringList  &longLabels,
					QStringList  &shortLabels );


    QDomElement  createElement(const QString &tagName,
			       const QFont &font,
			       QDomDocument &doc) const;
    QFont        toFont(QDomElement &element)     const;

    void         setChartDefaults();

private:
    // The chart and its contents
    OdfChartType             m_type;
    OdfChartSubtype          m_subtype;
    KDChart::Chart          *m_chart;
    QStandardItemModel      *m_currentData;

    // Info about the data.
    DataDirection  m_dataDirection; // Rows or Columns
    bool           m_firstRowAsLabel;
    bool           m_firstColAsLabel;

    QStringList              m_rowLabels;
    QStringList              m_colLabels;
    //QString                  m_regionName;

    // Other auxiliary values
    bool                     m_bCanChangeValue;

    // Graphics
    QWidget                 *m_parentWidget;

    // Used when displaying.
    QStandardItemModel       m_displayData;

    QPixmap                  m_bufferPixmap;
};


}  //KChart namespace

#endif