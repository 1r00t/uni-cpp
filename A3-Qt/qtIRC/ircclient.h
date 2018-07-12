#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <memory>
#include "serverselectwidget.h"
#include "irc.h"
#include "tabpage.h"

namespace Ui {
class IrcClient;
}

class IrcClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit IrcClient(QWidget *parent = 0);
    ~IrcClient();

private slots:
    void handleMessage(const IRC::Command &command);
    void connectionOpen();

    void on_actionConnect_triggered();
    void on_sendButton_clicked();
    void on_chatLineEdit_returnPressed();
    void on_tabSwitched(int index);
    void on_actionDisconnect_triggered();

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::IrcClient *ui;

    IRC *_irc;

    ServerSelectWidget *serverSelectWidget;

    QMap<QString, TabPage*> _tabs; // sollte auch besser auf den heap!
    QMap<QString, QStringList> _users; // sollte besser auf dem heap sein!

    void createTab(const QString &name);
    void removeTab(const QString &name);

    void IrcClient::updateAndShowUserList(QString channel);
};

#endif // IRCCLIENT_H
