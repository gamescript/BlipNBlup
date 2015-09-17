LIBS += ../BlipNBlup/Urho3D/lib/libUrho3D.a \
    -lpthread \
    -ldl \
    -lGL

DEFINES += URHO3D_COMPILE_QT

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += \
    ../BlipNBlup/Urho3D/include \
    ../BlipNBlup/Urho3D/include/Urho3D/ThirdParty \

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    helper.cpp \
    mastercontrol.cpp \
    inputmaster.cpp \
    bnbcam.cpp \
    player.cpp

HEADERS += \
    helper.h \
    mastercontrol.h \
    inputmaster.h \
    bnbcam.h \
    player.h