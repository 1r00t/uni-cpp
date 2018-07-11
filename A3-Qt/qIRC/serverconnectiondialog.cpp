#include "serverconnectiondialog.h"
#include "ui_serverconnectiondialog.h"
#include "client.h"

ServerConnectionDialog::ServerConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerConnectionDialog)
{
    ui->setupUi(this);
}

ServerConnectionDialog::~ServerConnectionDialog()
{
    delete ui;
}

void ServerConnectionDialog::on_okPushButton_clicked()
{
    emit connectToServer();
    this->close();

}