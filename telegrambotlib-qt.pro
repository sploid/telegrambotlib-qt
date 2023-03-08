TARGET = telegrambotlib-qt
TEMPLATE = lib
QT += core network
QT -= gui
CONFIG += qt warn_on release exceptions stl embed_manifest_exe precompile_header console suppress_vcproj_warnings c++latest
CONFIG -= flat app_bundle
PRECOMPILED_HEADER = $$PWD/src/stdafx.h
QMAKE_CXXFLAGS_WARN_ON = -W4
QMAKE_CXXFLAGS += /Zi /MP
QMAKE_LFLAGS += /DEBUG /FIXED:NO
DEFINES += \
  BOT_LIB \
  QT_DISABLE_DEPRECATED_BEFORE=0x060000 \
  QT_DEPRECATED_WARNINGS \
  _CRT_SECURE_NO_WARNINGS \
  _SCL_SECURE_NO_WARNINGS \
  NOMINMAX \
  WIN32 \
  _WINDOWS

SOURCES += \
  $$PWD/src/telegrambot.cpp \
  $$PWD/src/jsonhelper.cpp \
  $$PWD/modules/sslserver/sslserver.cpp \
  $$PWD/modules/httpserver/httpserver.cpp

HEADERS += \
  $$PWD/src/common.h \
  $$PWD/src/stdafx.h \
  $$PWD/src/telegrambot.h \
  $$PWD/src/jsonhelper.h \
  $$PWD/src/telegramdatastructs.h \
  $$PWD/src/telegramdatainterface.h \
  $$PWD/modules/sslserver/sslserver.h \
  $$PWD/modules/httpserver/httpserver.h

INCLUDEPATH += \
  $$PWD/src \
  $$PWD/modules/sslserver \
  $$PWD/modules/httpserver

include($$PWD/vendor/qdelegate/QDelegate.pri)