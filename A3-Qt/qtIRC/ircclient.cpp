#include "ircclient.h"
#include "ui_ircclient.h"
#include <QTextEdit>


IrcClient::IrcClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IrcClient),
    _irc(new IRC(this)), // IRC instanz erstellen
    _openChannels(new std::vector<Channel>)

{
    ui->setupUi(this);

    userListModel = new QStandardItemModel();
    ui->userList->setModel(userListModel);

    // connect ui _irc
    connect(_irc, SIGNAL(messageReceived(IRC::Command)), this, SLOT(handleMessage(IRC::Command)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabSwitched(int)));
    connect(_irc, SIGNAL(connectionOpen()), this, SLOT(connectionOpen()));
    connect(ui->actionQuit, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));


    // create Server Channel
    TabPage *server_tab = new TabPage;
    Channel channel = Channel();
    channel.name = "Server";
    channel.tabPage = server_tab;
    _openChannels->push_back(channel);

    // set up tabs
    ui->tabWidget->addTab(server_tab, channel.name);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->tabBar()->tabButton(0 ,QTabBar::RightSide)->deleteLater();
    ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, 0);

    // set up context menu for user list
    ui->userList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->userList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showUserListContextMenu(QPoint)));

}

IrcClient::~IrcClient()
{
    delete ui;
}

void IrcClient::connectionOpen()
{
    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
}


void IrcClient::createTab(Channel channel)
{
    TabPage *tab = new TabPage;
    channel.tabPage = tab;
    _openChannels->push_back(channel);
    ui->tabWidget->addTab(tab, channel.name);

}

void IrcClient::removeTab(int index)
{
    _openChannels->erase(_openChannels->begin() + index);
    ui->tabWidget->removeTab(index);

}

void IrcClient::on_tabSwitched(int index)
{
    userListModel = _openChannels->at(index).userList;
    ui->userList->setModel(userListModel);
    // change model;
}


//void IrcClient::showUserList(int index)
//{

//    Channel channel = _openChannels->at(index);

//    auto users = channel.users;

//    ui->userList->clear();

//    for(int i = 0; i < users->size(); i++)
//    {
//        QString userName = *std::next(users->begin(), i);
//        ui->userList->addItem(users->QString);
//        ui->userList->addItem(users->at(i).name);
//    }
//}


