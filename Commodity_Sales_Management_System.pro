QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commodityinformation.cpp \
    fileinteraction.cpp \
    login_form.cpp \
    main.cpp \
    mainwindow.cpp \
    purchasedialog.cpp \
    salesinformation.cpp \
    transactiondialog.cpp

HEADERS += \
    commodityinformation.h \
    fileinteraction.h \
    login_form.h \
    mainwindow.h \
    purchasedialog.h \
    salesinformation.h \
    transactiondialog.h

FORMS += \
    login_form.ui \
    mainwindow.ui \
    purchasedialog.ui \
    transactiondialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    CSMS.qrc \
