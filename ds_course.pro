QT       += core gui
QT  +=openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    InGame/Card/ActionParser.cpp \
    InGame/Basic.cpp \
    InGame/Buff/Buff.cpp \
    InGame/Buff/BuffParser.cpp \
    InGame/Buff/BuffSystem.cpp \
    InGame/Card/CardSystem.cpp \
    InGame/Card/GameCard.cpp \
    InGame/System.cpp \
    StageSelection/MyPushButton.cpp \
    StageSelection/Stagechoice1.cpp \
    StageSelection/levelgenerator.cpp \
    Title/login1.cpp \
    Title/setting.cpp \
    Title/widget.cpp \
    gameboard/gameboard.cpp \
    gameboard/myopenglwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    InGame/Card/ActionParser.h \
    InGame/Basic.h \
    InGame/Buff/Buff.h \
    InGame/Buff/BuffParser.h \
    InGame/Buff/BuffSystem.h \
    InGame/Card/CardSystem.h \
    InGame/Context.h \
    InGame/Card/GameCard.h \
    InGame/globalOptions.h \
    InGame/System.h \
    StageSelection/MyPushButton.h \
    StageSelection/Stagechoice1.h \
    StageSelection/levelgenerator.h \
    Title/login1.h \
    Title/setting.h \
    Title/widget.h \
    gameboard/gameboard.h \
    gameboard/myopenglwidget.h \
    mainwindow.h

FORMS += \
    InGame/CardView.ui \
    StageSelection/Stagechoice1.ui \
    Title/login1.ui \
    Title/setting.ui \
    Title/widget.ui \
    gameboard/gameboard.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    InGame.qrc \
    StageSelection/Stagechoice1.qrc \
    Title/src.qrc \
    gameboard/gameboard.qrc
DISTFILES += \
    InGame/buff.json \
    InGame/cards.json \
    InGame/interface.md

