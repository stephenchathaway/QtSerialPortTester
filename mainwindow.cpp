#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->refreshPortsButton, SIGNAL(clicked()), this, SLOT(onRefreshPortsClick()));
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(onConnectClick()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(onCloseClick()));
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(onSendClick()));
    connect(ui->sendLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));

    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(onReadData()));
    connect(&m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(onError(QSerialPort::SerialPortError)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRefreshPortsClick()
{
    ui->portsComboBox->clear();

    foreach (const QSerialPortInfo &tLoopInfo, QSerialPortInfo::availablePorts())
    {
        ui->portsComboBox->addItem(tLoopInfo.portName());
    }
}

void MainWindow::onConnectClick()
{
    QString tPortName   = ui->portsComboBox->currentText();
    QIODevice::OpenMode tOpenMode = QIODevice::ReadWrite;
    int tBaudRate       = ui->baudRateComboBox->currentText().toInt();
    int tDataBits       = ui->dataBitsComboBox->currentText().toInt();
    int tParity         = ui->parityComboBox->currentText().toInt();
    int tStopBits       = ui->stopBitsComboBox->currentText().toInt();
    int tFlowControl    = 0;

    if (ui->flowControlComboBox->currentText() == "Hardware")
        tFlowControl = 1;
    else if (ui->flowControlComboBox->currentText() == "Software")
        tFlowControl = 2;

    if (ui->openModeComboBox->currentText() == "NotOpen")
        tOpenMode = QIODevice::NotOpen;
    else if (ui->openModeComboBox->currentText() == "ReadOnly")
        tOpenMode = QIODevice::ReadOnly;
    else if (ui->openModeComboBox->currentText() == "WriteOnly")
        tOpenMode = QIODevice::WriteOnly;
    else if (ui->openModeComboBox->currentText() == "ReadWrite")
        tOpenMode = QIODevice::ReadWrite;
    else if (ui->openModeComboBox->currentText() == "Append")
        tOpenMode = QIODevice::Append;
    else if (ui->openModeComboBox->currentText() == "Truncate")
        tOpenMode = QIODevice::Truncate;
    else if (ui->openModeComboBox->currentText() == "Text")
        tOpenMode = QIODevice::Text;
    else if (ui->openModeComboBox->currentText() == "Unbuffered")
        tOpenMode = QIODevice::Unbuffered;

    m_serialPort.setPortName(tPortName);
    m_serialPort.setBaudRate(tBaudRate);
    m_serialPort.setDataBits(QSerialPort::DataBits(tDataBits));
    m_serialPort.setParity(QSerialPort::Parity(tParity));
    m_serialPort.setStopBits(QSerialPort::StopBits(tStopBits));
    m_serialPort.setFlowControl(QSerialPort::FlowControl(tFlowControl));

    if (!m_serialPort.isOpen())
    {
        if (m_serialPort.open(tOpenMode))
        {
            m_serialPort.clearError();
            ui->textEdit->append(tPortName + " now open!");
        }
        else
        {
            ui->textEdit->append(tPortName + " failed to open!");
        }
    }
    else
    {
        ui->textEdit->append(tPortName + " is already open!");
    }

    ui->isOpenRadioButton->setChecked(m_serialPort.isOpen());
}

void MainWindow::onReadData()
{
    QString tData = m_serialPort.readAll();
    ui->textEdit->append("onReadData: " + tData);
}

void MainWindow::onError(QSerialPort::SerialPortError pError)
{
    QString tError = "";

    if (pError == QSerialPort::NoError)
        tError = "No Error";
    else if (pError == QSerialPort::DeviceNotFoundError)
        tError = "Device Not Found";
    else if (pError == QSerialPort::PermissionError)
        tError = "Permission Error";
    else if (pError == QSerialPort::OpenError)
        tError = "Open Error";
    else if (pError == QSerialPort::NotOpenError)
        tError = "Not Open Error";
    else if (pError == QSerialPort::ParityError)
        tError = "Pairity Error";
    else if (pError == QSerialPort::FramingError)
        tError = "Framing Error";
    else if (pError == QSerialPort::BreakConditionError)
        tError = "Break Condition Error";
    else if (pError == QSerialPort::WriteError)
        tError = "Write Error";
    else if (pError == QSerialPort::ReadError)
        tError = "Read Error";
    else if (pError == QSerialPort::ResourceError)
        tError = "Resource Error";
    else if (pError == QSerialPort::UnsupportedOperationError)
        tError = "Unsupported Operation Error";
    else if (pError == QSerialPort::TimeoutError)
        tError = "Timeout Error";
    else if (pError == QSerialPort::UnknownError)
        tError = "Unknown Error";

    ui->textEdit->append("Error: " + tError);

}

void MainWindow::onCloseClick()
{
    if (m_serialPort.isOpen())
        m_serialPort.close();

    ui->isOpenRadioButton->setChecked(m_serialPort.isOpen());
}

void MainWindow::onSendClick()
{
    int tRetval = 0;
    QString pCommand = ui->sendLineEdit->text();
    pCommand = pCommand.trimmed();
    pCommand += "\n";

    tRetval = m_serialPort.write(pCommand.toUtf8().constData());
    ui->textEdit->append("Command Sent[" + QString::number(tRetval) + "]: " + pCommand);
}

void MainWindow::onTextChanged(QString pString)
{
    disconnect(ui->sendLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));

    ui->sendLineEdit->setText(ui->sendLineEdit->text().toUpper());
    ui->sendLineEdit->setCursorPosition(ui->sendLineEdit->text().length());

    connect(ui->sendLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
}
