/* -*- Mode: C++ -*-

  $Id$

  KDChart - a multi-platform charting engine

  Copyright (C) 2001 by Klarälvdalens Datakonsult AB
*/

#include <KDChartParams.h>
#include <KDChartAxisParams.h>

class KDChartData;

#include <qdom.h>

const uint KDChartParams::KDCHART_MAX_AXES = 12;
const uint KDChartParams::KDCHART_ALL_AXES = UINT_MAX-1;
const uint KDChartParams::KDCHART_NO_AXIS = UINT_MAX;
const uint KDChartParams::KDCHART_ALL_DATASETS = UINT_MAX - 1;
const uint KDChartParams::KDCHART_NO_DATASET = UINT_MAX;
const uint KDChartParams::KDCHART_UNKNOWN_CHART = UINT_MAX - 2;
const uint KDChartParams::KDCHART_ALL_CHARTS = UINT_MAX - 1;
const uint KDChartParams::KDCHART_NO_CHART = UINT_MAX;

static QString penStyleToString( Qt::PenStyle style );
static Qt::PenStyle stringToPenStyle( const QString& style );

static void createBoolNode( QDomDocument& doc, QDomNode& parent,
                            const QString& elementName, bool value );
static void createSizeNode( QDomDocument& doc, QDomNode& parent,
                            const QString& elementName, const QSize& value );
static void createIntNode( QDomDocument& doc, QDomNode& parent,
                           const QString& elementName, int value );
static void createDoubleNode( QDomDocument& doc, QDomNode& parent,
                              const QString& elementName, double value );
static void createStringNode( QDomDocument& doc, QDomNode& parent,
                              const QString& elementName,
                              const QString& text );
static void createColorNode( QDomDocument& doc, QDomNode& parent,
                             const QString& elementName, const QColor& color );
static void createRectNode( QDomDocument& doc, QDomNode& parent,
                            const QString& elementName, const QRect& rect );
static void createStringListNodes( QDomDocument& doc, QDomNode& parent,
                                   const QString& elementName,
                                   const QStringList* list );
static void createChartValueNode( QDomDocument& doc, QDomNode& parent,
                                  const QString& elementName,
                                  const KDChartData& data );
static void createColorMapNode( QDomDocument& doc, QDomNode& parent,
                                const QString& elementName,
                                const QMap< uint, QColor >& map );
static void createDoubleMapNode( QDomDocument& doc, QDomNode& parent,
                                 const QString& elementName,
                                 const QMap< int, double >& map );
static void createChartFontNode( QDomDocument& doc, QDomNode& parent,
                                 const QString& elementName,
                                 const QFont& font, bool useRelFont,
                                 int relFont );
static void createFontNode( QDomDocument& doc, QDomNode& parent,
                            const QString& elementName, const QFont& font );


static bool readIntNode( const QDomElement& element, int& value );
static bool readStringNode( const QDomElement& element, QString& value );
static bool readDoubleNode( const QDomElement& element, double& value );
static bool readBoolNode( const QDomElement& element, bool& value );
static bool readColorNode( const QDomElement& element, QColor& value );
static bool readColorMapNode( const QDomElement& element, QMap<uint,QColor>* value );
static bool readDoubleMapNode( const QDomElement& element, QMap<int,double>* value );
static bool readRectNode( const QDomElement& element, QRect& value );
static bool readFontNode( const QDomElement& element, QFont& font );
static bool readChartFontNode( const QDomElement& element, QFont& font,
                               bool& useRelFont, int& relFontSize );
static bool readChartValueNode( const QDomElement& element,
                                KDChartData& value );

KDChartParams::KDChartParams()
{
    // GENERAL

    // Default type is bar charts
    setChartType( Bar );

    // By default, there is no additional chart type
    setAdditionalChartType( NoType );

    // Default is to show all values.
    setNumValues( -1 );

    // Avoid Purify UMR
    _maxDatasetColor = 0;
    _maxDatasetSourceMode = 0;
    _setChartSourceModeWasUsed = false;

    // The default brightness of shadow colors (needs to be set
    // before the data colors to avoid an UMR).
    setShadowBrightnessFactor( 1.0 );

    // Some default colors for the data.
    setDataColor( 0, red );
    setDataColor( 1, green );
    setDataColor( 2, blue );
    setDataColor( 3, cyan );
    setDataColor( 4, magenta );
    setDataColor( 5, yellow );
    setDataColor( 6, darkRed );
    setDataColor( 7, darkGreen );
    setDataColor( 8, darkBlue );
    setDataColor( 9, darkCyan );
    setDataColor( 10, darkMagenta );
    setDataColor( 11, darkYellow );

    // Default color for data display outlines.
    setOutlineDataColor( black );

    // Default line width for data display outlines.
    setOutlineDataLineWidth( 1 );

    // Default line style for data display outlines.
    setOutlineDataLineStyle( Qt::SolidLine );

    // END GENERAL



    // BAR CHART-SPECIFIC

    // Default bar subtype is normal
    setBarChartSubType( BarNormal );

    // Default is not to draw 3D bars
    setThreeDBars( false );

    // Default is to used shadowed colors for 3D bar effects
    setThreeDBarsShadowColors( true );

    // Default angle for 3D bars is 45 degrees.
    setThreeDBarAngle( 45 );


    // Default gap between datasets is 6 per mille of chart data area.
    setDatasetGap( 6 );
    setDatasetGapIsRelative( true );

    // Default gap between value blocks is 15 per mille of chart data area.
    setValueBlockGap( 15 );
    setValueBlockGapIsRelative( true );

    // END BAR CHART-SPECIFIC


    // LINE/AREA CHART-SPECIFIC
    // Normal lines by default
    setLineChartSubType( LineNormal );

    // No markers by default
    setLineMarker( false );

    // Lines have a width of 1 pixel by default
    setLineWidth( 1 );

    // Default line marker styles
    _maxDatasetLineMarkerStyle = 0; // avoid UMR
    setLineMarkerStyle( 0, LineMarkerCircle );
    setLineMarkerStyle( 1, LineMarkerSquare );
    setLineMarkerStyle( 2, LineMarkerDiamond );

    // Normal areas by default
    setAreaChartSubType( AreaNormal );

    // Areas are filled below the value points by default.
    setAreaLocation( AreaBelow );

    // END LINE/AREA CHART-SPECIFIC


    // PIE/RING CHART-SPECIFIC
    // Pie/ring charts do not explode by default.
    setExplode( false );
    // But if they do, the explode factor is 10%
    setExplodeFactor( 0.1 );
    // setExplodeValues() is not called for initialization, the default
    // empty list is the default.
    // Flat pies by default
    setThreeDPies( false );
    // Height of 3D effect
    setThreeDPieHeight( 20 );
    // Start of pie circle
    setPieStart( 0 );
    // Start of ring circle
    setRingStart( 0 );
    // Ring thickness is constant by default
    setRelativeRingThickness( false );
    // END PIE/RING CHART-SPECIFIC


    // HILO CHART-SPECIFIC
    // Simple HiLo charts by default
    setHiLoChartSubType( KDChartParams::HiLoSimple );
    setHiLoChartPrintLowValues( false );
    setHiLoChartPrintHighValues( false );

    // GANTT CHART-SPECIFIC
    // Simple Gantt charts by default
    setGanttChartSubType( KDChartParams::GanttNormal );

    // LEGEND
    // Distance between legend and data.
    setLegendSpacing( 20 );
    // Position of the legend
    setLegendPosition( LegendRight );
    // Where the legend labels come from
    setLegendSource( LegendAutomatic );
    // legend texts are drawn in black by default
    setLegendTextColor( Qt::black );
    // legend font size is calculated dynamically, but ignore the font size
    setLegendFont( QFont( "helvetica", 10, QFont::Normal, false ), false );
    // legend font size is calculated dynamically:
    //                    20 / 1000 of the average value of
    //                                 the printable area height and width
    setLegendFontRelSize( 20 );
    // the default legend title is "Legend"
    setLegendTitleText( tr( "Legend" ) );
    // legend title is drawn in black by default
    setLegendTitleTextColor( Qt::black );
    // legend title font size is calculated dynamically, but ignore
    // the font size
    setLegendTitleFont( QFont( "helvetica", 12, QFont::Normal, false ), false );
    // legend title font size is calculated dynamically:
    //                    25 / 1000 of the average value of
    //                                 the printable area height and width
    setLegendTitleFontRelSize( 25 );
    // END LEGEND


    // AXES

    setDefaultAxesTypes();
    // activate bottom (ordinate) and left (abcissa) axis
    activateDefaultAxes();

    // END AXES


    // HEADERS/FOOTERS

    // Set a default font for all sections not taking the build-in
    // defaults from c'tor KDChartParams::HdFtParams::HdFtParams()
    QFont defaultHdFtFont( "helvetica", 14, QFont::Normal, false );
    int relHdSize = 28;
    int relHd2Size = 22;
    int relFtSize = 22;
    int relFt2Size = 16;
    setHeaderFooterFont( KDChartParams::HdFtPosHeader,
                         defaultHdFtFont, true, relHdSize );
    setHeaderFooterFont( KDChartParams::HdFtPosHeaderA,
                         defaultHdFtFont, true, relHdSize );
    setHeaderFooterFont( KDChartParams::HdFtPosHeaderB,
                         defaultHdFtFont, true, relHdSize );
    setHeaderFooterFont( KDChartParams::HdFtPosHeader2,
                         defaultHdFtFont, true, relHd2Size );
    setHeaderFooterFont( KDChartParams::HdFtPosHeader2A,
                         defaultHdFtFont, true, relHd2Size );
    setHeaderFooterFont( KDChartParams::HdFtPosHeader2B,
                         defaultHdFtFont, true, relHd2Size );
    setHeaderFooterFont( KDChartParams::HdFtPosFooter,
                         defaultHdFtFont, true, relFtSize );
    setHeaderFooterFont( KDChartParams::HdFtPosFooterA,
                         defaultHdFtFont, true, relFtSize );
    setHeaderFooterFont( KDChartParams::HdFtPosFooterB,
                         defaultHdFtFont, true, relFtSize );

    // END HEADERS/FOOTERS
}


KDChartParams::~KDChartParams()
{
    // Intentionally left blank for now.
}


void KDChartParams::setChartSourceMode( SourceMode mode,
                                        uint dataset,
                                        uint dataset2,
                                        uint chart )
{
    if ( KDCHART_NO_DATASET != dataset
         && KDCHART_ALL_DATASETS != dataset
         && KDCHART_ALL_DATASETS != dataset2 ) {
        uint i;
        uint last = ( KDCHART_NO_DATASET == dataset2 ) ? dataset : dataset2;
        for ( i = dataset; i <= last; ++i )
            _dataSourceModeAndChart[ i ] = ModeAndChart( mode, chart );
        _maxDatasetSourceMode = QMAX( _maxDatasetSourceMode, --i );
        _setChartSourceModeWasUsed = true;
    } else if ( UnknownMode == mode && dataset == KDCHART_ALL_DATASETS ) {
        _dataSourceModeAndChart.clear();
        _setChartSourceModeWasUsed = false;
    }

    emit changed();
}

