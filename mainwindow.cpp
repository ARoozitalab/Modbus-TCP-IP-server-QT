/*********************************************************************************************************
*
* File                : mainwindow.c
* Version             : V1.0
* modified            : alireza roozitalab (adding modbust tcp for scada)
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*
*                                          All Rights Reserved
*
*********************************************************************************************************/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "writeregistermodel.h"

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>

#include <qtextstream.h>
#include "wiringPi.h"
#include <QTextStream>
#include <QTextStreamFunction>
#include <QString>
#include <QFile>
#include <stdio.h>
#include <qfile.h>
#include <stdlib.h>
#include <stdint.h>


enum ModbusConnection {
    Serial,
    Tcp
};

char rname[23][10]=
{
  "relay 1",
  "relay 2",
  "relay 3",
  "input 1",
  "input 2",
  "input 3",
  "input 4",
  "input 5",
  "input 6",
  "input 7",
  "input 8",
  "phase 1",
  "phase 2",
  "phase 3",
  "ADC 1",
  "ADC 2",
  "ADC 3",
  "LOOP 1",
  "LOOP 2",
  "LOOP 3",
  "VBAT",
  "VIN"
};

int num_coil=0;
int err=0;
int init_f=1;
int init_f1=1;
int relay1=0,relay2=0,relay3=0,relay4=0;
int read_num=13;
double acdz1=1,acdz2=1,acdz3=1,acdz4=1,acdz5=1,acdz6=1,acdz7=1;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lastRequest(nullptr)
    , modbusDevice(nullptr)
{
    ui->setupUi(this);

    m_settingsDialog = new SettingsDialog(this);

    initActions();

    writeModel = new WriteRegisterModel(this);

    on_connectType_currentIndexChanged(0);

    auto model = new QStandardItemModel(10, 1, this);
    for (int i = 0; i < 10; ++i)
        model->setItem(i, new QStandardItem(QStringLiteral("%1").arg(i + 1)));
  timerId = startTimer(3000);

if(init_f1)
{

    QPixmap pixmap(":/images/HALT1.bmp");
    QIcon ButtonIcon(pixmap);
    ui->pushButton->setIcon(ButtonIcon);

    QPixmap pixmap1(":/images/HALT2.bmp");
    QIcon ButtonIcon1(pixmap1);
        ui->pushButton_2->setIcon(ButtonIcon1);


        QPixmap pixmap2(":/images/HALT3.bmp");
        QIcon ButtonIcon2(pixmap2);
        ui->pushButton_3->setIcon(ButtonIcon2);

        QPixmap pixmap3(":/images/HALT4.bmp");
        QIcon ButtonIcon3(pixmap3);
        ui->pushButton_4->setIcon(ButtonIcon3);


 init_f1=0;
}




}

MainWindow::~MainWindow()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
 killTimer(timerId);
    delete ui;
}

void MainWindow::initActions()
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionExit->setEnabled(true);
    ui->actionOptions->setEnabled(true);

    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::on_connectButton_clicked);
    connect(ui->actionDisconnect, &QAction::triggered,
            this, &MainWindow::on_connectButton_clicked);

    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionOptions, &QAction::triggered, m_settingsDialog, &QDialog::show);
}

void MainWindow::on_connectType_currentIndexChanged(int index)
{

    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;

        modbusDevice = nullptr;
    }

    auto type = static_cast<ModbusConnection> (index);

        modbusDevice = new QModbusTcpClient(this);
        if (ui->portEdit->text().isEmpty())
            ui->portEdit->setText(QLatin1Literal("192.168.1.100"));


    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        statusBar()->showMessage(modbusDevice->errorString(), 5000);
    });

    if (!modbusDevice) {

        ui->connectButton->setDisabled(true);
        if (type == Serial)
            statusBar()->showMessage(tr("Could not create Modbus master."), 5000);
        else
            statusBar()->showMessage(tr("Could not create Modbus client."), 5000);
    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &MainWindow::onStateChanged);
    }
}

