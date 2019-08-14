/*********************************************************************************************************
*
* File                : settingdialog.c
* Version             : V1.0
* modified            : alireza roozitalab (adding modbust tcp for scada)
* email               : alireza.roozitalab@gmail.com
* git user            : ARoozitalab
*
*                                          All Rights Reserved
*
*********************************************************************************************************/


#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFile>
#include <stdio.h>
#include <qfile.h>
#include <QTextStream>
#include <QTextStreamFunction>
#include <QString>
extern int init_f;
extern double acdz1,acdz2,acdz3,acdz4,acdz5,acdz6,acdz7;
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);


    ui->timeoutSpinner->setValue(m_settings.responseTime);
    ui->retriesSpinner->setValue(m_settings.numberOfRetries);
    QFile file("readme.txt");
   if (file.open(QIODevice::ReadOnly | QIODevice::Text))
   {

    QTextStream in(&file);

     // const QString line = in.readLine();
    ui->lineEdit1->setText(in.readLine());
    ui->lineEdit2->setText(in.readLine());
    ui->lineEdit3->setText(in.readLine());
    ui->lineEdit4->setText(in.readLine());
    ui->lineEdit5->setText(in.readLine());
    ui->lineEdit6->setText(in.readLine());
    ui->lineEdit7->setText(in.readLine());
    ui->lineEdit8->setText(in.readLine());
    ui->lineEdit9->setText(in.readLine());
    ui->lineEdit10->setText(in.readLine());
    ui->lineEdit11->setText(in.readLine());
    ui->lineEdit12->setText(in.readLine());
    ui->lineEdit13->setText(in.readLine());
    ui->lineEdit14->setText(in.readLine());
    ui->lineEdit15->setText(in.readLine());
    ui->lineEdit16->setText(in.readLine());
    ui->lineEdit17->setText(in.readLine());
    ui->lineEdit18->setText(in.readLine());
    ui->lineEdit19->setText(in.readLine());
    ui->lineEdit20->setText(in.readLine());
    ui->lineEdit21->setText(in.readLine());
    ui->lineEdit22->setText(in.readLine());
    ui->lineEdit23->setText(in.readLine());
    ui->lineEdit24->setText(in.readLine());
    ui->lineEdit25->setText(in.readLine());




    ui->lineEdit26->setText(in.readLine());
    ui->lineEdit27->setText(in.readLine());
    ui->lineEdit28->setText(in.readLine());
    ui->lineEdit29->setText(in.readLine());
    ui->lineEdit30->setText(in.readLine());
    ui->lineEdit31->setText(in.readLine());
    ui->lineEdit32->setText(in.readLine());






  file.close();
   }



    connect(ui->applyButton, &QPushButton::clicked, [this]() {
        m_settings.responseTime = ui->timeoutSpinner->value();
        m_settings.numberOfRetries = ui->retriesSpinner->value();

        hide();
    });
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsDialog::Settings SettingsDialog::settings() const
{
    return m_settings;
}



