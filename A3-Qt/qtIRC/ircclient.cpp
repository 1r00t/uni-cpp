#include "ircclient.h"
#include "ui_ircclient.h"
#include <QTextEdit>


IrcClient::IrcClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IrcClient),
    _irc(new IRC(this)) // IRC instanz erstellen

{
    ui->setupUi(this);

    // connect ui _irc
    connect(_irc, SIGNAL(messageReceived(IRC::Command)), this, SLOT(handleMessage(IRC::Command)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabSwitched(int)));
    connect(_irc, SIGNAL(connectionOpen()), this, SLOT(connectionOpen()));
    connect(ui->actionQuit, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));

    // create server tab
    TabPage *server_tab = new TabPage;
    _tabs.insert("Server", server_tab);

    // set up tabs
    ui->tabWidget->addTab(server_tab, "Server");
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->tabBar()->tabButton(0 ,QTabBar::RightSide)->deleteLater();
    ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, 0);
    ui->tabWidget->setStyleSheet("QTabBar::QTabButton { height: 100px; }");


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

void IrcClient::handleMessage(const IRC::Command &command)
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
    else if(command.command == "353") // nickname list
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
    else if(command.command == "JOIN")
    {
        QString channel = command.parameters.first();
        QString user = command.prefix.left(command.prefix.indexOf("!"));
        if(_users.contains(channel) && !_users[channel].contains(user))
        {
            _users[channel].append(user);
            qSort(_users[channel]);
            _tabs.value(channel)->appendText("<span style=\"color: lightgrey;\"><b>" + user + "</b> joined</span>");
            if(channel == ui->tabWidget->tabText(ui->tabWidget->currentIndex())) // if visible then update view.
            {
                updateAndShowUserList(channel);
            }
        }
    }

    // ":irwinz!~irwinz@107-138-25-186.lightspeed.austtx.sbcglobal.net QUIT :Changing host"
    // user quits
    else if(command.command == "QUIT")
    {
        QString user = command.prefix.left(command.prefix.indexOf("!"));
        QStringList channels = _users.keys();
        for(auto channel: channels)
        {
            if(_users[channel].contains(user))
            {
                _users[channel].removeOne(user);
            }
            // a user quits
            _tabs.value(channel)->appendText("<span style=\"color: lightgrey;\"><b>" + user + "</b> quit (" + command.trailing + ")</span>");

            if(channel == ui->tabWidget->tabText(ui->tabWidget->currentIndex())) // if visible then update view.
            {
                updateAndShowUserList(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
            }
        }
    }

    // :user!bla@bla PART #chan :message
    // user parts
    else if(command.command == "PART")
    {
        QString user = command.prefix.left(command.prefix.indexOf("!"));
        QString channel = command.parameters.first();
        if(_users.contains(channel) && _users[channel].contains(user))
        {
            _users[channel].removeOne(user);
            if(channel == ui->tabWidget->tabText(ui->tabWidget->currentIndex()))
            {
                _tabs.value(channel)->appendText("<span style=\"color: lightgrey;\"><b>" + user + "</b> parted (" + command.trailing + ")</span>");
                updateAndShowUserList(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
            }
        }
    }

    // ":p1n0ch3t!~root@45.56.151.86 NICK :Guest24324"
    // user changes nickname
    else if(command.command == "NICK")
    {
        QString user = command.prefix.left(command.prefix.indexOf("!"));
        QStringList channels = _users.keys();
        for(QString channel: channels)
        {
            if(_users[channel].contains(user))
            {
                _users[channel].replace(_users[channel].indexOf(user), command.trailing);
            }
            if(channel == ui->tabWidget->tabText(ui->tabWidget->currentIndex()))
            {
                qSort(_users[channel]);
                updateAndShowUserList(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
            }
        }
    }


    else if(command.command == "PRIVMSG")
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
        tab->appendText(outputText);
    }

    // send to Server tab.
    outputText = ":" + command.prefix + " " + command.parameters.join(" ") + " :" + command.trailing;
    tab = _tabs.value("Server");
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
                QString channel = command.parameters.first();
                createTab(channel);
                //ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(_tabs.value(channel)));
                ui->tabWidget->setCurrentWidget(_tabs.value(channel));
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
    for(auto tab: _tabs.values())
    {
        if(_tabs.key(tab) != "Server")
        {
            ui->tabWidget->removeTab(ui->tabWidget->indexOf(tab));
            _tabs.remove(_tabs.key(tab));
        }
    }
    _users.clear();
}

void IrcClient::on_tabWidget_tabCloseRequested(int index)
{
    QString channel = ui->tabWidget->tabBar()->tabText(index);
    removeTab(channel);

    IRC::Command command;
    command.command = "PART";
    command.parameters.append(channel);
    command.trailing = "bye!";
    _irc->sendCommand(command);
}
