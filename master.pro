QT += serialbus serialport widgets

TARGET = modbusmaster
        target.files = modbus
        target.path = f:/modbus
        INSTALLS += target
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
        settingsdialog.cpp \
        writeregistermodel.cpp \


HEADERS  += mainwindow.h \
         settingsdialog.h \
        writeregistermodel.h \


FORMS    += mainwindow.ui \
         settingsdialog.ui

RESOURCES += \
    master.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialbus/modbus/master
INSTALLS += target

DISTFILES += \
    images/readme.txt
