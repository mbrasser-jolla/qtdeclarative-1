CONFIG += testcase
TARGET = tst_qquickparticlegroup
SOURCES += tst_qquickparticlegroup.cpp
macx:CONFIG -= app_bundle

testDataFiles.files = data
testDataFiles.path = .
DEPLOYMENT += testDataFiles

QT += core-private gui-private v8-private declarative-private quick-private opengl-private testlib