void SettingsDialog::on_applyButton_clicked()
{


    QFile file("readme.txt");
   if (file.open(QIODevice::WriteOnly | QIODevice::Text))
   {

    QTextStream out(&file);
     // const QString line = in.readLine();

  out<< ui->lineEdit1->text()<<"\n";
  out<<    ui->lineEdit2->text()<<"\n";
   out<<   ui->lineEdit3->text()<<"\n";


  out<<  ui->lineEdit4->text()<<"\n";
   out<< ui->lineEdit5->text()<<"\n";
   out<< ui->lineEdit6->text()<<"\n";
   out<< ui->lineEdit7->text()<<"\n";
    out<<ui->lineEdit8->text()<<"\n";
   out<< ui->lineEdit9->text()<<"\n";
   out<< ui->lineEdit10->text()<<"\n";
   out<< ui->lineEdit11->text()<<"\n";
   out<< ui->lineEdit12->text()<<"\n";
    out<<ui->lineEdit13->text()<<"\n";
   out<< ui->lineEdit14->text()<<"\n";
  out<<  ui->lineEdit15->text()<<"\n";
   out<< ui->lineEdit16->text()<<"\n";
   out<< ui->lineEdit17->text()<<"\n";
   out<< ui->lineEdit18->text()<<"\n";
   out<< ui->lineEdit19->text()<<"\n";
  out<<  ui->lineEdit20->text()<<"\n";
   out<< ui->lineEdit21->text()<<"\n";
   out<< ui->lineEdit22->text()<<"\n";
   out<< ui->lineEdit23->text()<<"\n";
  out<<  ui->lineEdit24->text()<<"\n";
   out<< ui->lineEdit25->text()<<"\n";




   out<< ui->lineEdit26->text()<<"\n";
  out<<  ui->lineEdit27->text()<<"\n";
  out<<  ui->lineEdit28->text()<<"\n";
   out<< ui->lineEdit29->text()<<"\n";
  out<<  ui->lineEdit30->text()<<"\n";
   out<< ui->lineEdit31->text()<<"\n";
   out<< ui->lineEdit32->text()<<"\n";



   QString ali= ui->lineEdit26->text();  acdz1= ali.toDouble();
    ali=ui->lineEdit27->text();  acdz2= ali.toDouble();
    ali=ui->lineEdit28->text();  acdz3= ali.toDouble();
    ali=ui->lineEdit29->text();  acdz4= ali.toDouble();
    ali=ui->lineEdit30->text();  acdz5= ali.toDouble();
    ali=ui->lineEdit31->text(); acdz6= ali.toDouble();
    ali=ui->lineEdit32->text();  acdz7= ali.toDouble();

  init_f=1;
  file.close();
   }




}

void SettingsDialog::on_applyButton_2_clicked()
{

    QFile file("readme.txt");
   if (file.open(QIODevice::WriteOnly | QIODevice::Text))
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


  file.close();
   }

   if (file.open(QIODevice::ReadOnly | QIODevice::Text))
   {

    QTextStream in(&file);

     // const QString line = in.readLine();
    ui->lineEdit1->setText(in.readLine());
    ui->lineEdit2->setText(in.readLine());
    ui->lineEdit3->setText(in.readLine());
    ui->lineEdit4->setText(in.readLine());
    ui->lineEdit5->setText(in.readLine());
    ui->lineEdit6->setText(in.readLine());
    ui->lineEdit7->setText(in.readLine());
    ui->lineEdit8->setText(in.readLine());
    ui->lineEdit9->setText(in.readLine());
    ui->lineEdit10->setText(in.readLine());
    ui->lineEdit11->setText(in.readLine());
    ui->lineEdit12->setText(in.readLine());
    ui->lineEdit13->setText(in.readLine());
    ui->lineEdit14->setText(in.readLine());
    ui->lineEdit15->setText(in.readLine());
    ui->lineEdit16->setText(in.readLine());
    ui->lineEdit17->setText(in.readLine());
    ui->lineEdit18->setText(in.readLine());
    ui->lineEdit19->setText(in.readLine());
    ui->lineEdit20->setText(in.readLine());
    ui->lineEdit21->setText(in.readLine());
    ui->lineEdit22->setText(in.readLine());
    ui->lineEdit23->setText(in.readLine());
    ui->lineEdit24->setText(in.readLine());
    ui->lineEdit25->setText(in.readLine());




    ui->lineEdit26->setText(in.readLine());
    ui->lineEdit27->setText(in.readLine());
    ui->lineEdit28->setText(in.readLine());
    ui->lineEdit29->setText(in.readLine());
    ui->lineEdit30->setText(in.readLine());
    ui->lineEdit31->setText(in.readLine());
    ui->lineEdit32->setText(in.readLine());





  file.close();
   }




}