KDChartParams::SourceMode KDChartParams::chartSourceMode( uint dataset,
                                                          uint dataset2,
                                                          uint* pChart ) const
{
    uint chart = KDCHART_UNKNOWN_CHART;
    SourceMode mode = UnknownMode;

    if ( _setChartSourceModeWasUsed ) {
        if (    dataset <= _maxDatasetSourceMode
                && (    KDCHART_NO_DATASET == dataset2
                        || dataset2 <= _maxDatasetSourceMode ) ) {

            uint a, b;
            if ( KDCHART_ALL_DATASETS == dataset ) {
                a = 0;
                b = UINT_MAX;
            } else {
                a = dataset;
                b = KDCHART_NO_DATASET == dataset2 ? a : dataset2;
            }

            bool bStart = true;
            ModeAndChartMap::ConstIterator it;
            for ( it = _dataSourceModeAndChart.find( a );
                  ( it != _dataSourceModeAndChart.end() ) && ( it.key() <= b );
                  ++it ) {
                if ( bStart ) {
                    mode = it.data().mode();
                    chart = it.data().chart();
                    bStart = false;
                } else {
                    if ( mode != it.data().mode() )
                        mode = UnknownMode;
                    if ( chart != it.data().chart() )
                        chart = KDCHART_UNKNOWN_CHART;
                }
            }
        }
    } else {
        mode = DataEntry;
        chart = 0;
    }
    if ( pChart )
        * pChart = chart;
    return mode;
}

bool KDChartParams::findDataset( SourceMode mode,
                                 uint& dataset,
                                 uint& dataset2,
                                 uint chart ) const
{
    bool res = false;
    if ( _setChartSourceModeWasUsed ) {
        bool bStart = true;
        ModeAndChartMap::ConstIterator it;
        for ( it = _dataSourceModeAndChart.begin();
              it != _dataSourceModeAndChart.end(); ++it ) {
            if ( it.data().mode() == mode
                 && it.data().chart() == chart ) {
                if ( bStart ) {
                    dataset = it.key();
                    bStart = false;
                }
                dataset2 = it.key();
                res = true;
            } else if ( !bStart )
                return res;
        }
    } else {
        if ( DataEntry
             == mode ) {
            dataset = KDCHART_ALL_DATASETS;
            dataset2 = KDCHART_ALL_DATASETS;
            res = true;
        }
    }
    return res;
}


void KDChartParams::setDataColor( uint dataset, QColor color )
{
    _dataColors[ dataset ] = color;
    if ( !color.isValid() ) { // no fill color
        _dataColorsShadow1[ dataset ] = QColor();
        _dataColorsShadow2[ dataset ] = QColor();
    } else {
        int hue, saturation, value;
        color.hsv( &hue, &saturation, &value );
        QColor shadowColor1;
        double v = value;
        v = v * 2.0 / 3.0 * shadowBrightnessFactor();
        if ( 255.0 < v )
            v = 255.0;
        else if ( 1.0 > v )
            v = 0.0;
        shadowColor1.setHsv( hue, saturation, static_cast < int > ( v ) );
        _dataColorsShadow1[ dataset ] = shadowColor1;
        QColor shadowColor2;
        v = value;
        v = v / 3.0 * shadowBrightnessFactor();
        if ( 255.0 < v )
            v = 255.0;
        else if ( 1.0 > v )
            v = 0.0;
        shadowColor2.setHsv( hue, saturation, static_cast < int > ( v ) );
        _dataColorsShadow2[ dataset ] = shadowColor2;
    }

    _maxDatasetColor = QMAX( _maxDatasetColor, dataset );

    emit changed();
}


QColor KDChartParams::dataColor( uint dataset ) const
{
    uint index = dataset % _maxDatasetColor;
    return _dataColors[ index ];
}


QColor KDChartParams::dataShadow1Color( uint dataset ) const
{
    uint index = dataset % _maxDatasetColor;
    if ( _threeDShadowColors )
        return _dataColorsShadow1[ index ];
    else
        return _dataColors[ index ];
}


QColor KDChartParams::dataShadow2Color( uint dataset ) const
{
    uint index = dataset % _maxDatasetColor;
    if ( _threeDShadowColors )
        return _dataColorsShadow2[ index ];
    else
        return _dataColors[ index ];
}


void KDChartParams::setDefaultAxesTypes()
{
    // reset types of all axes
    uint i = 0;
    for ( i = 0; i < KDCHART_MAX_AXES; ++i )
        setAxisType( i, KDChartAxisParams::AxisTypeUnknown );

    // Note that abscissa axes should start labeling at the very
    // first position and end at the last position when drawing
    // area charts.
    // Bar charts and line charts look better with their abscissa labels
    // in the respective middle positions below each bar (or point, resp.)
    for ( i = KDChartAxisParams::AxisPosSTART;
          i <= KDChartAxisParams::AxisPosEND; ++i )
        switch ( i ) {
            // abscissa axes:
        case KDChartAxisParams::AxisPosBottom:
        case KDChartAxisParams::AxisPosTop:
        case KDChartAxisParams::AxisPosBottom2:
        case KDChartAxisParams::AxisPosTop2:
            setAxisLabelsTouchEdges( i, Area == chartType() );
            break;
            // ordinate axes:
        case KDChartAxisParams::AxisPosLeft:
        case KDChartAxisParams::AxisPosRight:
        case KDChartAxisParams::AxisPosLeft2:
        case KDChartAxisParams::AxisPosRight2:
            setAxisLabelsTouchEdges( i, true );
            break;
            // additional axes for charts representing 3-dimensional data:
        case KDChartAxisParams::AxisPosLowerRightEdge:
        case KDChartAxisParams::AxisPosLowerLeftEdge:
        case KDChartAxisParams::AxisPosLowerRightEdge2:
        case KDChartAxisParams::AxisPosLowerLeftEdge2:
            setAxisLabelsTouchEdges( i, false );
            break;
        default: {
            qDebug( "IMPLEMENTATION ERROR: axis type missing in KDChartParams::setDefaultAxesTypes()" );
            ASSERT( !this );
        }
        }

    // set default axis types according to chart type
    switch ( chartType() ) {
    case NoType:
        break;
    case Bar:
    case Line:
    case Area:
        // default axes
        setAxisType( KDChartAxisParams::AxisPosBottom,
                     KDChartAxisParams::AxisTypeEAST );
        setAxisShowGrid( KDChartAxisParams::AxisPosBottom,
                         true );
        setAxisType( KDChartAxisParams::AxisPosLeft,
                     KDChartAxisParams::AxisTypeNORTH );
        setAxisShowGrid( KDChartAxisParams::AxisPosLeft,
                         true );
        // 'other side' axes
        setAxisType( KDChartAxisParams::AxisPosTop,
                     KDChartAxisParams::AxisTypeEAST );
        setAxisType( KDChartAxisParams::AxisPosRight,
                     KDChartAxisParams::AxisTypeNORTH );
        // additional, 2nd axes
        setAxisType( KDChartAxisParams::AxisPosBottom2,
                     KDChartAxisParams::AxisTypeEAST );
        setAxisType( KDChartAxisParams::AxisPosLeft2,
                     KDChartAxisParams::AxisTypeNORTH );
        // additional, 2nd axes for 'other' sides
        setAxisType( KDChartAxisParams::AxisPosTop2,
                     KDChartAxisParams::AxisTypeEAST );
        setAxisType( KDChartAxisParams::AxisPosRight2,
                     KDChartAxisParams::AxisTypeNORTH );

        // Specify default numbering information
        // for all 'non-ordinate' axes (this are the X axes):
        //
        // axisSteadyValueCalc flag is set to false
        // Start value 1
        // End value   following the number of entries
        //             in the associated dataset(s)
        // Delta value 1.0
        // and dont show any Digits behind the comma.
        setAxisLabelTextParams( KDChartAxisParams::AxisPosBottom, false,
                                1.0,
                                KDChartAxisParams::AXIS_LABELS_AUTO_LIMIT,
                                1.0, 0 );
        setAxisLabelTextParams( KDChartAxisParams::AxisPosTop, false,
                                1.0,
                                KDChartAxisParams::AXIS_LABELS_AUTO_LIMIT,
                                1.0, 0 );
        setAxisLabelTextParams( KDChartAxisParams::AxisPosBottom2, false,
                                1.0,
                                KDChartAxisParams::AXIS_LABELS_AUTO_LIMIT,
                                1.0, 0 );
        setAxisLabelTextParams( KDChartAxisParams::AxisPosTop2, false,
                                1.0,
                                KDChartAxisParams::AXIS_LABELS_AUTO_LIMIT,
                                1.0, 0 );

        // no need to specify numbering information for
        // the ordinate-axes since the default auto-calc
        // feature is fine for here.

        break;

        // Code for charts representing 3-dimensional data.

        /*

        //   ( not active since there are no such charts yet )

        case KDChartParams::BarMatrix:
        // default axes
            setAxisType( KDChartAxisParams::AxisPosBottom,
                         KDChartAxisParams::AxisTypeEAST );
            setAxisType( KDChartAxisParams::AxisPosLowerRightEdge,
                         KDChartAxisParams::AxisTypeNORTH );
            setAxisType( KDChartAxisParams::AxisPosLeft,
                         KDChartAxisParams::AxisTypeUP );
                         // 'other side' axes
            setAxisType( KDChartAxisParams::AxisPosTop,
                         KDChartAxisParams::AxisTypeEAST );
            setAxisType( KDChartAxisParams::AxisPosLowerLeftEdge,
                         KDChartAxisParams::AxisTypeNORTH );
            setAxisType( KDChartAxisParams::AxisPosRight,
                         KDChartAxisParams::AxisTypeUP );
                         // additional, 2nd axes
            setAxisType( KDChartAxisParams::AxisPosBottom2,
                         KDChartAxisParams::AxisTypeEAST );
            setAxisType( KDChartAxisParams::AxisPosLowerRightEdge2,
                         KDChartAxisParams::AxisTypeNORTH );
            setAxisType( KDChartAxisParams::AxisPosLeft2,
                         KDChartAxisParams::AxisTypeUP );
                         // additional, 2nd axes for 'other' sides
            setAxisType( KDChartAxisParams::AxisPosTop2,
                         KDChartAxisParams::AxisTypeEAST );
            setAxisType( KDChartAxisParams::AxisPosLowerLeftEdge2,
                         KDChartAxisParams::AxisTypeNORTH );
            setAxisType( KDChartAxisParams::AxisPosRight2,
                         KDChartAxisParams::AxisTypeUP );

                         // Specify default numbering information
                         // for all 'non-ordinate' axes (this are the X and the Y axes):
                         // Start vaule 1
                         // End value   following the number of entries
                         //             in the associated dataset(s)
                         // Delta value 1.0
                         // and don't show any Digits behind the comma.
            setAxisLabelTextParams( KDChartAxisParams::AxisPosBottom,  false,
                                    1.0, AXIS_LABELS_AUTO_LIMIT, 1.0, 0 );
            setAxisLabelTextParams( KDChartAxisParams::AxisPosTop,     false,
                                    1.0, AXIS_LABELS_AUTO_LIMIT, 1.0, 0 );
            setAxisLabelTextParams( KDChartAxisParams::AxisPosLowerRightEdge,
                                    false,
                                    1.0, AXIS_LABELS_AUTO_LIMIT, 1.0, 0 );
            setAxisLabelTextParams( KDChartAxisParams::AxisPosLowerLeftEdge,
                                    false,
                                    1.0, AXIS_LABELS_AUTO_LIMIT, 1.0, 0 );
            setAxisLabelTextParams( KDChartAxisParams::AxisPosBottom2, false,
                                    1.0, AXIS_LABELS_AUTO_LIMIT, 1.0, 0 );
            setAxisLabelTextParams( KDChartAxisParams::AxisPosTop2,    false,
                                    1.0, AXIS_LABELS_AUTO_LIMIT, 1.0, 0 );
            setAxisLabelTextParams(KDChartAxisParams::AxisPosLowerRightEdge2,
                                    false,
                                    1.0, AXIS_LABELS_AUTO_LIMIT, 1.0, 0 );
            setAxisLabelTextParams( KDChartAxisParams::AxisPosLowerLeftEdge2,
                                    false,
                                    1.0, AXIS_LABELS_AUTO_LIMIT, 1.0, 0 );

                                    // no need to specify numbering information for
                                    // the ordinate-axes since the default auto-calc
                                    // feature is fine for here.

            break;
*/

    case KDChartParams::Pie:
        // by default there are no axes defined for pie charts
        break;
    default: {
        qDebug( "IMPLEMENTATION ERROR: type missing in KDChartParams::setDefaultAxesTypes()" );
        ASSERT( !this );
    }
    }

    emit changed();
}


