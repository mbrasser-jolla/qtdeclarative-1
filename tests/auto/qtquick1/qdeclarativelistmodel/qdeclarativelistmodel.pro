CONFIG += testcase
TARGET = tst_qdeclarativelistmodel
macx:CONFIG -= app_bundle

SOURCES += tst_qdeclarativelistmodel.cpp

DEFINES += SRCDIR=\\\"$$PWD\\\"

CONFIG += parallel_test

QT += core-private gui-private v8-private widgets-private declarative-private qtquick1-private testlib
