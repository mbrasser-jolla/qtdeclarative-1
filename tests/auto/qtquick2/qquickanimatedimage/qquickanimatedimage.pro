CONFIG += testcase
TARGET = tst_qquickanimatedimage
HEADERS += ../../shared/testhttpserver.h
SOURCES += tst_qquickanimatedimage.cpp \
           ../../shared/testhttpserver.cpp

include (../../shared/util.pri)

macx:CONFIG -= app_bundle

testDataFiles.files = data
testDataFiles.path = .
DEPLOYMENT += testDataFiles

CONFIG += parallel_test

QT += core-private gui-private declarative-private quick-private network testlib
