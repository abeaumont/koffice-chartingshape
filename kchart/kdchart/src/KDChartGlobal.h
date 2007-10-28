/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2007 Klaralvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#ifndef __KDCHARTGLOBAL_H__
#define __KDCHARTGLOBAL_H__

#include <qglobal.h>

#include "kdchart_export.h"

template <typename T>
inline T & __kdab__dereference_for_methodcall( T & o ) {
    return o;
}

template <typename T>
inline T & __kdab__dereference_for_methodcall( T * o ) {
    return *o;
}

#define KDAB_SET_OBJECT_NAME( x ) __kdab__dereference_for_methodcall( x ).setObjectName( QLatin1String( #x ) )

/* vc.net2002 is 1300, vc.net2003 is 1310 */
#if defined(_MSC_VER) && _MSC_VER <= 1300
#define KDCHART_DECLARE_PRIVATE_DERIVED( X )      \
public:                                           \
    class Private;                                \
protected:                                        \
    inline Private * d_func();                    \
    inline const Private * d_func() const;        \
    explicit inline X( Private * );               \
private:                                          \
    void init();
#else
#define KDCHART_DECLARE_PRIVATE_DERIVED( X )      \
protected:                                        \
    class Private;                                \
    inline Private * d_func();                    \
    inline const Private * d_func() const;        \
    explicit inline X( Private * );               \
private:                                          \
    void init();
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1300
#define KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( X, ParentType )      \
public:                                           \
    class Private;                                \
protected:                                        \
    inline Private * d_func();                    \
    inline const Private * d_func() const;        \
    explicit inline X( Private *, ParentType );   \
private:                                          \
    void init();
#else
#define KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( X, ParentType )      \
protected:                                        \
    class Private;                                \
    inline Private * d_func();                    \
    inline const Private * d_func() const;        \
    explicit inline X( Private *, ParentType );   \
private:                                          \
    void init();
#endif

#define KDCHART_DECLARE_PRIVATE_DERIVED_QWIDGET( X )         \
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( X, QWidget* )

#define KDCHART_DECLARE_PRIVATE_BASE_VALUE( X )              \
public:                                                      \
    inline void swap( X & other ) { qSwap( _d, other._d ); } \
protected:                                                   \
    class Private;                                           \
    Private * d_func() { return _d; }                        \
    const Private * d_func() const { return _d; }            \
private:                                                     \
    void init();                                             \
    Private * _d;

#if defined(_MSC_VER) && _MSC_VER <= 1300
#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( X ) \
public:                                           \
    class Private;                                    \
protected:                                        \
    Private * d_func() { return _d; }                 \
    const Private * d_func() const { return _d; }     \
    explicit inline X( Private * );                   \
private:                                              \
    void init();                                      \
    Private * _d;
#else
#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( X ) \
protected:                                        \
    class Private;                                    \
    Private * d_func() { return _d; }                 \
    const Private * d_func() const { return _d; }     \
    explicit inline X( Private * );                   \
private:                                              \
    void init();                                      \
    Private * _d;
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1300
#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( X ) \
public:                                           \
    class Private;                                    \
protected:                                        \
    Private * d_func() { return _d; }                 \
    const Private * d_func() const { return _d; }     \
    explicit inline X( Private *, QWidget* );                  \
private:                                              \
    void init();                                      \
    Private * _d;
#else
#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( X ) \
protected:                                        \
    class Private;                                    \
    Private * d_func() { return _d; }                 \
    const Private * d_func() const { return _d; }     \
    explicit inline X( Private *, QWidget* );                  \
private:                                              \
    void init();                                      \
    Private * _d;
#endif


#define KDCHART_DERIVED_PRIVATE_FOOTER( CLASS, PARENT )     \
inline CLASS::CLASS( Private * p )                          \
  : PARENT( p ) { init(); }                                 \
inline CLASS::Private * CLASS::d_func()                     \
{ return static_cast<Private*>( PARENT::d_func() ); }       \
inline const CLASS::Private * CLASS::d_func() const         \
{ return static_cast<const Private*>( PARENT::d_func() ); }