void KDChartParams::activateDefaultAxes()
{
    // deactivate all axes
    for ( uint i = 0; i < KDCHART_MAX_AXES; ++i ) {
        setAxisVisible( i, false );
        setAxisDatasets( i, KDCHART_NO_DATASET );
    }
    switch ( chartType() ) {
    case NoType:
        break;
    case Bar:
    case Line:
    case Area:
        setAxisVisible( KDChartAxisParams::AxisPosBottom, true );
        setAxisDatasets( KDChartAxisParams::AxisPosBottom, KDCHART_ALL_DATASETS );
        setAxisVisible( KDChartAxisParams::AxisPosLeft, true );
        setAxisDatasets( KDChartAxisParams::AxisPosLeft, KDCHART_ALL_DATASETS );
        /* test:
        setAxisVisible( KDChartAxisParams::AxisPosTop,    true );
        setAxisDatasets(KDChartAxisParams::AxisPosTop,    KDCHART_ALL_DATASETS );
        setAxisVisible( KDChartAxisParams::AxisPosRight,  true );
        setAxisDatasets(KDChartAxisParams::AxisPosRight,  KDCHART_ALL_DATASETS );

        setAxisVisible( KDChartAxisParams::AxisPosBottom2, true );
        setAxisDatasets(KDChartAxisParams::AxisPosBottom2, KDCHART_ALL_DATASETS );
        setAxisVisible( KDChartAxisParams::AxisPosLeft2,   true );
        setAxisDatasets(KDChartAxisParams::AxisPosLeft2,   KDCHART_ALL_DATASETS );

        setAxisVisible( KDChartAxisParams::AxisPosTop2,    true );
        setAxisDatasets(KDChartAxisParams::AxisPosTop2,    KDCHART_ALL_DATASETS );
        setAxisVisible( KDChartAxisParams::AxisPosRight2,  true );
        setAxisDatasets(KDChartAxisParams::AxisPosRight2,  KDCHART_ALL_DATASETS );
*/
        break;
        // Code for charts representing 3-dimensional data.

        /*

        //   ( not active since there are no such charts yet )

        case KDChartParams::BarMatrix:
            setAxisVisible( KDChartAxisParams::AxisPosBottom,         true );
            setAxisDatasets(KDChartAxisParams::AxisPosBottom, KDCHART_ALL_DATASETS );
            setAxisVisible( KDChartAxisParams::AxisPosLeft,           true );
            setAxisDatasets(KDChartAxisParams::AxisPosLeft,   KDCHART_ALL_DATASETS );
            setAxisVisible( KDChartAxisParams::AxisPosLowerRightEdge, true );
            setAxisDatasets(KDChartAxisParams::AxisPosLowerRightEdge,
                                                              KDCHART_ALL_DATASETS);
            break;
*/
    case KDChartParams::Pie:
        // by default there are no axes defined for pie charts
        break;
    default: {
        qDebug( "IMPLEMENTATION ERROR: type missing in KDChartParams::activateDefaultAxes()" );
        ASSERT( !this );
    }
    }
}



