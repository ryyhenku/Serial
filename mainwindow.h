#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QIODevice>
#include <QTextCodec>
#include <QTime>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void serial_init();//搜索当前可用串口
    QByteArray QString2Hex(QString hexStr);//QString转hex
    char ConvertHexChar(char c);//字符转数值

private slots:
    void on_pushButton_clicked();//打开（关闭串口）按钮槽函数
    void read_data();//读取数据槽函数
    void object_init();//界面透明测试
    void on_pushButton_2_clicked();//清除接收按钮槽函数
    void on_pushButton_4_clicked();//发送按钮槽函数
    void on_textEdit_textChanged();//接收数据窗口纵向滚动条保持在下方
    void on_pushButton_5_clicked();//串口更新按钮槽函数

private:
    QSerialPort serial;//串口对象
    Ui::MainWindow *ui;//界面ui对象
    QTimer timer1;//定时器对象
};
#endif // MAINWINDOW_H
