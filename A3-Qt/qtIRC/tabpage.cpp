#include "tabpage.h"
#include "ui_tabpage.h"

TabPage::TabPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabPage)
{
    ui->setupUi(this);
}

TabPage::~TabPage()
{
    delete ui;
}

void TabPage::appendText(QString text)
{
    ui->textEdit->append(text);
}