CONFIG += testcase
TARGET = tst_qjsvalue
macx:CONFIG -= app_bundle
QT += declarative widgets testlib
SOURCES  += tst_qjsvalue.cpp
HEADERS  += tst_qjsvalue.h

win32-msvc* {
    # With -O2, MSVC takes up to 24 minutes to compile this test!
    QMAKE_CXXFLAGS_RELEASE -= -O1 -O2
    QMAKE_CXXFLAGS_RELEASE += -Od
}
