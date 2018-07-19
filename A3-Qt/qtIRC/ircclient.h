#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QAbstractListModel>
#include <QListView>
#include <QListWidget>
#include <QStandardItemModel>
#include <set>
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

    struct Channel
    {
        QString name;
        TabPage *tabPage;
        QStandardItemModel *userList;
        int userCount = 0;

        Channel(): userList(new QStandardItemModel()){}
    };

private slots:
    void handleMessage(const IRC::Command &command);
    void connectionOpen();

    void on_actionConnect_triggered();
    void on_sendButton_clicked();
    void on_chatLineEdit_returnPressed();
    void on_tabSwitched(int index);
    void on_actionDisconnect_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void showUserListContextMenu(const QPoint& pos);
    void whois();

private:
    Ui::IrcClient *ui;
    IRC *_irc;
    ServerSelectWidget *serverSelectWidget;

    std::vector<Channel> *_openChannels;

    int getChannelIndex(QString name);
    int getUserIndex(int channelIndex, QString name);

    void createTab(Channel channel);
    void removeTab(int index);

    QStandardItemModel *userListModel;
    std::vector<QStandardItem*> userList;

    //void IrcClient::showUserList(int index);
};

#endif // IRCCLIENT_H
