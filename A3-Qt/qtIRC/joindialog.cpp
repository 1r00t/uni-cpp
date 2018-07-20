#include "joindialog.h"
#include "ui_joindialog.h"

JoinDialog::JoinDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JoinDialog)
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(on_buttonBox_accepted()));
}

JoinDialog::~JoinDialog()
{
    delete ui;
}

void JoinDialog::on_buttonBox_accepted()
{
    emit joinRequest(ui->lineEdit->text());
    close();
}

void JoinDialog::on_buttonBox_rejected()
{
    close();
}