void MainWindow::on_connectButton_clicked()
{
    if (!modbusDevice)
        return;


    statusBar()->clearMessage();
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {

            modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, "502");
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter,ui->portEdit->text());

        modbusDevice->setTimeout(m_settingsDialog->settings().responseTime);
        modbusDevice->setNumberOfRetries(m_settingsDialog->settings().numberOfRetries);


        if (!modbusDevice->connectDevice()) {

            statusBar()->showMessage(tr("Connect failed: ") + modbusDevice->errorString(), 5000);
        } else {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
        }
    } else {

        modbusDevice->disconnectDevice();
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
    }


}

void MainWindow::onStateChanged(int state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    ui->actionConnect->setEnabled(!connected);
    ui->actionDisconnect->setEnabled(connected);

    if (state == QModbusDevice::UnconnectedState)
        ui->connectButton->setText(tr("Connect"));
    else if (state == QModbusDevice::ConnectedState)
        ui->connectButton->setText(tr("Disconnect"));

     //   ui->connectButton->setIcon(QIcon("off.xpm"));

     //   init_f1=0;

        readbus1();

        readbus2();

}

void MainWindow::readbus2()
{
    if (!modbusDevice)
        return;

    statusBar()->clearMessage();
 read_num=2;
    if (auto *reply = modbusDevice->sendReadRequest(read_coil_Request(), 1)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::readReady1);
        else
            delete reply; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);

    }


}

void MainWindow::readbus1()
{
    if (!modbusDevice)
        return;

    statusBar()->clearMessage();
 read_num=1;
    if (auto *reply = modbusDevice->sendReadRequest(readRequest(), 1)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);

    }


}



void MainWindow::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

double test=0;
    if (reply->error() == QModbusDevice::NoError) {

        const QModbusDataUnit unit = reply->result();
        err=0;
       // QModbusDataUnit::Coils;
      //  QModbusDataUnit::InputRegisters;
     //   QModbusDataUnit::HoldingRegisters;

    //    for (uint i = 0; i < unit.valueCount(); i++) {
     //         test=unit.value(0);
          //  const QString entry = tr("%1: %2").arg(rname[i]);



     //   }

        ui->lcdNum1->display((double)((unit.value(0))*acdz1));
        ui->lcdNum2->display((double)((unit.value(1))*acdz2));
        ui->lcdNum3->display((double)((unit.value(2))*acdz3));
        ui->lcdNum4->display((double)((unit.value(3))*acdz4));
        ui->lcdNum5->display((double)((unit.value(4))*acdz5));
        ui->lcdNum6->display((double)((unit.value(5))*acdz6));
        ui->lcdNum7->display((double)((unit.value(6))*acdz7));

    } else if (reply->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16), 5000);

    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16), 5000);

    }

    reply->deleteLater();
}






