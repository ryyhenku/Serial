#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QIcon"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    object_init();

    serial_init();
    connect(&serial,SIGNAL(readyRead()),
            this,SLOT(read_data()));
    //timer1.start(1000);
    connect(&timer1,SIGNAL(timeout()),
            this,SLOT(time_out_fuc()));
    ui->pushButton_4->setEnabled(false);
    //this->setWindowIcon(QIcon(":/img/icon.ico"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="接收数据"){
    serial.setPortName(ui->comboBox->currentText());
    serial.setBaudRate(ui->comboBox_2->currentText().toInt());
    if(ui->comboBox_3->currentText()=="None"){
        serial.setParity(QSerialPort::NoParity);
        }
    if(ui->comboBox_3->currentText()=="Odd"){
        serial.setParity(QSerialPort::OddParity);
        }

    if(ui->comboBox_3->currentText()=="Even"){
        serial.setParity(QSerialPort::EvenParity);
        }

    if(ui->comboBox_3->currentText()=="Mark"){
        serial.setParity(QSerialPort::MarkParity);
        }

    if(ui->comboBox_3->currentText()=="Space"){
        serial.setParity(QSerialPort::SpaceParity);
        }
    /*数据位*/
    switch (ui->comboBox_4->currentText().toUInt()) {
              case 8:serial.setDataBits(QSerialPort::Data8);break;
              case 7:serial.setDataBits(QSerialPort::Data7);break;
              case 6:serial.setDataBits(QSerialPort::Data6);break;
              case 5:serial.setDataBits(QSerialPort::Data5);break;
    }
    if(ui->comboBox_5->currentText()=="1"){
       serial.setStopBits(QSerialPort::OneStop);
        }

    if(ui->comboBox_5->currentText()=="1.5"){
       serial.setStopBits(QSerialPort::OneAndHalfStop);
        }
    if(ui->comboBox_5->currentText()=="2"){
       serial.setStopBits(QSerialPort::TwoStop);
        }
    if(serial.open(QIODevice::ReadWrite)==true){
        ui->pushButton->setText("停止接收");
        ui->pushButton_4->setEnabled(true);
        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->comboBox_4->setEnabled(false);
        ui->comboBox_5->setEnabled(false);


    }


    }
    else{
        serial.close();
        ui->pushButton->setText("接收数据");
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        ui->pushButton_4->setEnabled(true);

    }

}
void MainWindow:: serial_init()
{


    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos){
        ui->comboBox->addItem(info.portName());
         }

}
void MainWindow::read_data()
{
    QByteArray buf = serial.readLine();
    QTextCodec *tc1 = QTextCodec::codecForName("GBK");
    QString str  = tc1->toUnicode(buf);
    QString info = str.toUtf8();
    if(ui->checkBox_2->checkState()==Qt::Checked){
       info = QTime::currentTime().toString() +"  "+ info;
       ui->textEdit->insertPlainText(info);
    }
    else{
        ui->textEdit->insertPlainText(info);

    }

    if(ui->checkBox->checkState()==Qt::Checked){
        QByteArray hex_buf = QString2Hex(info);
        ui->textEdit->setText(QString(hex_buf));
    }

    buf.clear();

}

void MainWindow::object_init()
{


    QPalette pl = ui->textEdit->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    ui->textEdit->setPalette(pl);

    QPalette pl2 = ui->textEdit_2->palette();
    pl2.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    ui->textEdit_2->setPalette(pl2);


}



void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    QByteArray Buf1;
    QString str1 = ui->textEdit_2->toPlainText();
    Buf1 = str1.toLocal8Bit();
    serial.write(Buf1);
}



//字符串
QByteArray MainWindow::QString2Hex(QString hexStr)
{
    QByteArray senddata;
    int hexdata, lowhexdata;
    int hexdatalen = 0;
    int len = hexStr.length();
    senddata.resize(len/2);
    char lstr, hstr;
    for(int i = 0; i < len; )
    {
        //将第一个不为' '的字符赋给hstr;
        hstr = hexStr[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        //当i >= len时，跳出循环
        if(i >= len)
            break;
        //当i < len时，将下一个字符赋值给lstr;
        lstr = hexStr[i].toLatin1();
        //将hstr和lstr转换为0-15的对应数值
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        //
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata * 16 + lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}
//单个字符
char MainWindow::ConvertHexChar(char c)
{
    if((c >= '0') && (c <= '9'))
        return c - 0x30;
    else if((c >= 'A') && (c <= 'F'))
        return c - 'A' + 10;//'A' = 65;
    else if((c >= 'a') && (c <= 'f'))
        return c - 'a' + 10;
    else
        return -1;
}

void MainWindow::on_textEdit_textChanged()
{
    ui->textEdit->moveCursor(QTextCursor::End);
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->comboBox->clear();
    serial_init();
}
