#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serverconnectiondialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new Client(this);
    openServerConnectionDialog();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openServerConnectionDialog()
{
    ServerConnectionDialog serverConnectionDialog;

    connect(serverConnectionDialog, )

    serverConnectionDialog.exec();
}

// https://doc.qt.io/archives/3.3/clientserver-example.html

void MainWindow::on_chatPushButton_clicked()
{
    sendMessage();
}

void MainWindow::on_messageLineEdit_returnPressed()
{
    sendMessage();
}

/*
 * Sends Messages to the chatPlainTextEdit widget
 */
void MainWindow::sendMessage()
{
    QString chatMessage = ui->messageLineEdit->text().trimmed();
    if (!chatMessage.isEmpty()) {
        ui->chatPlainTextEdit->appendPlainText(chatMessage);
    }
    ui->messageLineEdit->clear();
    ui->messageLineEdit->setFocus();
}

void MainWindow::displayMessage(QString message)
{
    ui->chatPlainTextEdit->appendPlainText(message);
}
