#ifndef PORTCOMM_H
#define PORTCOMM_H

#include <QMainWindow>
#include <QTextCodec>
#include <QByteArray>
#include <QDateTime>
#include "win_qextserialport.h"//加入头文件

namespace Ui {
class portComm;
}

class portComm : public QMainWindow
{
    Q_OBJECT

public:
    explicit portComm(QWidget *parent = 0);
    ~portComm();

private:
    Ui::portComm *ui;
    Win_QextSerialPort *myCom;
    QByteArray send;
    byte cs;

private slots:
    void readMyCom();
    void doAddrTxtChanged(const QString&);
    void doIPortTxtChanged(const QString&);
    void doTimeTxtChanged(const QString&);
    void on_pushButtonAddr_clicked();
    void on_pushButtonIPort_clicked();
    void on_pushButtonTime_clicked();
    void on_pushButton_toggled(bool checked);
};

#endif // PORTCOMM_H
