#include "tabpage.h"
#include "ui_tabpage.h"

TabPage::TabPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabPage)
{
    ui->setupUi(this);
    ui->textEdit->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse);
}

TabPage::~TabPage()
{
    delete ui;
}

void TabPage::appendText(QString text)
{
    ui->textEdit->append(text);
}
