/*********************************************************************************************************
*
* File                : main.c
* Version             : V1.0
* modified            : alireza roozitalab (adding modbust tcp for scada)
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*
*                                          All Rights Reserved
*
*********************************************************************************************************/


#include "mainwindow.h"

#include <QApplication>
#include <QLoggingCategory>

#include <QLineEdit>
#include <QVBoxLayout>
extern int init_f1;





int main(int argc, char *argv[])
{

    QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));
    QApplication a(argc, argv);

    MainWindow w;
    w.show();




    return a.exec();
}

