#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QSerialPort m_serialPort;

private slots:
    void onRefreshPortsClick();
    void onConnectClick();
    void onReadData();
    void onError(QSerialPort::SerialPortError pError);
    void onCloseClick();
    void onSendClick();
    void onTextChanged(QString pString);
};

#endif // MAINWINDOW_H
