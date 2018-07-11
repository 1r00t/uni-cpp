#ifndef SERVERCONNECTIONDIALOG_H
#define SERVERCONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ServerConnectionDialog;
}

class ServerConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerConnectionDialog(QWidget *parent = 0);
    ~ServerConnectionDialog();

signals:
    void connectToServer();

private slots:
    void on_okPushButton_clicked();

private:
    Ui::ServerConnectionDialog *ui;
};

#endif // SERVERCONNECTIONDIALOG_H