QTextStream& operator<<( QTextStream& s, const KDChartParams& p )
{
    // Create an inital DOM document
    QString docstart = "<ChartParams/>";

    QDomDocument doc( "ChartParams" );
    doc.setContent( docstart );

    QDomElement docRoot = doc.documentElement();
    docRoot.setAttribute( "xmlns", "http://www.klaralvdalens-datakonsult.se/kdchart" );
    docRoot.setAttribute( "xmlns:xsi", "http://www.w3.org/2000/10/XMLSchema-instance" );
    docRoot.setAttribute( "xsi:schemaLocation", "http://www.klaralvdalens-datakonsult.se/kdchart" );

    // the ChartType element
    QDomElement chartTypeElement = doc.createElement( "ChartType" );
    docRoot.appendChild( chartTypeElement );
    chartTypeElement.setAttribute( "primary",
                                   KDChartParams::chartTypeToString( p._chartType ) );
    chartTypeElement.setAttribute( "secondary",
                                   KDChartParams::chartTypeToString( p._additionalChartType ) );


    // the NumValues element
    createIntNode( doc, docRoot, "NumValues", p._numValues );

    // the ModeAndChartMap element
    QDomElement modeAndChartMapElement =
        doc.createElement( "ModeAndChartMap" );
    docRoot.appendChild( modeAndChartMapElement );
    for( QMap<uint,KDChartParams::ModeAndChart>::ConstIterator it = p._dataSourceModeAndChart.begin();
         it != p._dataSourceModeAndChart.end(); ++it ) {
        // Dataset element
        QDomElement datasetElement = doc.createElement( "Dataset" );
        QDomText datasetContent =
            doc.createTextNode( QString::number( it.key() ) );
        datasetElement.appendChild( datasetContent );
        modeAndChartMapElement.appendChild( datasetElement );
        // ModeAndChart element
        QDomElement modeAndChartElement = doc.createElement( "ModeAndChart" );
        modeAndChartElement.setAttribute( "Mode", it.data().mode() );
        modeAndChartElement.setAttribute( "Chart", it.data().chart() );
        modeAndChartMapElement.appendChild( modeAndChartElement );
    }

    createBoolNode( doc, docRoot, "ChartSourceModeWasUsed",
                    p._setChartSourceModeWasUsed );

    // the MaxDatasetSourceMode element
    createIntNode( doc, docRoot, "MaxDatasetSourceMode",
                   p._maxDatasetSourceMode );

    // the ColorSettings element
    QDomElement colorSettingsElement =
        doc.createElement( "ColorSettings" );
    docRoot.appendChild( colorSettingsElement );

    {
        // the DataColors element
        createColorMapNode( doc, colorSettingsElement,
                            "DataColors", p._dataColors );

        // the MaxDatasetColor element
        createIntNode( doc, colorSettingsElement, "MaxDatasetColor",
                       p._maxDatasetColor );

        // the ShadowBrightnessFactor element
        createDoubleNode( doc, colorSettingsElement,
                          "ShadowBrightnessFactor",
                          p._shadowBrightnessFactor );

        // the ThreeDShadowColors element
        createBoolNode( doc, colorSettingsElement,
                        "ThreeDShadowColors",
                        p._threeDShadowColors );

        // the DataColorsShadow1 element
        createColorMapNode( doc, colorSettingsElement,
                            "DataColorsShadow1",
                            p._dataColorsShadow1 );

        // the DataColorsShadow2 element
        createColorMapNode( doc, colorSettingsElement,
                            "DataColorsShadow2",
                            p._dataColorsShadow2 );

        // the OutlineDataColor element
        createColorNode( doc, colorSettingsElement,
                         "OutlineDataColor",
                         p._outlineDataColor );

        // the OutlineDataLineWidth element
        createIntNode( doc, colorSettingsElement,
                       "OutlineDataLineWidth",
                       p._outlineDataLineWidth );

        // the OutlineDataLineStyle element
        QDomElement outlineDataLineStyleElement =
            doc.createElement( "OutlineDataLineStyle" );
        colorSettingsElement.appendChild( outlineDataLineStyleElement );
        outlineDataLineStyleElement.setAttribute( "Style", penStyleToString( p._outlineDataLineStyle ) );
    }


    // the BarSettings elemenet
    QDomElement barSettingsElement =
        doc.createElement( "BarSettings" );
    docRoot.appendChild( barSettingsElement );

    {
        // the SubType element
        createStringNode( doc, barSettingsElement,
                          "SubType", KDChartParams::barChartSubTypeToString( p._barChartSubType ) );

        createBoolNode( doc, barSettingsElement,
                        "ThreeDBars", p._threeDBars );

        createDoubleNode( doc, barSettingsElement,
                          "ThreeDBarDepth", p._threeDBarDepth );

        createIntNode( doc, barSettingsElement,
                       "DatasetGap", p._datasetGap );

        createBoolNode( doc, barSettingsElement,
                        "DatasetGapIsRelative", p._datasetGapIsRelative );

        createIntNode( doc, barSettingsElement,
                       "ValueBlockGap", p._valueBlockGap );

        createBoolNode( doc, barSettingsElement,
                        "ValueBlockGapIsRelative",
                        p._valueBlockGapIsRelative );
    }


    // the LineSettings element
    QDomElement lineSettingsElement =
        doc.createElement( "LineSettings" );
    docRoot.appendChild( lineSettingsElement );

    {
        // the SubType element
        createStringNode( doc, lineSettingsElement,
                          "SubType", KDChartParams::lineChartSubTypeToString( p._lineChartSubType ) );

        // the Marker element
        createBoolNode( doc, lineSettingsElement,
                        "Marker", p._lineMarker );

        // the MarkerStyles elements
        for( QMap<uint,KDChartParams::LineMarkerStyle>::ConstIterator it = p._lineMarkerStyles.begin();
         it != p._lineMarkerStyles.end(); ++it ) {
            QDomElement markerStyleElement = doc.createElement( "MarkerStyle" );
            lineSettingsElement.appendChild( markerStyleElement );
            markerStyleElement.setAttribute( "Dataset", it.key() );
            markerStyleElement.setAttribute( "Style", KDChartParams::lineMarkerStyleToString( it.data() ) );
        }

        // the MarkerSize element
        createSizeNode( doc, lineSettingsElement,
                        "MarkerSize", p._lineMarkerSize );

        // the LineWidth element
        createIntNode( doc, lineSettingsElement,
                       "LineWidth", p._lineWidth );
    }


    // the AreaSettings element
    QDomElement areaSettingsElement =
        doc.createElement( "AreaSettings" );
    docRoot.appendChild( areaSettingsElement );

    {
        // the SubType element
        createStringNode( doc, areaSettingsElement, "SubType",
                          KDChartParams::areaChartSubTypeToString( p._areaChartSubType ) );

        // the Location elemenet
        createStringNode( doc, areaSettingsElement, "Location",
                          KDChartParams::areaLocationToString( p._areaLocation ) );
    }


    // the PieRingSettings element
    QDomElement pieRingSettingsElement =
        doc.createElement( "PieRingSettings" );
    docRoot.appendChild( pieRingSettingsElement );

    {
        // the Explode element
        createBoolNode( doc, pieRingSettingsElement, "Explode",
                        p._explode );

        // the DefaultExplodeFactor element
        createDoubleNode( doc, pieRingSettingsElement, "DefaultExplodeFactor",
                          p._explodeFactor );

        // the ExplodeFactors element
        createDoubleMapNode( doc, pieRingSettingsElement, "ExplodeFactors",
                             p._explodeFactors );

        // the ExplodeSegment element
        for( QValueList<int>::ConstIterator it = p._explodeList.begin();
             it != p._explodeList.end(); ++it )
            createIntNode( doc, pieRingSettingsElement,
                           "ExplodeSegment", *it );

        // the ThreeDPies element
        createBoolNode( doc, pieRingSettingsElement, "ThreeDPies",
                        p._threeDPies );

        // the ThreeDPieHeight element
        createIntNode( doc, pieRingSettingsElement, "ThreeDPieHeight",
                       p._threeDPieHeight );

        // the PieStart element
        createIntNode( doc, pieRingSettingsElement, "PieStart",
                       p._pieStart );

        // the RingStart element
        createIntNode( doc, pieRingSettingsElement, "RingStart",
                       p._ringStart );

        // the RelativeRingThickness element
        createBoolNode( doc, pieRingSettingsElement,
                        "RelativeRingThickness", p._relativeRingThickness );
    }

    // the HiLoSettings element
    QDomElement hiLoSettingsElement =
        doc.createElement( "HiLoSettings" );
    docRoot.appendChild( hiLoSettingsElement );
    {
        // the SubType element
        createStringNode( doc, hiLoSettingsElement, "SubType",
                          KDChartParams::hiLoChartSubTypeToString( p._hiLoChartSubType ) );

        // the PrintLowValues element
        createBoolNode( doc, hiLoSettingsElement, "PrintLowValues",
                        p._hiLoChartPrintLowValues );

        // the LowValuesFont element
        createChartFontNode( doc, hiLoSettingsElement, "LowValuesFont",
                             p._hiLoChartLowValuesFont,
                             p._hiLoChartLowValuesUseFontRelSize,
                             p._hiLoChartLowValuesFontRelSize );

        // the LowValuesColor element
        createColorNode( doc, hiLoSettingsElement, "LowValuesColor",
                         p._hiLoChartLowValuesColor );

        // the PrintHighValues element
        createBoolNode( doc, hiLoSettingsElement, "PrintHighValues",
                        p._hiLoChartPrintHighValues );

        // the HighValuesFont element
        createChartFontNode( doc, hiLoSettingsElement, "HighValuesFont",
                             p._hiLoChartHighValuesFont,
                             p._hiLoChartHighValuesUseFontRelSize,
                             p._hiLoChartHighValuesFontRelSize );

        // the HighValuesColor element
        createColorNode( doc, hiLoSettingsElement, "HighValuesColor",
                         p._hiLoChartHighValuesColor );

        // the PrintOpenValues element
        createBoolNode( doc, hiLoSettingsElement, "PrintOpenValues",
                        p._hiLoChartPrintOpenValues );

        // the OpenValuesFont element
        createChartFontNode( doc, hiLoSettingsElement, "OpenValuesFont",
                             p._hiLoChartOpenValuesFont,
                             p._hiLoChartOpenValuesUseFontRelSize,
                             p._hiLoChartOpenValuesFontRelSize );

        // the OpenValuesColor element
        createColorNode( doc, hiLoSettingsElement, "OpenValuesColor",
                         p._hiLoChartOpenValuesColor );

        // the PrintCloseValues element
        createBoolNode( doc, hiLoSettingsElement, "PrintCloseValues",
                        p._hiLoChartPrintCloseValues );

        // the CloseValuesFont element
        createChartFontNode( doc, hiLoSettingsElement, "CloseValuesFont",
                             p._hiLoChartCloseValuesFont,
                             p._hiLoChartCloseValuesUseFontRelSize,
                             p._hiLoChartCloseValuesFontRelSize );

        // the CloseValuesColor element
        createColorNode( doc, hiLoSettingsElement, "CloseValuesColor",
                         p._hiLoChartCloseValuesColor );
}


    // the GanttSettings element
    QDomElement ganttSettingsElement =
        doc.createElement( "GanttSettings" );
    docRoot.appendChild( ganttSettingsElement );
    {
        // the SubType element
        createStringNode( doc, ganttSettingsElement, "SubType",
                          KDChartParams::ganttChartSubTypeToString( p._ganttChartSubType ) );

        // the TemporalResolution element
        createStringNode( doc, ganttSettingsElement, "TemporalResolution",
                          KDChartParams::ganttTemporalResolutionToString( p._ganttChartTemporalResolution ) );

        // the PrintStartValues element
        createBoolNode( doc, ganttSettingsElement, "PrintStartValues",
                        p._ganttChartPrintStartValues );

        // the StartValuesAlign element
        QDomElement startValuesAlignElement =
            doc.createElement( "StartValuesAlign" );
        ganttSettingsElement.appendChild( startValuesAlignElement );
        createStringNode( doc, startValuesAlignElement, "GanttAlign",
                          KDChartParams::ganttTextAlignToString( p._ganttChartStartValuesAlign ) );

        // the StartValuesDateTimeFormat element
        createStringNode( doc, ganttSettingsElement,
                          "StartValuesDateTimeFormat",
                          p._ganttChartStartValuesDateTimeFormat );

        // the StartValuesFont element
        createChartFontNode( doc, ganttSettingsElement,
                             "StartValuesFont",
                             p._ganttChartStartValuesFont,
                             p._ganttChartStartValuesUseFontRelSize,
                             p._ganttChartStartValuesFontRelSize );

        // the StartValuesColor element
        createColorNode( doc, ganttSettingsElement, "StartValuesColor",
                         p._ganttChartStartValuesColor );

        // the PrintEndValues element
        createBoolNode( doc, ganttSettingsElement, "PrintEndValues",
                        p._ganttChartPrintEndValues );

        // the EndValuesAlign element
        QDomElement endValuesAlignElement =
            doc.createElement( "EndValuesAlign" );
        ganttSettingsElement.appendChild( endValuesAlignElement );
        createStringNode( doc, endValuesAlignElement, "GanttAlign",
                          KDChartParams::ganttTextAlignToString( p._ganttChartEndValuesAlign ) );

        // the EndValuesDateTimeFormat element
        createStringNode( doc, ganttSettingsElement,
                          "EndValuesDateTimeFormat",
                          p._ganttChartEndValuesDateTimeFormat );

        // the EndValuesFont element
        createChartFontNode( doc, ganttSettingsElement,
                             "EndValuesFont",
                             p._ganttChartEndValuesFont,
                             p._ganttChartEndValuesUseFontRelSize,
                             p._ganttChartEndValuesFontRelSize );

        // the EndValuesColor element
        createColorNode( doc, ganttSettingsElement, "EndValuesColor",
                         p._ganttChartEndValuesColor );

        // the PrintDurations element
        createBoolNode( doc, ganttSettingsElement, "PrintDurations",
                        p._ganttChartPrintDurations );

        // the DurationsAlign element
        QDomElement durationsAlignElement =
            doc.createElement( "DurationsAlign" );
        ganttSettingsElement.appendChild( durationsAlignElement );
        createStringNode( doc, durationsAlignElement, "GanttAlign",
                          KDChartParams::ganttTextAlignToString( p._ganttChartDurationsAlign ) );

        // the DurationsDateTimeFormat element
        createStringNode( doc, ganttSettingsElement,
                          "DurationsDateTimeFormat",
                          p._ganttChartDurationsDateTimeFormat );

        // the DurationsFont element
        createChartFontNode( doc, ganttSettingsElement,
                             "DurationsFont",
                             p._ganttChartDurationsFont,
                             p._ganttChartDurationsUseFontRelSize,
                             p._ganttChartDurationsFontRelSize );

        // the DurationsColor element
        createColorNode( doc, ganttSettingsElement, "DurationsColor",
                         p._ganttChartDurationsColor );
    }


    // the LegendSettings element
    QDomElement legendSettingsElement =
        doc.createElement( "LegendSettings" );
    docRoot.appendChild( legendSettingsElement );
    {
        // the Position element
        createStringNode( doc, legendSettingsElement, "Position",
                          KDChartParams::legendPositionToString( p._legendPosition ) );

        // the Source element
        createStringNode( doc, legendSettingsElement, "Source",
                          KDChartParams::legendSourceToString( p._legendSource ) );

        // the Text elements
        for( QMap<int,QString>::ConstIterator it = p._legendText.begin();
         it != p._legendText.end(); ++it ) {
            QDomElement legendTextElement = doc.createElement( "LegendText" );
            legendTextElement.appendChild( legendSettingsElement );
            legendTextElement.setAttribute( "Dataset", it.key() );
            legendTextElement.setAttribute( "Text", it.data() );
        }

        // the TextColor element
        createColorNode( doc, legendSettingsElement, "TextColor",
                         p._legendTextColor );

        // the TextFont element
        createChartFontNode( doc, legendSettingsElement, "TextFont",
                             p._legendFont,
                             p._legendFontUseRelSize,
                             p._legendFontRelSize );

        // the TitleText element
        createStringNode( doc, legendSettingsElement, "TitleText",
                          p._legendTitleText );

        // the TitleColor element
        createColorNode( doc, legendSettingsElement, "TitleColor",
                         p._legendTitleTextColor );

        // the TitleFont element
        createChartFontNode( doc, legendSettingsElement, "TitleFont",
                             p._legendTitleFont,
                             p._legendTitleFontUseRelSize,
                             p._legendTitleFontRelSize );

        // the Spacing element
        createIntNode( doc, legendSettingsElement, "Spacing",
                       p._legendSpacing );
    }

    // the AxisSettings elements
    for( int axis = 0; axis < 13; axis++ ) {
        QDomElement axisSettingsElement =
            doc.createElement( "AxisSettings" );
        docRoot.appendChild( axisSettingsElement );
        axisSettingsElement.setAttribute( "Dataset",
                                          p._axisSettings[axis].dataset );
        axisSettingsElement.setAttribute( "Dataset2",
                                          p._axisSettings[axis].dataset2 );
        axisSettingsElement.setAttribute( "Chart",
                                          p._axisSettings[axis].chart );
        {
            // the Type element
            createStringNode( doc, axisSettingsElement, "Type",
                              KDChartAxisParams::axisTypeToString( p._axisSettings[axis].params._axisType ) );

            // the Visible element
            createBoolNode( doc, axisSettingsElement, "Visible",
                            p._axisSettings[axis].params._axisVisible );

            // the LabelsTouchEdges element
            createBoolNode( doc, axisSettingsElement, "LabelsTouchEdges",
                            p._axisSettings[axis].params._axisLabelsTouchEdges );

            // the AreaMode element
            createStringNode( doc, axisSettingsElement, "AreaMode",
                              KDChartAxisParams::axisAreaModeToString( p._axisSettings[axis].params._axisAreaMode ) );

            // the AreaMin element
            createIntNode( doc, axisSettingsElement, "AreaMin",
                           p._axisSettings[axis].params._axisAreaMin );

            // the AreaMax element
            createIntNode( doc, axisSettingsElement, "AreaMax",
                           p._axisSettings[axis].params._axisAreaMax );

            // the CalcMode element
            createStringNode( doc, axisSettingsElement, "CalcMode",
                              KDChartAxisParams::axisCalcModeToString( p._axisSettings[axis].params._axisCalcMode ) );

            // the TrueAreaSize element
            createIntNode( doc, axisSettingsElement, "TrueAreaSize",
                           p._axisSettings[axis].params._axisTrueAreaSize );

            // the TrueAreaRect element
            createRectNode( doc, axisSettingsElement, "TrueAreaRect",
                            p._axisSettings[axis].params._axisTrueAreaRect );

            // the ShowSubDelimiters element
            createBoolNode( doc, axisSettingsElement, "ShowSubDelimiters",
                            p._axisSettings[axis].params._axisShowSubDelimiters );

            // the LineVisible element
            createBoolNode( doc, axisSettingsElement, "LineVisible",
                            p._axisSettings[axis].params._axisLineVisible );

            // the LineWidth element
            createIntNode( doc, axisSettingsElement, "LineWidth",
                           p._axisSettings[axis].params._axisLineWidth );

            // the TrueLineWidth element
            createIntNode( doc, axisSettingsElement, "TrueLineWidth",
                           p._axisSettings[axis].params._axisTrueLineWidth );

            // the LineColor element
            createColorNode( doc, axisSettingsElement, "LineColor",
                             p._axisSettings[axis].params._axisLineColor );

            // the ShowGrid element
            createBoolNode( doc, axisSettingsElement, "ShowGrid",
                            p._axisSettings[axis].params._axisShowGrid );

            // the GridColor element
            createColorNode( doc, axisSettingsElement, "GridColor",
                             p._axisSettings[axis].params._axisGridColor );

            // the GridLineWidth element
            createIntNode( doc, axisSettingsElement, "GridLineWidth",
                           p._axisSettings[axis].params._axisGridLineWidth );

            // the GridStyle element
            createStringNode( doc, axisSettingsElement, "GridStyle",
                              penStyleToString( p._axisSettings[axis].params._axisGridStyle ) );

            // the GridSubStyle element
            createStringNode( doc, axisSettingsElement, "GridSubStyle",
                              penStyleToString( p._axisSettings[axis].params._axisGridSubStyle ) );

            // the ZeroLineColor element
            createColorNode( doc, axisSettingsElement, "ZeroLineColor",
                             p._axisSettings[axis].params._axisZeroLineColor );

            // the LabelsVisible element
            createBoolNode( doc, axisSettingsElement, "LabelsVisible",
                            p._axisSettings[axis].params._axisLabelsVisible );

            // the LabelsFont element
            createChartFontNode( doc, axisSettingsElement, "LabelsFont",
                                 p._axisSettings[axis].params._axisLabelsFont,
                                 p._axisSettings[axis].params._axisLabelsFontUseRelSize,
                                 p._axisSettings[axis].params._axisLabelsFontRelSize );

            // the LabelsColor element
            createColorNode( doc, axisSettingsElement, "LabelsColor",
                             p._axisSettings[axis].params._axisLabelsColor );

            // the SteadyValueCalc element
            createBoolNode( doc, axisSettingsElement, "SteadyValueCalc",
                            p._axisSettings[axis].params._axisSteadyValueCalc );

            // the ValueStart element
            createChartValueNode( doc, axisSettingsElement, "ValueStart",
                                  p._axisSettings[axis].params._axisValueStart );

            // the ValueEnd element
            createChartValueNode( doc, axisSettingsElement, "ValueEnd",
                                  p._axisSettings[axis].params._axisValueEnd );

            // the ValueDelta element
            createDoubleNode( doc, axisSettingsElement, "ValueDelta",
                              p._axisSettings[axis].params._axisValueDelta );

            // the TrueLow element
            createDoubleNode( doc, axisSettingsElement, "TrueLow",
                              p._axisSettings[axis].params._trueLow );

            // the TrueHigh element
            createDoubleNode( doc, axisSettingsElement, "TrueHigh",
                              p._axisSettings[axis].params._trueHigh );

            // the TrueDelta element
            createDoubleNode( doc, axisSettingsElement, "TrueDelta",
                              p._axisSettings[axis].params._trueDelta );

            // the ZeroLineStart element
            QDomElement zeroLineStartElement = doc.createElement( "ZeroLineStart" );
            axisSettingsElement.appendChild( zeroLineStartElement );
            zeroLineStartElement.setAttribute( "X", p._axisSettings[axis].params._axisZeroLineStartX );
            zeroLineStartElement.setAttribute( "Y", p._axisSettings[axis].params._axisZeroLineStartY );

            // the DigitsBehindComma element
            createIntNode( doc, axisSettingsElement, "DigitsBehindComma",
                           p._axisSettings[axis].params._axisDigitsBehindComma );

            // the MaxEmptyInnerSpan element
            createIntNode( doc, axisSettingsElement, "MaxEmptyInnerSpan",
                           p._axisSettings[axis].params._axisMaxEmptyInnerSpan );

            // the LabelsFromDataRow element
            createStringNode( doc, axisSettingsElement, "LabelsFromDataRow",
                              KDChartAxisParams::labelsFromDataRowToString( p._axisSettings[axis].params._takeLabelsFromDataRow ) );

            // the TextsDataRow element
            createIntNode( doc, axisSettingsElement, "TextsDataRow",
                           p._axisSettings[axis].params._labelTextsDataRow );

            // the LabelString elements
            createStringListNodes( doc, axisSettingsElement, "LabelString",
                                   p._axisSettings[axis].params._axisLabelStringList );

            // the ShortLabelString elements
            createStringListNodes( doc, axisSettingsElement, "ShortLabelString",
                                   p._axisSettings[axis].params._axisShortLabelsStringList );

            // the LabelText elements
            createStringListNodes( doc, axisSettingsElement, "LabelText",
                                   &p._axisSettings[axis].params._axisLabelTexts );

            // the LabelTextsDirty element
            createBoolNode( doc, axisSettingsElement, "LabelTextsDirty",
                            p._axisSettings[axis].params._axisLabelTextsDirty );
        }
    }

    // the HeaderFooterSettings elements
    for( int hf = 0; hf < 18; hf++ ) {
        QDomElement hfSettingsElement =
            doc.createElement( "HeaderFooterSettings" );
        docRoot.appendChild( hfSettingsElement );
        {
            createStringNode( doc, hfSettingsElement, "Text",
                              p._hdFtParams[hf]._text );
            createChartFontNode( doc, hfSettingsElement, "Font",
                                 p._hdFtParams[hf]._font,
                                 p._hdFtParams[hf]._fontUseRelSize,
                                 p._hdFtParams[hf]._fontRelSize );
        }
    }


    s << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
    s << doc.toString();

    return s;
}

