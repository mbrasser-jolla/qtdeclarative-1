CONFIG += testcase
TARGET = tst_qdeclarativesmoothedanimation
macx:CONFIG -= app_bundle

SOURCES += tst_qdeclarativesmoothedanimation.cpp

include (../../shared/util.pri)

testDataFiles.files = data
testDataFiles.path = .
DEPLOYMENT += testDataFiles

CONFIG += parallel_test

QT += core-private gui-private v8-private declarative-private quick-private testlib
