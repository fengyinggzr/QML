#include "portcomm.h"
#include "ui_portcomm.h"

portComm::portComm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::portComm)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    ui->pushButton->setCheckable(true);

    connect(ui->lineEditAddr,SIGNAL(textChanged(QString)),this,SLOT(doAddrTxtChanged(QString)));
    connect(ui->lineEditIP1,SIGNAL(textChanged(QString)),this,SLOT(doIPortTxtChanged(QString)));
    connect(ui->lineEditIP2,SIGNAL(textChanged(QString)),this,SLOT(doIPortTxtChanged(QString)));
    connect(ui->lineEditIP3,SIGNAL(textChanged(QString)),this,SLOT(doIPortTxtChanged(QString)));
    connect(ui->lineEditIP4,SIGNAL(textChanged(QString)),this,SLOT(doIPortTxtChanged(QString)));
    connect(ui->lineEditPort,SIGNAL(textChanged(QString)),this,SLOT(doIPortTxtChanged(QString)));
    connect(ui->lineEditTime,SIGNAL(textChanged(QString)),this,SLOT(doTimeTxtChanged(QString)));
}

portComm::~portComm()
{
    delete ui;
}

void portComm::readMyCom() //读串口函数

{
    QByteArray temp = myCom->readAll();//读取串口缓冲区的所有数据给临时变量temp
    ui->textBrowser->insertPlainText(temp);//将串口的数据显示在窗口的文本浏览器中
}

void portComm::on_pushButtonAddr_clicked()
{
    cs=0x00;
    send.resize(11);
    send[0]=0x68;
    send[1]=0x09;
    send[2]=0x00;
    send[3]=0x09;
    send[4]=0x00;
    send[5]=0x68;
    send[6]=0x01;
    qint64 intAddr=ui->lineEditAddr->text().toInt();
    qint64 high=intAddr/256;
    qint64 low=intAddr%256;
    send[7]=high;
    send[8]=low;
    for(int i=0;i<9;i++)
    {
        cs+=send[i];
    }
    send[9]=cs;
    send[10]=0x16;

    myCom->write(send);
    cs=0x00;
    send.resize(0);
    ui->frameIPort->setEnabled(true);
    ui->frameTime->setEnabled(true);
}

void portComm::on_pushButtonIPort_clicked()
{
    cs=0x00;
    send.resize(14);
    send[0]=0x68;
    send[1]=0x12;
    send[2]=0x00;
    send[3]=0x12;
    send[4]=0x00;
    send[5]=0x68;
    send[6]=0x02;
    send[7]=ui->lineEditIP1->text().toInt();
    send[8]=ui->lineEditIP2->text().toInt();
    send[9]=ui->lineEditIP3->text().toInt();
    send[10]=ui->lineEditIP4->text().toInt();
    send[11]=ui->lineEditPort->text().toInt();
    for(int i=0;i<12;i++)
    {
        cs+=send[i];
    }
    send[12]=cs;
    send[13]=0x16;

    myCom->write(send);
    cs=0x00;
    send.resize(0);
    ui->frameAddr->setEnabled(true);
    ui->frameTime->setEnabled(true);
}

void portComm::on_pushButtonTime_clicked()
{
    QDateTime timeStr=QDateTime::currentDateTime();
    QString time=timeStr.toString("yyMMddhhmmss");
    cs=0x00;
    send.resize(15);
    send[0]=0x68;
    send[1]=0x13;
    send[2]=0x00;
    send[3]=0x13;
    send[4]=0x00;
    send[5]=0x68;
    send[6]=0x03;
    for(int i=0;i<11;i+=2)
    {
        qint64 temp=time.mid(i,2).toInt();
        int index=7+i/2;
        send[index]=temp;
    }
    for(int i=0;i<13;i++)
    {
        cs+=send[i];
    }
    send[13]=cs;
    send[14]=0x16;
    time=timeStr.toString("yy-MM-dd hh:mm:ss");
    ui->lineEditTime->setText(time);

    myCom->write(send);
    cs=0x00;
    send.resize(0);
    ui->frameAddr->setEnabled(true);
    ui->frameIPort->setEnabled(true);
}

void portComm::doAddrTxtChanged(const QString& txt)
{
    if(!txt.isEmpty())
    {
        ui->frameIPort->setEnabled(false);
        ui->frameTime->setEnabled(false);
    }
    else
    {
        ui->frameIPort->setEnabled(true);
        ui->frameTime->setEnabled(true);
    }
}

void portComm::doIPortTxtChanged(const QString& txt)
{
    if(!txt.isEmpty())
    {
        ui->frameAddr->setEnabled(false);
        ui->frameTime->setEnabled(false);
    }
    else
    {
        ui->frameAddr->setEnabled(true);
        ui->frameTime->setEnabled(true);
    }
}

void portComm::doTimeTxtChanged(const QString& txt)
{
    if(!txt.isEmpty())
    {
        ui->frameAddr->setEnabled(false);
        ui->frameIPort->setEnabled(false);
    }
    else
    {
        ui->frameAddr->setEnabled(true);
        ui->frameIPort->setEnabled(true);
    }
}

void portComm::on_pushButton_toggled(bool checked)
{
    if(checked)
    {
        QString portName = ui->comboBoxSerial->currentText(); //获取串口名
        myCom = new Win_QextSerialPort(portName,QextSerialBase::EventDriven);//定义串口对象，并传递参数，在构造函数里对其进行初始化
        myCom ->open(QIODevice::ReadWrite); //打开串口

        if(ui->comboBoxBaudRate->currentText()==tr("9600")) //根据组合框内容对串口进行设置
            myCom->setBaudRate(BAUD9600);
        else if(ui->comboBoxBaudRate->currentText()==tr("115200"))
            myCom->setBaudRate(BAUD115200);//设置波特率

        if(ui->comboBoxDataBits->currentText()==tr("8"))
            myCom->setDataBits(DATA_8);
        else if(ui->comboBoxDataBits->currentText()==tr("7"))
            myCom->setDataBits(DATA_7);//设置数据位

        if(ui->comboBoxParity->currentText()==tr("无"))
            myCom->setParity(PAR_NONE);
        else if(ui->comboBoxParity->currentText()==tr("奇"))
            myCom->setParity(PAR_ODD);
        else if(ui->comboBoxParity->currentText()==tr("偶"))
            myCom->setParity(PAR_EVEN);//设置奇偶校验

        if(ui->comboBoxStopBits->currentText()==tr("1"))
            myCom->setStopBits(STOP_1);
        else if(ui->comboBoxStopBits->currentText()==tr("2"))
            myCom->setStopBits(STOP_2);//设置停止位

        myCom->setFlowControl(FLOW_OFF); //设置数据流控制，我们使用无数据流控制的默认设置

        myCom->setTimeout(500); //设置延时

        connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));//信号和槽函数关联，当串口缓冲区有数据时，进行读串口操作

        ui->pushButton->setText(tr("关闭串口"));
        ui->frameSettings->setEnabled(false);
    }
    else
    {
        myCom->close();

        ui->pushButton->setText(tr("打开串口"));
        ui->frameSettings->setEnabled(true);
    }
}
