set( TestLoading_SRCS TestLoading.cpp ../TestLoadingBase.cpp ../../../ChartDocument.cpp )
kde4_add_unit_test( TestLoading TESTNAME kchart-TestLoading-@@NAME@@ ${TestLoading_SRCS} )
target_link_libraries( TestLoading  ${QT_QTTEST_LIBRARY}  chartshape )