void IrcClient::handleMessage(const IRC::Command &command)
{
    // :rajaniemi.freenode.net 470 one_roOt #linux ##linux :Forwarding to another channel
    // switch channel

    if(command.prefix.left(command.prefix.indexOf("!")) == _irc->getNickname())
    {
        qDebug() << "es geht mich etwas an!";
    } else if(command.command == "470") // forward
    {
        QString oldChanName = command.parameters.at(1);
        QString newChanName = command.parameters.at(2);

        for(int i = 0; i < _openChannels->size(); i++)
        {
            if(_openChannels->at(i).name == oldChanName)
            {
                Channel channel = Channel();
                channel.name = newChanName;

                removeTab(i);
                createTab(channel);
                break;
            }
        }
    }

    // :rajaniemi.freenode.net 353 one_roOt * ##linux :feigned banisterfiend b1101 ALowther_ Guest52012
    // name list
    else if(command.command == "353") // nickname list
    {
        QString channelName = command.parameters.at(2);
        QStringList userNames = command.trailing.split(" ", QString::SkipEmptyParts);
        int channelIndex = getChannelIndex(channelName);
        Channel channel = _openChannels->at(channelIndex);

        // add users
        for(QString userName: userNames)
        {
            userList.push_back(new QStandardItem(userName));
            channel.userList->appendRow(userList.back());
        }

        // show userlist if is curren index
//        if(channelIndex == ui->tabWidget->currentIndex())
//        {
//            showUserList(channelIndex);
//        }
    }

    // ":alpha!~alpha@97-127-17-117.mpls.qwest.net JOIN ##linux"
    // user joins
    else if(command.command == "JOIN")
    {
        QString channelName = command.parameters.first();
        QString userName = command.prefix.left(command.prefix.indexOf("!"));
        int channelIndex = getChannelIndex(channelName);
        Channel channel = _openChannels->at(channelIndex);

        userList.push_back(new QStandardItem(userName));
        channel.userList->appendRow(userList.back());
        channel.tabPage->appendText("<span style=\"color: lightgrey;\"><b>" + userName + "</b> joined</span>");

//        if(channelIndex == ui->tabWidget->currentIndex())
//        {
//            showUserList(channelIndex);
//        }
    }

    // ":irwinz!~irwinz@107-138-25-186.lightspeed.austtx.sbcglobal.net QUIT :Changing host"
    // user quits
    else if(command.command == "QUIT")
    {
        QString userName = command.prefix.left(command.prefix.indexOf("!"));


        for(int i = 0; i < _openChannels->size(); i++)
        {
            int userIndex = getUserIndex(i, userName);
            if(userIndex != -1)
            {
                userList.erase(userList.begin() + userIndex);
                _openChannels->at(i).userList->removeRow(userIndex);
                _openChannels->at(i).tabPage->appendText("<span style=\"color: lightgrey;\"><b>" + userName + "</b> quit (" + command.trailing + ")</span>");

//                if(i == ui->tabWidget->currentIndex())
//                {
//                    showUserList(i);
//                }
            }
        }
    }

    // :user!bla@bla PART #chan :message
    // user parts
    else if(command.command == "PART")
    {
        QString userName = command.prefix.left(command.prefix.indexOf("!"));
        QString channelName = command.parameters.first();
        int channelIndex = getChannelIndex(channelName);
        int userIndex = getUserIndex(channelIndex, userName);

        userList.erase(userList.begin() + userIndex);
        _openChannels->at(channelIndex).userList->removeRow(userIndex);
        _openChannels->at(channelIndex).tabPage->appendText("<span style=\"color: lightgrey;\"><b>" + userName + "</b> parted (" + command.trailing + ")</span>");

//        if(channelIndex == ui->tabWidget->currentIndex())
//        {
//            showUserList(channelIndex);
//        }
    }

    // ":p1n0ch3t!~root@45.56.151.86 NICK :Guest24324"
    // user changes nickname
    else if(command.command == "NICK")
    {
        QString oldUserName = command.prefix.left(command.prefix.indexOf("!"));
        QString newUserName = command.trailing;

        for(int i = 0; i < _openChannels->size(); i++)
        {
            int userIndex = getUserIndex(i, oldUserName);

            if(userIndex != -1)
            {
                // HIER IST DER FEHLER DASS &QStandardItem nicht persistent ist denke ich!

                userList.at(userIndex) = new QStandardItem(newUserName);
                _openChannels->at(i).userList->setItem(userIndex, userList.at(userIndex));
                _openChannels->at(i).tabPage->appendText("<span style=\"color: lightgrey;\"><b>" + oldUserName + "</b> changed name to <b>" + newUserName + "</b></span>");

//                if(i == ui->tabWidget->currentIndex())
//                {
//                    showUserList(i);
//                }
            }
        }
    }

    // WHOIS
    else if(command.command == "311") // real name
    {
        QString nickname = command.parameters.at(1);
        // QString username = command.parameters.at(1);
        // QString host = command.parameters.at(2);
        QString realname = command.trailing;
        Channel channel = _openChannels->at(ui->tabWidget->currentIndex());
        channel.tabPage->appendText("<span style=\"color: darkgrey;\">" + nickname + "'s real name is " + realname + "</span>");
    }

    else if(command.command == "319") // is in channels
    {
        QString nickname = command.parameters.at(1);
        QString inChannel = command.trailing;
        Channel channel = _openChannels->at(ui->tabWidget->currentIndex());
        channel.tabPage->appendText("<span style=\"color: darkgrey;\">" + nickname + " is in channel " + inChannel + "</span>");
    }

    else if(command.command == "312") // server info
    {
        QString nickname = command.parameters.at(1);
        QString server = command.parameters.last();
        QString serverInfo = command.trailing;
        Channel channel = _openChannels->at(ui->tabWidget->currentIndex());
        channel.tabPage->appendText("<span style=\"color: darkgrey;\">" + nickname + " is on server " + server + " - " + serverInfo + "</span>");
    }

    else if(command.command == "317") // idle
    {
        QString nickname = command.parameters.at(1);
        QString signOnTime = command.parameters.last();
        QString idleSince = command.parameters.at(2);
        Channel channel = _openChannels->at(ui->tabWidget->currentIndex());
        channel.tabPage->appendText("<span style=\"color: darkgrey;\">" + nickname + " is idle since " + idleSince + " seconds</span>");
    }

    else if(command.command == "PRIVMSG")
    {
        QString sender = command.prefix.left(command.prefix.indexOf("!"));
        if(sender.isEmpty()) sender = _irc->getNickname();
        QString recipient = command.parameters.at(0);
        QString message = command.trailing;

        QString outputText = "<b style=\"color: darkred;\">" + sender + "</b>: " + message;

        Channel channel;

        if(recipient.startsWith("#"))
        {
            channel = _openChannels->at(getChannelIndex(recipient));
        } else {
            if(getChannelIndex(sender) == -1)
            {
                channel = Channel();
                channel.name = sender;
                channel.userList->appendRow(&QStandardItem(sender));
                createTab(channel); // evtl geht es hier kaputt wegen weiterleitung #linux -> ##linux
            } else {
                channel = _openChannels->at(getChannelIndex(sender));
            }
        }
        channel.tabPage->appendText(outputText);
    }

    // send to Server tab.

    QString outputText = "";

    if(!command.prefix.isEmpty()) outputText.append(":" + command.prefix + " ");
    outputText.append(command.command);
    if(!command.parameters.isEmpty()) outputText.append(" " + command.parameters.join(" "));
    if(!command.trailing.isEmpty()) outputText.append(" :" + command.trailing);

    _openChannels->at(0).tabPage->appendText(outputText);
}

