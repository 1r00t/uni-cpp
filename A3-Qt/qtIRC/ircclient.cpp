#include "ircclient.h"
#include "ui_ircclient.h"
#include <QTextEdit>


IrcClient::IrcClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IrcClient),
    _irc(new IRC(this)) // IRC instanz erstellen

{
    ui->setupUi(this);

    connect(_irc, SIGNAL(messageReceived(IRC::Command)), this, SLOT(displayMessage(IRC::Command)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabSwitched(int)));

    TabPage *server_tab = new TabPage;
    _tabs.insert("Server", server_tab);

    ui->tabWidget->addTab(server_tab, "Server");
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

}

IrcClient::~IrcClient()
{
    delete ui;
}

void IrcClient::createTab(const QString &name)
{
    if(!_tabs.contains(name))
    {
        TabPage *tab = new TabPage;
        _tabs.insert(name, tab);
        ui->tabWidget->addTab(tab, name);
    }
}

void IrcClient::removeTab(const QString &name)
{
    if(_tabs.contains(name))
    {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(_tabs.value(name)));
        _tabs.remove(name);
    }
}

void IrcClient::on_tabSwitched(int index)
{
    QString channel = ui->tabWidget->tabText(index);
    updateAndShowUserList(channel);
}

void IrcClient::updateAndShowUserList(QString channel)
{
    // update user list per channel
    if(!_users.contains(channel))
    {
        ui->userList->clear();
        return;
    }
    QStringList users = _users[channel];

    ui->userList->clear();
    ui->userList->addItems(users);
}

void IrcClient::on_actionConnect_triggered()
{
    serverSelectWidget = new ServerSelectWidget(0, _irc);
    serverSelectWidget->show();

}

void IrcClient::displayMessage(const IRC::Command &command)
{
    QString outputText;
    TabPage *tab;


    // :rajaniemi.freenode.net 470 one_roOt #linux ##linux :Forwarding to another channel
    // switch channel
    if(command.command == "470") // forward
    {
        QString oldChanName = command.parameters.at(1);
        QString newChanName = command.parameters.at(2);
        tab = _tabs.value(oldChanName);
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(tab), newChanName);
        createTab(newChanName);
        removeTab(oldChanName);
    }

    // :rajaniemi.freenode.net 353 one_roOt * ##linux :feigned banisterfiend b1101 ALowther_ Guest52012
    // name list
    if(command.command == "353")
    {
        QStringList users = command.trailing.split(" ", QString::SkipEmptyParts);
        QString channel = command.parameters.at(2);
        if(!_users.contains(channel))
        {
            _users.insert(channel, users);
        } else {
            for(auto user: users)
            {
                if(!_users.value(channel).contains(user))
                {
                    _users[channel].append(user);
                }
            }
        }
        qSort(_users[channel]);
        updateAndShowUserList(channel);
    }

    // ":alpha!~alpha@97-127-17-117.mpls.qwest.net JOIN ##linux"
    // user joins
    if(command.command == "JOIN")
    {
        QString channel = command.parameters.first();
        QString user = command.prefix.left(command.prefix.indexOf("!"));
        if(_users.contains(channel) && !_users[channel].contains(user))
        {
            _users[channel].append(user);
            qSort(_users[channel]);
            updateAndShowUserList(channel);
        }
    }

    // ":irwinz!~irwinz@107-138-25-186.lightspeed.austtx.sbcglobal.net QUIT :Changing host"
    // user quits
    if(command.command == "QUIT")
    {
        QString user = command.prefix.left(command.prefix.indexOf("!"));
        QStringList channels = _users.keys();
        for(auto channel: channels)
        {
            if(_users[channel].contains(user))
            {
                _users[channel].removeOne(user);
            }
        }
        updateAndShowUserList(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
    }

    // user parts
    // :user!bla@bla PART #chan :message
    if(command.command == "PART")
    {
        QString user = command.prefix.left(command.prefix.indexOf("!"));
        QString channel = command.parameters.first();
        if(_users.contains(channel) && _users[channel].contains(user))
        {
            _users[channel].removeOne(user);
            if(channel == ui->tabWidget->tabText(ui->tabWidget->currentIndex()))
            {
                updateAndShowUserList(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
            }
        }
    }

    // user changes nickname
    // ":p1n0ch3t!~root@45.56.151.86 NICK :Guest24324"


    if(command.command == "PRIVMSG")
    {
        QString sender = command.prefix.left(command.prefix.indexOf("!"));
        if(sender.isEmpty()) sender = _irc->getNickname();
        QString recipient = command.parameters.at(0);
        QString message = command.trailing;

        outputText = sender + ": " + message;

        if(recipient.startsWith("#"))
        {
            tab = _tabs.value(recipient);
        } else {
            createTab(sender); // evtl geht es hier kaputt wegen weiterleitung #linux -> ##linux
            tab = _tabs.value(sender);
        }


    } else { // not privmsg => send to Server tab.
        outputText = ":" + command.prefix + " " + command.parameters.join(" ") + " :" + command.trailing;
        tab = _tabs.value("Server");
    }

    tab->appendText(outputText);
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
                createTab(command.parameters.first());
            }
            _irc->sendCommand(command);
        } else {
            // send privmsg
            IRC::Command command;
            command.command = "PRIVMSG";
            command.parameters.append(currentTab);
            command.trailing = message;
            _irc->sendCommand(command);
            displayMessage(command);
        }
    }
    ui->chatLineEdit->clear();

}

void IrcClient::on_chatLineEdit_returnPressed()
{
    // same as send clicked
    on_sendButton_clicked();
}
