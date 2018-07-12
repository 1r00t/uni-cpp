#include "irc.h"

IRC::IRC(QObject *parent) : QObject(parent)
{

}

IRC::~IRC()
{
    if (_tcpSocket != nullptr && _tcpSocket->isOpen())
    {
         closeConnection();
    }
}

void IRC::openConnection(Connection connection)
{
    // open connection

    // create socket
    _tcpSocket = new QTcpSocket(this);

    _connection = connection;

    // connect socket signals
    connect(_tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(_tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(_tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    // open connection
    _tcpSocket->connectToHost(_connection.host, _connection.port);

    // wait for connection
    if(!_tcpSocket->waitForConnected(5000))
    {
        qDebug() << "Error: " << _tcpSocket->errorString();
    }
}

void IRC::closeConnection() const
{
    // close connection

    if(!_tcpSocket->isOpen()) return;

    //"QUIT :bye bye\r\n"

    Command command;
    command.command = "QUIT";
    command.parameters.append(":bye bye");

    sendCommand(command);

    _tcpSocket->close();
}

void IRC::sendCommand(const Command &command) const
{
    // send command

    if(_tcpSocket == nullptr || !_tcpSocket->isOpen()) return;

    QString commandString;

    if (!command.prefix.isEmpty())
    {
        commandString.append(":");
        commandString.append(command.prefix);
        commandString.append(" ");
    }

    commandString.append(command.command);

    if (!command.parameters.isEmpty())
    {
        commandString.append(" ");
        commandString.append(command.parameters.join(" "));
    }

    if (!command.trailing.isEmpty())
    {
        commandString.append(" :");
        commandString.append(command.trailing);
    }

    commandString.append("\r\n");

    qDebug() << "SENDING: " << commandString;

    _tcpSocket->write(commandString.toUtf8());
    _tcpSocket->flush();

}

void IRC::sendCommand(const QString &command) const
{
    // send command from QString

    if(_tcpSocket == nullptr || !_tcpSocket->isOpen()) return;

    Command com = parseMessage(command.mid(1));
    sendCommand(com);
}

IRC::Command IRC::parseMessage(const QString &message) const // build irc command
{
    // parse message

    Command command;

    QStringList splitMessage = message.split(" ", QString::SkipEmptyParts);

    // get prefix
    if (message.at(0) == ":")
    {
        QString prefix = splitMessage.takeAt(0);
        command.prefix = prefix.mid(1);
        qDebug() << "PREFIX: " << command.prefix;
    }

    // get command
    command.command = splitMessage.takeAt(0).toUpper();
    qDebug() << "COMMAND: " << command.command;

    // get parameters and trailing

    bool trailing = false;
    for (QString m: splitMessage)
    {
        if (m.startsWith(":") || trailing == true)
        {
            trailing = true;
            command.trailing.append(m + " ");
            continue;
        }
        command.parameters.append(m);
    }

    command.trailing = command.trailing.trimmed().mid(1);  // trim trailing space and :
    qDebug() << "PARAMETERS: " << command.parameters;
    qDebug() << "TRAILING: " << command.trailing;

    return command;
}

QString IRC::getNickname()
{
    if(!_connection.nickname.isEmpty())
        return _connection.nickname;
    return "you";
}

void IRC::login() const
{
    // login

    if(_tcpSocket == nullptr || !_tcpSocket->isOpen()) return;

    qDebug() << "logging in ...";
    Command nickCom;
    nickCom.command = "NICK";
    nickCom.parameters.append(_connection.nickname);
    sendCommand(nickCom);

    Command userCom;
    userCom.command = "USER";
    userCom.parameters.append(_connection.username);
    userCom.parameters.append("0");
    userCom.parameters.append("*");
    userCom.parameters.append(_connection.realname);
    sendCommand(userCom);

    if(_connection.identify)
    {
        Command identifyCom;
        identifyCom.command = "PRIVMSG";
        identifyCom.parameters.append("NickServ");
        identifyCom.trailing = "IDENTIFY " + _connection.nickname + " " + _connection.password;
        sendCommand(identifyCom);
    }
}


// slots
void IRC::connected()
{
    // SLOT connected
    qDebug() << "connected ...";

    emit connectionOpen();

    login();
}

void IRC::disconnected()
{
    // SLOT disconnected
    qDebug() << "disconnected ...";
}

void IRC::bytesWritten(qint64 bytes)
{
    // SLOT bytes written
    qDebug() << bytes << " bytes written ...";
}

void IRC::readyRead()
{
    // SLOT read new message


    qDebug() << "reading ...";

    *_messageBuffer->append(_tcpSocket->readAll());

    if(_messageBuffer->contains("\r\n"))
    {
        int bytes = _messageBuffer->indexOf("\r\n") + 2;
        QString message = _messageBuffer->left(bytes).trimmed();
        qDebug() << message;
        *_messageBuffer = _messageBuffer->mid(bytes);

        Command command = parseMessage(message);

        emit messageReceived(command);

        if (command.command == "PING")
        {
            Command com;
            com.command = "PONG";
            com.trailing = command.trailing;
            sendCommand(com);
        }
        readyRead();
    }
}
