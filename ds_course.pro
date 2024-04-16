QT       += core gui
QT  +=openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    InGame/Basic.cpp \
    InGame/GameCard.cpp \
    InGame/InGame.cpp \
    InGame/styles.cpp \
    StageSelection/MyPushButton.cpp \
    StageSelection/Stagechoice1.cpp \
    gameboard/gameboard.cpp \
    gameboard/myopenglwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    InGame/Basic.h \
    InGame/GameCard.h \
    InGame/InGame.h \
    InGame/globalOptions.h \
    InGame/styles.h \
    StageSelection/MyPushButton.h \
    StageSelection/Stagechoice1.h \
    gameboard/gameboard.h \
    gameboard/myopenglwidget.h \
    mainwindow.h

FORMS += \
    InGame/CardView.ui \
    StageSelection/Stagechoice1.ui \
    gameboard/gameboard.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    StageSelection/Stagechoice1.qrc \
    #Stagechoice1.qrc \
    gameboard/gameboard.qrc

DISTFILES += \
    InGame/cards.json