#if defined(_MSC_VER) && _MSC_VER <= 1300
#define KDCHART_DECLARE_DERIVED_DIAGRAM( X, PLANE )     \
public:                                                 \
    class Private;                                      \
protected:                                              \
    inline Private * d_func();                          \
    inline const Private * d_func() const;              \
    explicit inline X( Private * );                     \
    explicit inline X( Private *, QWidget *, PLANE * ); \
private:                                                \
    void init();
#else
#define KDCHART_DECLARE_DERIVED_DIAGRAM( X, PLANE )     \
protected:                                              \
    class Private;                                      \
    inline Private * d_func();                          \
    inline const Private * d_func() const;              \
    explicit inline X( Private * );                     \
    explicit inline X( Private *, QWidget *, PLANE * ); \
private:                                                \
    void init();
#endif

#define KDCHART_IMPL_DERIVED_DIAGRAM( CLASS, PARENT, PLANE ) \
inline CLASS::CLASS( Private * p )                           \
    : PARENT( p ) { init(); }                                \
inline CLASS::CLASS(                                         \
    Private * p, QWidget* parent, PLANE * plane )            \
    : PARENT( p, parent, plane ) { init(); }                 \
inline CLASS::Private * CLASS::d_func()                      \
    { return static_cast<Private *>( PARENT::d_func() ); }   \
inline const CLASS::Private * CLASS::d_func() const          \
    { return static_cast<const Private *>( PARENT::d_func() ); }


#define KDCHART_IMPL_DERIVED_PLANE( CLASS, BASEPLANE )        \
inline CLASS::CLASS( Private * p, Chart* parent )           \
    : BASEPLANE( p, parent ) { init(); }                      \
inline CLASS::Private * CLASS::d_func()                       \
    { return static_cast<Private *>( BASEPLANE::d_func() ); } \
inline const CLASS::Private * CLASS::d_func() const           \
    { return static_cast<const Private *>( BASEPLANE::d_func() ); }


#include <QtAlgorithms> // qSwap
#ifndef QT_NO_STL
#include <algorithm>
#define KDCHART_DECLARE_SWAP_SPECIALISATION( X )            \
    template <> inline void qSwap<X>( X & lhs, X & rhs )    \
    { lhs.swap( rhs ); }                                    \
    namespace std {                                         \
        template <> inline void swap<X>( X & lhs, X & rhs ) \
        { lhs.swap( rhs ); }                                \
    }
#else
#define KDCHART_DECLARE_SWAP_SPECIALISATION( X )            \
    template <> inline void qSwap<X>( X & lhs, X & rhs )    \
    { lhs.swap( rhs ); }
#endif

#define KDCHART_DECLARE_SWAP_SPECIALISATION_DERIVED( X )    \
    KDCHART_DECLARE_SWAP_SPECIALISATION( X )

#define KDCHART_DECLARE_SWAP_BASE( X ) \
protected: \
    void doSwap( X& other ) \
    { qSwap( _d, other._d); }

#define KDCHART_DECLARE_SWAP_DERIVED( X ) \
    void swap( X& other ) { doSwap( other ); }

#if defined(Q_OS_WIN) && defined(QT_DLL)
#if defined(_MSC_VER) && _MSC_VER >= 1300
// workaround http://support.microsoft.com/default.aspx?scid=kb;en-us;309801
#include <QPointF>
#include <QVector>
template class Q_DECL_IMPORT QVector<QPointF>;
#endif
#endif

#include <Qt>

namespace KDChart {

enum DisplayRoles {
  DatasetPenRole = 0x0A79EF95,
  DatasetBrushRole,
  DataValueLabelAttributesRole,
  ThreeDAttributesRole,
  LineAttributesRole,
  ThreeDLineAttributesRole,
  BarAttributesRole,
  ThreeDBarAttributesRole,
  PieAttributesRole,
  ThreeDPieAttributesRole,
  DataHiddenRole,
  ValueTrackerAttributesRole
};
}

#endif // __KDCHARTGLOBAL_H__