void IrcClient::on_sendButton_clicked()
{
    // get active tab and send to it
    QString currentTab = ui->tabWidget->tabBar()->tabText(ui->tabWidget->currentIndex());

    QString message = ui->chatLineEdit->text();

    if(!message.isEmpty())
    {
        if(message.startsWith("/"))
        {
            IRC::Command command = _irc->parseMessage(message.mid(1));
            if(message.startsWith("/join "))
            {
                Channel channel = Channel();
                channel.name = command.parameters.first();
                createTab(channel);
                ui->tabWidget->setCurrentIndex(getChannelIndex(channel.name));
            }
            _irc->sendCommand(command);
        } else {
            // send privmsg
            IRC::Command command;
            command.command = "PRIVMSG";
            command.parameters.append(currentTab);
            command.trailing = message;
            _irc->sendCommand(command);
            handleMessage(command);
        }
    }
    ui->chatLineEdit->clear();

}

void IrcClient::on_chatLineEdit_returnPressed()
{
    // same as send clicked
    on_sendButton_clicked();
}

void IrcClient::on_actionConnect_triggered()
{
    serverSelectWidget = new ServerSelectWidget(0, _irc);
    serverSelectWidget->show();
}

void IrcClient::on_actionDisconnect_triggered()
{
    _irc->closeConnection();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    for(int i = 1; i < _openChannels->size(); i++)
    {
        removeTab(i);
    }
}

void IrcClient::on_tabWidget_tabCloseRequested(int index)
{

    IRC::Command command;
    command.command = "PART";
    command.parameters.append(_openChannels->at(index).name);
    command.trailing = "bye!";
    _irc->sendCommand(command);
    removeTab(index);
}

void IrcClient::showUserListContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->userList->mapToGlobal(pos);

    QMenu menu;
    menu.addAction("whois", this, SLOT(whois()));
    menu.exec(globalPos);
}

void IrcClient::whois()
{
    QString item = ui->userList->currentIndex().data().toByteArray();
    IRC::Command command;
    command.command = "WHOIS";
    command.parameters.append(item);
    _irc->sendCommand(command);
}

int IrcClient::getChannelIndex(QString name)
{
    int index = -1;
    for(int i = 0; i < _openChannels->size(); i++)
    {
        if(_openChannels->at(i).name == name)
        {
            index = i;
            break;
        }
    }
    return index;
}

int IrcClient::getUserIndex(int channelIndex, QString name)
{
    int index = -1;
    for(int i = 0; i < _openChannels->at(channelIndex).userList->rowCount(); i++)
    {
        if(_openChannels->at(channelIndex).userList->item(i)->data() == name)
        {
            index = i;
            break;
        }
    }
    return index;
}