void MainWindow::readReady1()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {

        const QModbusDataUnit unit = reply->result();
        err=0;
       // QModbusDataUnit::Coils;
      //  QModbusDataUnit::InputRegisters;
     //   QModbusDataUnit::HoldingRegisters;

//        for (uint i = 0; i < unit.valueCount(); i++) {
           //   test13=unit.value(i);
          //  const QString entry = tr("coil%1: %2").arg(i).arg(test13);



        if(unit.value(0)==1)    //r1
        {
      QPixmap pixmap(":/images/ion.bmp");
      QIcon ButtonIcon(pixmap);
      ui->ipushButton_13->setIcon(ButtonIcon);
        }
        else
        {
            QPixmap pixmap(":/images/ioff.bmp");
            QIcon ButtonIcon(pixmap);
            ui->ipushButton_13->setIcon(ButtonIcon);

        }


        if(unit.value(1)==1)    //r2
        {
      QPixmap pixmap(":/images/ion.bmp");
      QIcon ButtonIcon(pixmap);
      ui->ipushButton_14->setIcon(ButtonIcon);
        }
        else
        {
            QPixmap pixmap(":/images/ioff.bmp");
            QIcon ButtonIcon(pixmap);
            ui->ipushButton_14->setIcon(ButtonIcon);

        }


        if(unit.value(2)==1)    //r3
        {
      QPixmap pixmap(":/images/ion.bmp");
      QIcon ButtonIcon(pixmap);
      ui->ipushButton_15->setIcon(ButtonIcon);
        }
        else
        {
            QPixmap pixmap(":/images/ioff.bmp");
            QIcon ButtonIcon(pixmap);
            ui->ipushButton_15->setIcon(ButtonIcon);

        }


        if(unit.value(3)==1)    //r4
        {
      QPixmap pixmap(":/images/ion.bmp");
      QIcon ButtonIcon(pixmap);
      ui->ipushButton_16->setIcon(ButtonIcon);
        }
        else
        {
            QPixmap pixmap(":/images/ioff.bmp");
            QIcon ButtonIcon(pixmap);
            ui->ipushButton_16->setIcon(ButtonIcon);

        }

                if(unit.value(4)==1)    //i1
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_1->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_1->setIcon(ButtonIcon);

                }

                if(unit.value(5)==1)//i2
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_2->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_2->setIcon(ButtonIcon);

                }

                if(unit.value(6)==1)//i3
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_3->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_3->setIcon(ButtonIcon);

                }


                if(unit.value(7)==1)//i4
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_4->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_4->setIcon(ButtonIcon);

                }


                if(unit.value(8)==1)//i5
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_5->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_5->setIcon(ButtonIcon);

                }


                if(unit.value(9)==1)//i6
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_6->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_6->setIcon(ButtonIcon);

                }

                if(unit.value(10)==1)//i7
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_7->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_7->setIcon(ButtonIcon);

                }

                if(unit.value(11)==1)//i8
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_8->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_8->setIcon(ButtonIcon);

                }
                if(unit.value(12)==1)//i9
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_9->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_9->setIcon(ButtonIcon);

                }

                if(unit.value(13)==1)//i10
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_10->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_10->setIcon(ButtonIcon);

                }

                if(unit.value(14)==1)//i11
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_11->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_11->setIcon(ButtonIcon);

                }

                if(unit.value(15)==1)//i12
                {
              QPixmap pixmap(":/images/ion.bmp");
              QIcon ButtonIcon(pixmap);
              ui->ipushButton_12->setIcon(ButtonIcon);
                }
                else
                {
                    QPixmap pixmap(":/images/ioff.bmp");
                    QIcon ButtonIcon(pixmap);
                    ui->ipushButton_12->setIcon(ButtonIcon);

                }





//        }




    } else if (reply->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16), 5000);

    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16), 5000);

    }

    reply->deleteLater();
}










void MainWindow::set_coil(int index , int value)
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = writeRequest();

            writeUnit.setValue(index,value);


    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, 1)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                        5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                        arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        statusBar()->showMessage(tr("Write error: ") + modbusDevice->errorString(), 5000);
    }
}



//void MainWindow::get_coil(int *index , int *value)
//{
//if (!modbusDevice)
//    return;
//ui->readValue->clear();
//statusBar()->clearMessage();

//if (auto *reply = modbusDevice->sendReadRequest(readRequest(), ui->serverEdit->value())) {
//    if (!reply->isFinished())
//        connect(reply, &QModbusReply::finished, this, &MainWindow::readReady);
//    else
//        delete reply; // broadcast replies return immediately
//} else {
//    statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
//}
//}

void MainWindow::on_writeTable_currentIndexChanged(int index)
{
    const bool coilsOrHolding = index == 0 || index == 3;
    if (coilsOrHolding) {

    }



}

QModbusDataUnit MainWindow::readRequest() const
{


    return QModbusDataUnit(QModbusDataUnit::InputRegisters, 0, 13);
}

QModbusDataUnit MainWindow::writeRequest() const
{

    return QModbusDataUnit(QModbusDataUnit::Coils, num_coil, 1);
}

QModbusDataUnit MainWindow::read_coil_Request() const
{

    return QModbusDataUnit(QModbusDataUnit::Coils, 0, 16);
}

void MainWindow::on_portEdit_cursorPositionChanged(int arg1, int arg2)
{

}

