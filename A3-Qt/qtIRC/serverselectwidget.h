#ifndef SERVERSELECTWIDGET_H
#define SERVERSELECTWIDGET_H

#include <QWidget>
#include "irc.h"



namespace Ui {
class ServerSelectWidget;
}

class ServerSelectWidget : public QWidget
{
    Q_OBJECT

//signals:
//    void openConnection();

public:
    explicit ServerSelectWidget(QWidget *parent = 0, IRC *irc = nullptr);
    ~ServerSelectWidget();

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();


    // connect to serverselect signal




private:
    Ui::ServerSelectWidget *ui;
    IRC *_irc;
};

#endif // SERVERSELECTWIDGET_H
