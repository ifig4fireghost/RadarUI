#-------------------------------------------------
#
# Project created by QtCreator 2018-05-07T16:16:37
#
#-------------------------------------------------

QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RadarUId
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += app core skin ui

SOURCES += main.cpp \
	app/ChartView.cpp \
	app/LmsDataParser.cpp \
	core/CoreRouter.cpp \
	core/libs.cpp \
	skin/ChangeSkin.cpp \
	ui/MainWindow.cpp \
	ui/SettingDialog.cpp \
	ui/UIWaiting.cpp

FORMS += \
	app/ChartView.ui \
	skin/ChangeSkin.ui \
	ui/MainWindow.ui \
	ui/SettingDialog.ui \
	ui/UIWaiting.ui

HEADERS += \
	app/ChartView.h \
	app/LmsDataParser.h \
	core/CommonDialog.h \
	core/CommonWidget.h \
	core/CoreRouter.h \
	core/libs.h \
	core/reflecthelper.h \
	skin/ChangeSkin.h \
	skin/ChangeStyle.h \
	ui/MainWindow.h \
	ui/SettingDialog.h \
	ui/UIWaiting.h

RESOURCES += \
	config.qrc \
	qss.qrc \
	texture.qrc

LIBS += -lz
