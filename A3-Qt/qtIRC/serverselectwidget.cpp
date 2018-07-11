#include "serverselectwidget.h"
#include "ui_serverselectwidget.h"

ServerSelectWidget::ServerSelectWidget(QWidget *parent, IRC *irc) :
    QWidget(parent),
    ui(new Ui::ServerSelectWidget),
    _irc(irc)
{
    ui->setupUi(this);

    // prefill ui for now
    ui->serverEdit->insert("chat.freenode.com");
    ui->portEdit->insert("6667");
    ui->channelEdit->insert("#linux");
    ui->nicknameEdit->insert("one_roOt");
    ui->usernameEdit->insert("kroko");
    ui->realnameEdit->insert("Nicolai");
    ui->passwordEdit->insert("1wootroot");
}

ServerSelectWidget::~ServerSelectWidget()
{
    delete ui;
}


void ServerSelectWidget::on_cancelButton_clicked()
{
    // cancel click

    this->close();
}

void ServerSelectWidget::on_okButton_clicked()
{
    // OK click

    IRC::Connection connection;

    // validation??

    connection.host = ui->serverEdit->text();
    connection.port = ui->portEdit->text().toInt();
    connection.channel = ui->channelEdit->text();
    connection.nickname = ui->nicknameEdit->text();
    connection.username = ui->usernameEdit->text();
    connection.realname = ui->realnameEdit->text();
    connection.password = ui->passwordEdit->text();
    connection.identify = ui->identifyCheck->isChecked();

    _irc->openConnection(connection);

    this->close();
}