QTextStream& operator>>( QTextStream& s, KDChartParams& p )
{
    int curAxisSettings = 0;
    int curHFSettings = 0;

    QDomDocument doc( "ChartParams" );
    // would be nicer if QDomDocument could read from a QTextStream...
    QString docString = s.read();
    doc.setContent( docString );

    QDomElement docRoot = doc.documentElement(); // ChartParams element
    QDomNode node = docRoot.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "ChartType" ) {
                if( element.hasAttribute( "primary" ) )
                    p._chartType = KDChartParams::stringToChartType( element.attribute( "primary" ) );
                if( element.hasAttribute( "secondary" ) )
                    p._additionalChartType = KDChartParams::stringToChartType( element.attribute( "secondary" ) );
            } else if( tagName == "NumValues" ) {
                int numValues;
                if( readIntNode( element, numValues ) )
                    p._numValues = numValues;
            } else if( tagName == "ModeAndChartMap" ) {
                int dataset = -1;
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "Dataset" ) {
                            readIntNode( element, dataset );
                        } else if( tagName == "ModeAndChart" ) {
                            ASSERT( dataset != -1 ); // there must have been a dataset tag before
                            if( element.hasAttribute( "Mode" ) &&
                                element.hasAttribute( "Chart" ) ) {
                                KDChartParams::SourceMode sourceMode = KDChartParams::stringToChartSourceMode( element.attribute( "Mode" ) );
                                bool ok = false;
                                uint chart = element.attribute( "Chart" ).toUInt( &ok );
                                if( ok )
                                    p._dataSourceModeAndChart[dataset] = KDChartParams::ModeAndChart( sourceMode, chart );
                            }
                        } else {
                            qDebug( "Unknown subelement of ModeAndChartMap found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
            } else if( tagName == "ChartSourceModeWasUsed" ) {
                bool chartSourceModeWasUsed;
                if( readBoolNode( element, chartSourceModeWasUsed ) )
                    p._setChartSourceModeWasUsed = chartSourceModeWasUsed;
            } else if( tagName == "MaxDatasetSourceMode" ) {
                int maxDatasetSourceMode;
                if( readIntNode( element, maxDatasetSourceMode ) )
                    p._maxDatasetSourceMode = maxDatasetSourceMode;
            } else if( tagName == "ColorSettings" ) {
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "DataColors" ) {
                            QMap<uint,QColor>* map = &p._dataColors;
                            readColorMapNode( element, map );
                        } else if( tagName == "MaxDatasetColor" ) {
                            int maxDatasetColor;
                            if( readIntNode( element, maxDatasetColor ) )
                                p._maxDatasetColor = maxDatasetColor;
                        } else if( tagName == "ShadowBrightnessFactor" ) {
                            double shadowBrightnessFactor;
                            if( readDoubleNode( element, shadowBrightnessFactor ) )
                                p._shadowBrightnessFactor = shadowBrightnessFactor;
                        } else if( tagName == "ThreeDShadowColors" ) {
                            bool threeDShadowColors;
                            if( readBoolNode( element, threeDShadowColors ) )
                                p._threeDShadowColors = threeDShadowColors;
                        } else if( tagName == "DataColorsShadow1" ) {
                            QMap<uint,QColor>* map = &p._dataColorsShadow1;
                            readColorMapNode( element, map );
                        } else if( tagName == "DataColorsShadow2" ) {
                            QMap<uint,QColor>* map = &p._dataColorsShadow2;
                            readColorMapNode( element, map );
                        } else if( tagName == "OutlineDataColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._outlineDataColor = color;
                        } else if( tagName == "OutlineDataLineWidth" ) {
                            int outlineDataLineWidth;
                            if( readIntNode( element, outlineDataLineWidth ) )
                                p._outlineDataLineWidth = outlineDataLineWidth;
                        } else if( tagName == "OutlineDataLineStyle" ) {
                            if( element.hasAttribute( "Style" ) )
                                p._outlineDataLineStyle = stringToPenStyle( element.attribute( "Style" ) );
                        } else {
                            qDebug( "!!!Unknown subelement of ColorSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
            } else if( tagName == "BarSettings" ) {
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "SubType" ) {
                            QString value;
                            if( readStringNode( element, value ) )
                                p._barChartSubType = KDChartParams::stringToBarChartSubType( value );
                        } else if( tagName == "ThreeDBars" ) {
                            bool threeDBars;
                            if( readBoolNode( element, threeDBars ) )
                                p._threeDBars = threeDBars;
                        } else if( tagName == "ThreeDBarDepth" ) {
                            double threeDBarDepth;
                            if( readDoubleNode( element, threeDBarDepth ) )
                                p._threeDBarDepth = threeDBarDepth;
                        } else if( tagName == "DatasetGap" ) {
                            int datasetGap;
                            if( readIntNode( element, datasetGap ) )
                                p._datasetGap = datasetGap;
                        } else if( tagName == "DatasetGapIsRelative" ) {
                            bool datasetGapIsRelative;
                            if( readBoolNode( element, datasetGapIsRelative ) )
                                p._datasetGapIsRelative = datasetGapIsRelative;
                        } else if( tagName == "ValueBlockGap" ) {
                            int valueBlockGap;
                            if( readIntNode( element, valueBlockGap ) )
                                p._valueBlockGap = valueBlockGap;
                        } else if( tagName == "ValueBlockGapIsRelative" ) {
                            bool valueBlockGapIsRelative;
                            if( readBoolNode( element, valueBlockGapIsRelative ) )
                                p._valueBlockGapIsRelative = valueBlockGapIsRelative;
                        } else {
                            qDebug( "Unknown subelement of BarSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
            } else if( tagName == "LineSettings" ) {
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "SubType" ) {
                            QString value;
                            if( readStringNode( element, value ) )
                                p._lineChartSubType = KDChartParams::stringToLineChartSubType( value );
                        } else if( tagName == "Marker" ) {
                            bool marker;
                            if( readBoolNode( element, marker ) )
                                p._lineMarker = marker;
                        } else if( tagName == "MarkerStyle" ) {
                            bool ok = true;
                            uint dataset;
                            KDChartParams::LineMarkerStyle style;
                            if( element.hasAttribute( "Dataset" ) &&
                                element.hasAttribute( "Style" ) ) {
                                dataset = element.attribute( "Dataset" ).toUInt( &ok );
                                if( ok )
                                    style = KDChartParams::stringToLineMarkerStyle( element.attribute( "Style" ) );
                            } else
                                ok = false;
                            if( ok )
                                p._lineMarkerStyles[dataset] = style;
                        } else if( tagName == "MarkerSize" ) {
                            int width, height;
                            bool ok = true;
                            if( element.hasAttribute( "Width" ) &&
                                element.hasAttribute( "Height" ) ) {
                                width = element.attribute( "Width" ).toInt( &ok );
                                if( ok )
                                    height = element.attribute( "Height" ).toInt( &ok );
                            } else
                                ok = false;

                            if( ok ) {
                                p._lineMarkerSize.setWidth( width );
                                p._lineMarkerSize.setHeight( height );
                            }
                        } else if( tagName == "LineWidth" ) {
                            int lineWidth;
                            if( readIntNode( element, lineWidth ) )
                                p._lineWidth = lineWidth;
                        } else {
                            qDebug( "Unknown subelement of LineSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
            } else if( tagName == "AreaSettings" ) {
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "SubType" ) {
                            QString value;
                            if( readStringNode( element, value ) )
                                p._areaChartSubType = KDChartParams::stringToAreaChartSubType( value );
                        } else if( tagName == "Location" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._areaLocation = KDChartParams::stringToAreaLocation( string );
                        } else {
                            qDebug( "Unknown subelement of AreaSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
            } else if( tagName == "PieRingSettings" ) {
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "Explode" ) {
                            bool explode;
                            if( readBoolNode( element, explode ) )
                                p._explode = explode;
                        } else if( tagName == "DefaultExplodeFactor" ) {
                            double defaultExplodeFactor;
                            if( readDoubleNode( element, defaultExplodeFactor ) )
                                p._explodeFactor = defaultExplodeFactor;
                        } else if( tagName == "ExplodeFactors" ) {
                            QMap<int,double>* map = &p._explodeFactors;
                            readDoubleMapNode( element, map );
                        } else if( tagName == "ExplodeSegment" ) {
                            int explodeSegment;
                            if( readIntNode( element, explodeSegment ) )
                                p._explodeList << explodeSegment;
                        } else if( tagName == "ThreeDPies" ) {
                            bool threeDPies;
                            if( readBoolNode( element, threeDPies ) )
                                p._threeDPies = threeDPies;
                        } else if( tagName == "ThreeDPieHeight" ) {
                            int threeDPieHeight;
                            if( readIntNode( element, threeDPieHeight ) )
                                p._threeDPieHeight = threeDPieHeight;
                        } else if( tagName == "PieStart" ) {
                            int pieStart;
                            if( readIntNode( element, pieStart ) )
                                p._pieStart = pieStart;
                        } else if( tagName == "RingStart" ) {
                            int ringStart;
                            if( readIntNode( element, ringStart ) )
                                p._ringStart = ringStart;
                        } else if( tagName == "RelativeRingThickness" ) {
                            bool relativeRingThickness;
                            if( readBoolNode( element, relativeRingThickness ) )
                                p._relativeRingThickness = relativeRingThickness;
                        } else {
                            qDebug( "Unknown subelement of PieRingSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
            } else if( tagName == "HiLoSettings" ) {
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "SubType" ) {
                            QString value;
                            if( readStringNode( element, value ) )
                                p._hiLoChartSubType = KDChartParams::stringToHiLoChartSubType( value );
                        } else if( tagName == "PrintLowValues" ) {
                            bool printLowValues;
                            if( readBoolNode( element, printLowValues ) )
                                p._hiLoChartPrintLowValues = printLowValues;
                        } else if( tagName == "LowValuesFont" ) {
                            readChartFontNode( element,
                                               p._hiLoChartLowValuesFont,
                                               p._hiLoChartLowValuesUseFontRelSize,
                                               p._hiLoChartLowValuesFontRelSize );
                        } else if( tagName == "LowValuesColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._hiLoChartLowValuesColor = color;
                        } else if( tagName == "PrintHighValues" ) {
                            bool printHighValues;
                            if( readBoolNode( element, printHighValues ) )
                                p._hiLoChartPrintHighValues = printHighValues;
                        } else if( tagName == "HighValuesFont" ) {
                            readChartFontNode( element,
                                               p._hiLoChartHighValuesFont,
                                               p._hiLoChartHighValuesUseFontRelSize,
                                               p._hiLoChartHighValuesFontRelSize );
                        } else if( tagName == "HighValuesColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._hiLoChartHighValuesColor = color;
                        } else if( tagName == "PrintOpenValues" ) {
                            bool printOpenValues;
                            if( readBoolNode( element, printOpenValues ) )
                                p._hiLoChartPrintOpenValues = printOpenValues;
                        } else if( tagName == "OpenValuesFont" ) {
                            readChartFontNode( element,
                                               p._hiLoChartOpenValuesFont,
                                               p._hiLoChartOpenValuesUseFontRelSize,
                                               p._hiLoChartOpenValuesFontRelSize );
                        } else if( tagName == "OpenValuesColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._hiLoChartOpenValuesColor = color;
                        } else if( tagName == "PrintCloseValues" ) {
                            bool printCloseValues;
                            if( readBoolNode( element, printCloseValues ) )
                                p._hiLoChartPrintCloseValues = printCloseValues;
                        } else if( tagName == "CloseValuesFont" ) {
                            readChartFontNode( element,
                                               p._hiLoChartCloseValuesFont,
                                               p._hiLoChartCloseValuesUseFontRelSize,
                                               p._hiLoChartCloseValuesFontRelSize );
                        } else if( tagName == "CloseValuesColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._hiLoChartCloseValuesColor = color;
                        } else {
                            qDebug( "Unknown subelement of HiLoSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
            } else if( tagName == "GanttSettings" ) {
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "SubType" ) {
                            QString value;
                            if( readStringNode( element, value ) )
                                p._ganttChartSubType = KDChartParams::stringToGanttChartSubType( value );
                        } else if( tagName == "TemporalResolution" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._ganttChartTemporalResolution = KDChartParams::stringToGanttChartTemporalResolution( string );
                        } else if( tagName == "PrintStartValues" ) {
                            bool printStartValues;
                            if( readBoolNode( element, printStartValues ) )
                                p._ganttChartPrintStartValues = printStartValues;
                        } else if( tagName == "StartValuesAlign" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._ganttChartStartValuesAlign = KDChartParams::stringToGanttChartTextAlign( string );
                        } else if( tagName == "StartValuesDateTimeFormat" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._ganttChartStartValuesDateTimeFormat = string;
                        } else if( tagName == "StartValuesFont" ) {
                            readChartFontNode( element,
                                               p._ganttChartStartValuesFont,
                                               p._ganttChartStartValuesUseFontRelSize,
                                               p._ganttChartStartValuesFontRelSize );
                        } else if( tagName == "StartValuesColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._ganttChartStartValuesColor = color;
                        } else if( tagName == "PrintEndValues" ) {
                            bool printEndValues;
                            if( readBoolNode( element, printEndValues ) )
                                p._ganttChartPrintEndValues = printEndValues;
                        } else if( tagName == "EndValuesAlign" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._ganttChartEndValuesAlign = KDChartParams::stringToGanttChartTextAlign( string );
                        } else if( tagName == "EndValuesDateTimeFormat" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._ganttChartEndValuesDateTimeFormat = string;
                        } else if( tagName == "EndValuesFont" ) {
                            readChartFontNode( element,
                                               p._ganttChartEndValuesFont,
                                               p._ganttChartEndValuesUseFontRelSize,
                                               p._ganttChartEndValuesFontRelSize );
                        } else if( tagName == "EndValuesColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._ganttChartEndValuesColor = color;
                        } else if( tagName == "PrintDurations" ) {
                            bool printDurations;
                            if( readBoolNode( element, printDurations ) )
                                p._ganttChartPrintDurations = printDurations;
                        } else if( tagName == "DurationsAlign" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._ganttChartDurationsAlign = KDChartParams::stringToGanttChartTextAlign( string );
                        } else if( tagName == "DurationsDateTimeFormat" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._ganttChartDurationsDateTimeFormat = string;
                        } else if( tagName == "DurationsFont" ) {
                            readChartFontNode( element,
                                               p._ganttChartDurationsFont,
                                               p._ganttChartDurationsUseFontRelSize,
                                               p._ganttChartDurationsFontRelSize );
                        } else if( tagName == "DurationsColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._ganttChartDurationsColor = color;
                        } else {
                            qDebug( "Unknown subelement of GanttSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
            } else if( tagName == "LegendSettings" ) {
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "Position" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._legendPosition = KDChartParams::stringToLegendPosition( string );
                        } else if( tagName == "Source" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._legendSource = KDChartParams::stringToLegendSource( string );
                        } else if( tagName == "Text" ) {
                            bool ok = true;
                            uint dataset;
                            QString text;
                            if( element.hasAttribute( "Dataset" ) &&
                                element.hasAttribute( "Text" ) ) {
                                dataset = element.attribute( "Dataset" ).toUInt( &ok );
                                if( ok )
                                    text = element.attribute( "Text" );
                            } else
                                ok = false;
                            if( ok )
                                p._legendText[dataset] = text;
                        } else if( tagName == "TextColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._legendTextColor = color;
                        } else if( tagName == "TextFont" ) {
                            readChartFontNode( element,
                                               p._legendFont,
                                               p._legendFontUseRelSize,
                                               p._legendFontRelSize );
                        } else if( tagName == "TitleText" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                p._legendTitleText = string;
                        } else if( tagName == "TitleColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                p._legendTitleTextColor = color;
                        } else if( tagName == "TitleFont" ) {
                            readChartFontNode( element,
                                               p._legendTitleFont,
                                               p._legendTitleFontUseRelSize,
                                               p._legendTitleFontRelSize );
                        } else if( tagName == "Spacing" ) {
                            int spacing;
                            if( readIntNode( element, spacing ) )
                                p._legendSpacing = spacing;
                        } else {
                            qDebug( "Unknown subelement of LegendSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
            } else if( tagName == "AxisSettings" ) {
                KDChartAxisParams* axisSettings =
                    &( p._axisSettings[ curAxisSettings ].params );
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "Type" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                axisSettings->_axisType = KDChartAxisParams::stringToAxisType( string );
                        } else if( tagName == "Visible" ) {
                            bool visible;
                            if( readBoolNode( element, visible ) )
                                axisSettings->_axisVisible = visible;
                        } else if( tagName == "LabelsTouchEdges" ) {
                            bool labelsTouchEdges;
                            if( readBoolNode( element, labelsTouchEdges ) )
                                axisSettings->_axisLabelsTouchEdges = labelsTouchEdges;
                        } else if( tagName == "AreaMode" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                axisSettings->_axisAreaMode = KDChartAxisParams::stringToAxisAreaMode( string );
                        } else if( tagName == "AreaMin" ) {
                            int areaMin;
                            if( readIntNode( element, areaMin ) )
                                axisSettings->_axisAreaMin = areaMin;
                        } else if( tagName == "AreaMax" ) {
                            int areaMax;
                            if( readIntNode( element, areaMax ) )
                                axisSettings->_axisAreaMax = areaMax;
                        } else if( tagName == "CalcMode" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                axisSettings->_axisCalcMode = KDChartAxisParams::stringToAxisCalcMode( string );
                        } else if( tagName == "TrueAreaSize" ) {
                            int trueAreaSize;
                            if( readIntNode( element, trueAreaSize ) )
                                axisSettings->_axisTrueAreaSize = trueAreaSize;
                        } else if( tagName == "TrueAreaRect" ) {
                            QRect trueAreaRect;
                            if( readRectNode( element, trueAreaRect ) )
                                axisSettings->_axisTrueAreaRect = trueAreaRect;
                        } else if( tagName == "ShowSubDelimiters" ) {
                            bool showSubDelimiters;
                            if( readBoolNode( element, showSubDelimiters ) )
                                axisSettings->_axisShowSubDelimiters = showSubDelimiters;
                        } else if( tagName == "LineVisible" ) {
                            bool lineVisible;
                            if( readBoolNode( element, lineVisible ) )
                                axisSettings->_axisLineVisible = lineVisible;
                        } else if( tagName == "LineWidth" ) {
                            int lineWidth;
                            if( readIntNode( element, lineWidth ) )
                                axisSettings->_axisLineWidth = lineWidth;
                        } else if( tagName == "TrueLineWidth" ) {
                            int trueLineWidth;
                            if( readIntNode( element, trueLineWidth ) )
                                axisSettings->_axisTrueLineWidth = trueLineWidth;
                        } else if( tagName == "LineColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                axisSettings->_axisLineColor = color;
                        } else if( tagName == "ShowGrid" ) {
                            bool showGrid;
                            if( readBoolNode( element, showGrid ) )
                                axisSettings->_axisShowGrid = showGrid;
                        } else if( tagName == "GridColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                axisSettings->_axisGridColor = color;
                        } else if( tagName == "GridLineWidth" ) {
                            int gridLineWidth;
                            if( readIntNode( element, gridLineWidth ) )
                                axisSettings->_axisGridLineWidth = gridLineWidth;
                        } else if( tagName == "GridStyle" ) {
                            if( element.hasAttribute( "Style" ) )
                                axisSettings->_axisGridStyle = stringToPenStyle( element.attribute( "Style" ) );
                        } else if( tagName == "GridSubStyle" ) {
                            if( element.hasAttribute( "Style" ) )
                                axisSettings->_axisGridSubStyle = stringToPenStyle( element.attribute( "Style" ) );
                        } else if( tagName == "ZeroLineColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                axisSettings->_axisZeroLineColor = color;
                        } else if( tagName == "LabelsVisible" ) {
                            bool labelsVisible;
                            if( readBoolNode( element, labelsVisible ) )
                                axisSettings->_axisLabelsVisible = labelsVisible;
                        } else if( tagName == "LabelsFont" ) {
                            readChartFontNode( element,
                                               axisSettings->_axisLabelsFont,
                                               axisSettings->_axisLabelsFontUseRelSize,
                                               axisSettings->_axisLabelsFontRelSize );
                        } else if( tagName == "LabelsColor" ) {
                            QColor color;
                            if( readColorNode( element, color ) )
                                axisSettings->_axisLabelsColor = color;
                        } else if( tagName == "SteadyValueCalc" ) {
                            bool steadyValueCalc;
                            if( readBoolNode( element, steadyValueCalc ) )
                                axisSettings->_axisSteadyValueCalc = steadyValueCalc;
                        } else if( tagName == "ValueStart" ) {
                            KDChartData value;
                            if( readChartValueNode( element, value ) )
                                axisSettings->_axisValueStart = value;
                        } else if( tagName == "ValueEnd" ) {
                            KDChartData value;
                            if( readChartValueNode( element, value ) )
                                axisSettings->_axisValueEnd = value;
                        } else if( tagName == "ValueDelta" ) {
                            double valueDelta;
                            if( readDoubleNode( element, valueDelta ) )
                                axisSettings->_axisValueDelta = valueDelta;
                        } else if( tagName == "TrueLow" ) {
                            double trueLow;
                            if( readDoubleNode( element, trueLow ) )
                                axisSettings->_trueLow  = trueLow;
                        } else if( tagName == "TrueHigh" ) {
                            double trueHigh;
                            if( readDoubleNode( element, trueHigh ) )
                                axisSettings->_trueHigh  = trueHigh;
                        } else if( tagName == "TrueDelta" ) {
                            double trueDelta;
                            if( readDoubleNode( element, trueDelta ) )
                                axisSettings->_trueDelta  = trueDelta;
                        } else if( tagName == "ZeroLineStart" ) {
                            double x, y;
                            bool ok = true;
                            if( element.hasAttribute( "X" ) &&
                                element.hasAttribute( "Y" ) ) {
                                x = element.attribute( "X" ).toDouble( &ok );
                                if( ok )
                                    y = element.attribute( "Y" ).toDouble( &ok );
                            } else
                                ok = false;

                            if( ok ) {
                                axisSettings->_axisZeroLineStartX = x;
                                axisSettings->_axisZeroLineStartY = y;
                            }
                        } else if( tagName == "DigitsBehindComma" ) {
                            int digitsBehindComma;
                            if( readIntNode( element, digitsBehindComma ) )
                                axisSettings->_axisDigitsBehindComma = digitsBehindComma;
                        } else if( tagName == "MaxEmptyInnerSpan" ) {
                            int maxEmptyInnerSpan;
                            if( readIntNode( element, maxEmptyInnerSpan ) )
                                axisSettings->_axisMaxEmptyInnerSpan = maxEmptyInnerSpan;
                        } else if( tagName == "LabelsFromDataRow" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                axisSettings->_labelTextsDataRow = KDChartAxisParams::stringToLabelsFromDataRow( string );
                        } else if( tagName == "TextsDataRow" ) {
                            int textsDataRow;
                            if( readIntNode( element, textsDataRow ) )
                                axisSettings->_labelTextsDataRow = textsDataRow;
                        } else if( tagName == "LabelString" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                if( axisSettings->_axisLabelStringList )
                                    *(axisSettings->_axisLabelStringList) << string;
                        } else if( tagName == "ShortLabelString" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                if( axisSettings->_axisShortLabelsStringList )
                                    *(axisSettings->_axisShortLabelsStringList) << string;
                        } else if( tagName == "LabelTexts" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                axisSettings->_axisLabelTexts = string;
                        } else if( tagName == "LabelTextsDirty" ) {
                            bool labelTextsDirty;
                            if( readBoolNode( element, labelTextsDirty ) )
                                axisSettings->_axisLabelTextsDirty = labelTextsDirty;
                        } else {
                            qDebug( "Unknown subelement of AxisSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
                curAxisSettings++; // one axis settings entry finished
            } else if( tagName == "HeaderFooterSettings" ) {
                KDChartParams::HdFtParams* hfSettings =
                    &( p._hdFtParams[ curHFSettings ] );
                QDomNode node = element.firstChild();
                while( !node.isNull() ) {
                    QDomElement element = node.toElement();
                    if( !element.isNull() ) { // was really an element
                        QString tagName = element.tagName();
                        if( tagName == "Text" ) {
                            QString string;
                            if( readStringNode( element, string ) )
                                hfSettings->_text = string;
                        } else if( tagName == "Font" ) {
                            readChartFontNode( element,
                                               hfSettings->_font,
                                               hfSettings->_fontUseRelSize,
                                               hfSettings->_fontRelSize );
                        } else {
                            qDebug( "Unknown subelement of HeaderFooterSettings found: %s", tagName.latin1() );
                        }
                    }
                    node = node.nextSibling();
                }
                curHFSettings++; // one header/footer setting finished
            } else {
                qDebug( "!!!Unknown element found: %s", tagName.latin1() );
            }
        }
        node = node.nextSibling();
    }

    return s;
}


static void createBoolNode( QDomDocument& doc, QDomNode& parent,
                            const QString& elementName, bool value )
{
    QDomElement newElement =
        doc.createElement( elementName );
    parent.appendChild( newElement );
    QDomText elementContent =
        doc.createTextNode( value ? "true" : "false" );
    newElement.appendChild( elementContent );
}



static void createSizeNode( QDomDocument& doc, QDomNode& parent,
                            const QString& elementName, const QSize& value )
{
    QDomElement newElement =
        doc.createElement( elementName );
    parent.appendChild( newElement );
    newElement.setAttribute( "Width", value.width() );
    newElement.setAttribute( "Height", value.height() );
}


static void createIntNode( QDomDocument& doc, QDomNode& parent,
                           const QString& elementName, int value )
{
    QDomElement newElement =
        doc.createElement( elementName );
    parent.appendChild( newElement );
    QDomText elementContent =
        doc.createTextNode( QString::number( value ) );
    newElement.appendChild( elementContent );
}


static void createDoubleNode( QDomDocument& doc, QDomNode& parent,
                              const QString& elementName, double value )
{
    QDomElement newElement =
        doc.createElement( elementName );
    parent.appendChild( newElement );
    QDomText elementContent =
        doc.createTextNode( QString::number( value ) );
    newElement.appendChild( elementContent );
}


static void createStringNode( QDomDocument& doc, QDomNode& parent,
                              const QString& elementName,
                              const QString& text )
{
    QDomElement newElement =
        doc.createElement( elementName );
    parent.appendChild( newElement );
    QDomText elementContent =
        doc.createTextNode( text );
    newElement.appendChild( elementContent );
}


static void createColorNode( QDomDocument& doc, QDomNode& parent,
                             const QString& elementName, const QColor& color )
{
    QDomElement colorElement = doc.createElement( elementName );
    parent.appendChild( colorElement );
    colorElement.setAttribute( "Red",
                               QString::number( color.red() ) );
    colorElement.setAttribute( "Green",
                               QString::number( color.green() ) );
    colorElement.setAttribute( "Blue",
                               QString::number( color.blue() ) );
}


static void createColorMapNode( QDomDocument& doc, QDomNode& parent,
                                const QString& elementName,
                                const QMap< uint, QColor >& map )
{
    QDomElement mapElement =
        doc.createElement( elementName );
    parent.appendChild( mapElement );
    for( QMap<uint,QColor>::ConstIterator it = map.begin();
         it != map.end(); ++it ) {
        // Dataset element
        QDomElement datasetElement = doc.createElement( "Dataset" );
        mapElement.appendChild( datasetElement );
        QDomText datasetContent =
            doc.createTextNode( QString::number( it.key() ) );
        datasetElement.appendChild( datasetContent );
        // Color element
        createColorNode( doc, mapElement, "Color", it.data() );
    }
}


static void createDoubleMapNode( QDomDocument& doc, QDomNode& parent,
                                 const QString& elementName,
                                 const QMap< int, double >& map )
{
    QDomElement mapElement =
        doc.createElement( elementName );
    parent.appendChild( mapElement );
    for( QMap<int,double>::ConstIterator it = map.begin();
         it != map.end(); ++it ) {
        // Dataset element
        QDomElement valueElement = doc.createElement( "Value" );
        mapElement.appendChild( valueElement );
        QDomText valueContent =
            doc.createTextNode( QString::number( it.key() ) );
        valueElement.appendChild( valueContent );
        // Color element
        createDoubleNode( doc, mapElement, "Factor", it.data() );
    }
}


static void createRectNode( QDomDocument& doc, QDomNode& parent,
                            const QString& elementName, const QRect& rect )
{
    QDomElement rectElement = doc.createElement( elementName );
    parent.appendChild( rectElement );
    QDomElement xElement = doc.createElement( "X" );
    rectElement.appendChild( xElement );
    QDomText xContent = doc.createTextNode( QString::number( rect.x() ) );
    xElement.appendChild( xContent );
    QDomElement yElement = doc.createElement( "Y" );
    rectElement.appendChild( yElement );
    QDomText yContent = doc.createTextNode( QString::number( rect.y() ) );
    yElement.appendChild( yContent );
    QDomElement widthElement = doc.createElement( "Width" );
    rectElement.appendChild( widthElement );
    QDomText widthContent = doc.createTextNode( QString::number( rect.width() ) );
    widthElement.appendChild( widthContent );
    QDomElement heightElement = doc.createElement( "Height" );
    rectElement.appendChild( heightElement );
    QDomText heightContent = doc.createTextNode( QString::number( rect.height() ) );
    heightElement.appendChild( heightContent );
}


static void createStringListNodes( QDomDocument& doc, QDomNode& parent,
                                   const QString& elementName,
                                   const QStringList* list )
{
    if( !list )
        return;

    for( QStringList::ConstIterator it = list->begin();
         it != list->end(); ++it ) {
        QDomElement element = doc.createElement( elementName );
        parent.appendChild( element );
        QDomText elementContent = doc.createTextNode( *it );
        element.appendChild( elementContent );
    }
}


static void createChartValueNode( QDomDocument& doc, QDomNode& parent,
                                  const QString& elementName,
                                  const KDChartData& data )
{
    QDomElement element = doc.createElement( elementName );
    parent.appendChild( element );
    if( data.isDouble() )
        element.setAttribute( "DoubleValue",
                              QString::number( data.doubleValue() ) );
    else if( data.isString() )
        element.setAttribute( "StringValue", data.stringValue() );
    else
        element.setAttribute( "NoValue", "true" );
}


static void createChartFontNode( QDomDocument& doc, QDomNode& parent,
                                 const QString& elementName,
                                 const QFont& font, bool useRelFont,
                                 int relFont )
{
    QDomElement chartFontElement = doc.createElement( elementName );
    parent.appendChild( chartFontElement );
    createFontNode( doc, chartFontElement, "Font", font );
    createBoolNode( doc, chartFontElement, "UseRelFontSize", useRelFont );
    createIntNode( doc, chartFontElement, "RelFontSize", relFont );
}


static void createFontNode( QDomDocument& doc, QDomNode& parent,
                            const QString& elementName, const QFont& font )
{
    QDomElement fontElement = doc.createElement( elementName );
    parent.appendChild( fontElement );
    createStringNode( doc, fontElement, "Family", font.family() );
    createIntNode( doc, fontElement, "PointSize", font.pointSize() );
    createIntNode( doc, fontElement, "Weight", font.weight() );
    createBoolNode( doc, fontElement, "Italic", font.italic() );
    createIntNode( doc, fontElement, "CharSet", font.charSet() );
}


static QString penStyleToString( Qt::PenStyle style )
{
    switch( style ) {
    case Qt::NoPen:
        return "NoPen";
    case Qt::SolidLine:
        return "SolidLine";
    case Qt::DashLine:
        return "DashLine";
    case Qt::DotLine:
        return "DotLine";
    case Qt::DashDotLine:
        return "DashDotLine";
    case Qt::DashDotDotLine:
        return "DashDotDotLine";
    default: // should not happen
        return "SolidLine";
    }
}


static bool readStringNode( const QDomElement& element, QString& value )
{
    value = element.text();
    return true;
}


static bool readIntNode( const QDomElement& element, int& value )
{
    bool ok = false;
    int temp = element.text().toInt( &ok );
    if( ok )
        value = temp;
    return ok;
}


static bool readDoubleNode( const QDomElement& element, double& value )
{
    bool ok = false;
    double temp = element.text().toDouble( &ok );
    if( ok )
        value = temp;
    return ok;
}


static bool readBoolNode( const QDomElement& element, bool& value )
{
    if( element.text() == "true" ) {
        value = true;
        return true;
    } else if( element.text() == "false" ) {
        value = false;
        return true;
    } else
        return false;
}


static bool readColorNode( const QDomElement& element, QColor& value )
{
    bool ok = true;
    int red, green, blue;
    if( element.hasAttribute( "Red" ) ) {
        bool redOk = false;
        red = element.attribute( "Red" ).toInt( &redOk );
        ok = ok & redOk;
    }
    if( element.hasAttribute( "Green" ) ) {
        bool greenOk = false;
        green = element.attribute( "Green" ).toInt( &greenOk );
        ok = ok & greenOk;
    }
    if( element.hasAttribute( "Blue" ) ) {
        bool blueOk = false;
        blue = element.attribute( "Blue" ).toInt( &blueOk );
        ok = ok & blueOk;
    }

    if( ok )
        value.setRgb( red, green, blue );

    return ok;
}


static bool readColorMapNode( const QDomElement& element,
                              QMap<uint,QColor>* value )
{
    QDomNode node = element.firstChild();
    int curDataset = -1;
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Dataset" ) {
                readIntNode( element, curDataset );
            } else if( tagName == "Color" ) {
                ASSERT( curDataset != -1 ); // there was a Dataset tag before
                QColor color;
                readColorNode( element, color );
                value->insert( curDataset, color );
            } else {
                qDebug( "Unknown tag in color map" );
                return false;
            }
        }
        node = node.nextSibling();
    }

    return true;
}


static bool readDoubleMapNode( const QDomElement& element,
                               QMap<int,double>* value )
{
    QDomNode node = element.firstChild();
    int curValue = -1;
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Value" ) {
                readIntNode( element, curValue );
            } else if( tagName == "Factor" ) {
                ASSERT( curValue != -1 ); // there was a Value tag before
                double doubleValue;
                readDoubleNode( element, doubleValue );
                value->insert( curValue, doubleValue );
            } else {
                qDebug( "Unknown tag in double map" );
                return false;
            }
        }
        node = node.nextSibling();
    }

    return true;
}


static bool readFontNode( const QDomElement& element, QFont& font )
{
    bool ok = true;
    QString family;
    int pointSize, weight;
    bool italic;
    int charSet;
    QDomNode node = element.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Family" ) {
                ok = ok & readStringNode( element, family );
            } else if( tagName == "PointSize" ) {
                ok = ok & readIntNode( element, pointSize );
            } else if( tagName == "Weight" ) {
                ok = ok & readIntNode( element, weight );
            } else if( tagName == "Italic" ) {
                ok = ok & readBoolNode( element, italic );
            } else if( tagName == "CharSet" ) {
                ok = ok & readIntNode( element, charSet );
            } else {
                qDebug( "Unknown tag in color map" );
            }
        }
        node = node.nextSibling();
    }

    if( ok ) {
        font.setFamily( family );
        font.setPointSize( pointSize );
        font.setWeight( weight );
        font.setItalic( italic );
        font.setCharSet( (QFont::CharSet)charSet );
    }

    return ok;
}


