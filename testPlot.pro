QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets printsupport

TARGET = testPlot

INCLUDEPATH += "/usr/local/boost_1_63_0"

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    svimodel.cpp \
    calibrate.cpp \
    arbitrage.cpp \
    dialogrange.cpp \
    coordinate.cpp

FORMS += \
    mainwindow.ui \
    dialogrange.ui

HEADERS += \
    mainwindow.h \
    qcustomplot.h \
    svimodel.h \
    calibrate.h \
    arbitrage.h \
    dialogrange.h \
    coordinate.h