void MainWindow::on_pushButton_3_clicked()
{
    if(relay1==0)
    {
        QPixmap pixmap(":/images/ON1.bmp");
        QIcon ButtonIcon(pixmap);
        ui->pushButton_3->setIcon(ButtonIcon);
        relay1=1;
        num_coil=1;
        set_coil(0, 1);
    }
    else
    {
        relay1=0;
        QPixmap pixmap(":/images/OFF1.bmp");
        QIcon ButtonIcon(pixmap);
          ui->pushButton_3->setIcon(ButtonIcon);
        num_coil=1;
      set_coil(0, 0);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(relay4==0)
    {
        QPixmap pixmap(":/images/ON4.bmp");
        QIcon ButtonIcon(pixmap);
        ui->pushButton_4->setIcon(ButtonIcon);
        relay4=1;
        num_coil=4;
        set_coil(0, 1);
    }
    else
    {
        relay4=0;
        QPixmap pixmap(":/images/OFF4.bmp");
        QIcon ButtonIcon(pixmap);
          ui->pushButton_4->setIcon(ButtonIcon);
        num_coil=4;
      set_coil(0, 0);
    }
}

void MainWindow::on_pushButton_6_clicked()
{

}

void MainWindow::on_pushButton_clicked()
{
    if(relay3==0)
    {
        QPixmap pixmap(":/images/ON3.bmp");
        QIcon ButtonIcon(pixmap);
        ui->pushButton->setIcon(ButtonIcon);
        relay3=1;
        num_coil=3;
        set_coil(0, 1);
    }
    else
    {
        relay3=0;
        QPixmap pixmap(":/images/OFF3.bmp");
        QIcon ButtonIcon(pixmap);
          ui->pushButton->setIcon(ButtonIcon);
        num_coil=3;
      set_coil(0, 0);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    if(relay2==0)
    {
        QPixmap pixmap(":/images/ON2.bmp");
        QIcon ButtonIcon(pixmap);
        ui->pushButton_2->setIcon(ButtonIcon);
        relay2=1;
        num_coil=2;
        set_coil(0, 1);
    }
    else
    {
        relay2=0;
        QPixmap pixmap(":/images/OFF2.bmp");
        QIcon ButtonIcon(pixmap);
          ui->pushButton_2->setIcon(ButtonIcon);
        num_coil=2;
      set_coil(0, 0);
    }
}



void MainWindow::on_readButton_clicked()
{
 readbus1();
 readbus2();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    readbus1();

    readbus2();

    if(init_f)
    {

        QFile file("readme.txt");
       if (file.open(QIODevice::ReadOnly | QIODevice::Text))
       {

        QTextStream in(&file);

         // const QString line = in.readLine();
      ui->label1->setText(in.readLine());
      ui->label2->setText(in.readLine());
      ui->label3->setText(in.readLine());
      ui->label4->setText(in.readLine());
      ui->label5->setText(in.readLine());
      ui->label6->setText(in.readLine());
      ui->label7->setText(in.readLine());
      ui->label8->setText(in.readLine());
      ui->label9->setText(in.readLine());
      ui->label10->setText(in.readLine());
      ui->label11->setText(in.readLine());
      ui->label12->setText(in.readLine());
      ui->label13->setText(in.readLine());
      ui->label14->setText(in.readLine());
      ui->label15->setText(in.readLine());
      ui->label16->setText(in.readLine());
      ui->label17->setText(in.readLine());
      ui->label18->setText(in.readLine());
      ui->label19->setText(in.readLine());
      ui->label20->setText(in.readLine());
      ui->label21->setText(in.readLine());
      ui->label22->setText(in.readLine());
      ui->label23->setText(in.readLine());
      ui->label24->setText(in.readLine());
      ui->label25->setText(in.readLine());


      QString ali=in.readLine();  acdz1= ali.toDouble();
       ali=in.readLine();  acdz2= ali.toDouble();
       ali=in.readLine();  acdz3= ali.toDouble();
       ali=in.readLine();  acdz4= ali.toDouble();
       ali=in.readLine();  acdz5= ali.toDouble();
       ali=in.readLine();  acdz6= ali.toDouble();
       ali=in.readLine();  acdz7= ali.toDouble();
    //     acdz2=1
    //     acdz3=1
    //     acdz4=1
    //     acdz5=1
    //     acdz6=1
    //     acdz7=1;


     file.close();
       }
      else if (file.open(QIODevice::WriteOnly | QIODevice::Text))
       {

        QTextStream out(&file);
         // const QString line = in.readLine();

      out<<"Relay1" <<"\n";
      out<<"Relay2" <<"\n";
      out<<"Relay3" <<"\n";
      out<<"Relay4" <<"\n";

      out<<"I1" <<"\n";
      out<<"I2" <<"\n";
      out<<"I3" <<"\n";
      out<<"I4" <<"\n";
      out<<"I5" <<"\n";
      out<<"I6" <<"\n";
      out<<"I7" <<"\n";
      out<<"I8" <<"\n";
      out<<"I9" <<"\n";
      out<<"I10" <<"\n";
      out<<"I11" <<"\n";
      out<<"I12" <<"\n";


      out<<"TEMP" <<"\n";
      out<<"HUM" <<"\n";
      out<<"ADC1" <<"\n";
      out<<"ADC2" <<"\n";
      out<<"ADC3" <<"\n";
      out<<"ADC4" <<"\n";
      out<<"ADC5" <<"\n";


      out<<"PWM1" <<"\n";
      out<<"PWM2" <<"\n";

      out<<"1" <<"\n";
      out<<"1" <<"\n";
      out<<"1" <<"\n";
      out<<"1" <<"\n";
      out<<"1" <<"\n";
      out<<"1" <<"\n";
      out<<"1" <<"\n";
      out<<"    by:alireza roozitalab" <<"\n";
      out<<"    alireza.roozitalab@gmail.com" <<"\n";


      file.close();
      if (file.open(QIODevice::ReadOnly | QIODevice::Text))
      {

       QTextStream in(&file);

        // const QString line = in.readLine();
     ui->label1->setText(in.readLine());
     ui->label2->setText(in.readLine());
     ui->label3->setText(in.readLine());
     ui->label4->setText(in.readLine());
     ui->label5->setText(in.readLine());
     ui->label6->setText(in.readLine());
     ui->label7->setText(in.readLine());
     ui->label8->setText(in.readLine());
     ui->label9->setText(in.readLine());
     ui->label10->setText(in.readLine());
     ui->label11->setText(in.readLine());
     ui->label12->setText(in.readLine());
     ui->label13->setText(in.readLine());
     ui->label14->setText(in.readLine());
     ui->label15->setText(in.readLine());
     ui->label16->setText(in.readLine());
     ui->label17->setText(in.readLine());
     ui->label18->setText(in.readLine());
     ui->label19->setText(in.readLine());
     ui->label20->setText(in.readLine());
     ui->label21->setText(in.readLine());
     ui->label22->setText(in.readLine());
     ui->label23->setText(in.readLine());
     ui->label24->setText(in.readLine());
     ui->label25->setText(in.readLine());


     QString ali=in.readLine();  acdz1= ali.toDouble();
      ali=in.readLine();  acdz2= ali.toDouble();
      ali=in.readLine();  acdz3= ali.toDouble();
      ali=in.readLine();  acdz4= ali.toDouble();
      ali=in.readLine();  acdz5= ali.toDouble();
      ali=in.readLine();  acdz6= ali.toDouble();
      ali=in.readLine();  acdz7= ali.toDouble();
   //     acdz2=1
   //     acdz3=1
   //     acdz4=1
   //     acdz5=1
   //     acdz6=1
   //     acdz7=1;


    file.close();
      }










       }





//       on_connectButton_clicked();
      init_f=0  ;

    }

}

void MainWindow::on_pushButton_7_clicked()
{
    if(relay4==0)
    {
     num_coil=4;
    set_coil(0, 1);
    relay4=1;

    }
    else
    {
     num_coil=4;
    set_coil(0, 0);
     relay4=0;
    }

}

void MainWindow::on_ipushButton_1_clicked()
{

}

void MainWindow::on_lcdNum1_overflow()
{

}