static bool readChartFontNode( const QDomElement& element, QFont& font,
                               bool& useRelFont, int& relFontSize )
{
    bool ok = true;
    QFont tempFont;
    bool tempRelFont;
    int tempRelFontSize;
    QDomNode node = element.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Font" ) {
                ok = ok & readFontNode( element, tempFont );
            } else if( tagName == "UseRelFontSize" ) {
                ok = ok & readBoolNode( element, tempRelFont );
            } else if( tagName == "RelFontSize" ) {
                ok = ok & readIntNode( element, tempRelFontSize );
            } else {
                qDebug( "Unknown tag in color map" );
            }
        }
        node = node.nextSibling();
    }

    if( ok ) {
        font = tempFont;
        useRelFont = tempRelFont;
        relFontSize = tempRelFontSize;
    }

    return ok;
}


static bool readRectNode( const QDomElement& element, QRect& value )
{
    bool ok = true;
    int width, height, x, y;
    QDomNode node = element.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            if( tagName == "Width" ) {
                ok = ok & readIntNode( element, width );
            } else if( tagName == "Height" ) {
                ok = ok & readIntNode( element, height );
            } else if( tagName == "X" ) {
                ok = ok & readIntNode( element, x );
            } else if( tagName == "Y" ) {
                ok = ok & readIntNode( element, y );
            } else {
                qDebug( "Unknown tag in rect" );
            }
        }
        node = node.nextSibling();
    }

    if( ok ) {
        value.setX( x );
        value.setY( y );
        value.setWidth( width );
        value.setHeight( height );
    }

    return ok;
}

// PENDING(kalle) Support DateTime values, even when writing.
static bool readChartValueNode( const QDomElement& element,
                                KDChartData& value )
{
    bool ok = true;
    if( element.hasAttribute( "NoValue" ) )
        value = KDChartData();
    else if( element.hasAttribute( "DoubleValue" ) ) {
        double d = element.attribute( "DoubleValue" ).toDouble( &ok );
        if( ok )
              value = KDChartData( d );
    } else if( element.hasAttribute( "StringValue" ) )
        value = KDChartData( element.attribute( "StringValue" ) );
    else // should not happen
        ok = false;

    return ok;
}



static Qt::PenStyle stringToPenStyle( const QString& style )
{
    if( style == "NoPen" )
        return Qt::NoPen;
    else if( style == "SolidLine" )
        return Qt::SolidLine;
    else if( style == "DashLine" )
        return Qt::DashLine;
    else if( style == "DotLine" )
        return Qt::DotLine;
    else if( style == "DashDotLine" )
        return Qt::DashDotLine;
    else if( style == "DashDotDotLine" )
        return Qt::DashDotDotLine;
    else // should not happen
        return Qt::SolidLine;
}
