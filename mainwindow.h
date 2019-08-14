/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the QtSerialBus module.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModbusDataUnit>
#include <QBasicTimer>
#include <QWidget>
QT_BEGIN_NAMESPACE

class QModbusClient;
class QModbusReply;

namespace Ui {
class MainWindow;
class SettingsDialog;
}

QT_END_NAMESPACE

class SettingsDialog;
class WriteRegisterModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void set_coil(int index , int value);

    void readbus1();
     void readbus2();
    void connect_prss();

    ~MainWindow();

private:
    void initActions();
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;
     QModbusDataUnit   read_coil_Request() const;

private slots:

    void onStateChanged(int state);


    void readReady();
    void readReady1();
        void on_connectButton_clicked();
    void on_connectType_currentIndexChanged(int);
    void on_writeTable_currentIndexChanged(int);

    void on_portEdit_cursorPositionChanged(int arg1, int arg2);

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_readButton_clicked();

    void on_pushButton_7_clicked();

    void on_ipushButton_1_clicked();

    void on_lcdNum1_overflow();

private:
    Ui::MainWindow *ui;
    QModbusReply *lastRequest;
    QModbusClient *modbusDevice;
    SettingsDialog *m_settingsDialog;
    WriteRegisterModel *writeModel;
     int timerId;

protected:
    void timerEvent(QTimerEvent *event);
};




#endif // MAINWINDOW_H
