#ifndef IRC_H
#define IRC_H

#include <QObject>
#include <QTcpSocket>
#include <memory>



class IRC : public QObject
{
    Q_OBJECT
public:
    explicit IRC(QObject *parent = nullptr);

    ~IRC();

    struct Command
    {
        QString prefix = "";
        QString command;
        QStringList parameters;
        QString trailing = "";
    };

    struct Connection
    {
        QString host;
        qint16 port;
        QString channel;
        QString nickname;
        QString username;
        QString realname;
        QString password;
        bool identify;
    };

    Command parseMessage(const QString &message) const;
    QString getNickname();

signals:
    void messageReceived(const IRC::Command &command) const;
    void connectionOpen();


public slots:
    void openConnection(Connection connection); // nickname="roOt_1", username="root", realname="das geht dich nichts an!"
    void closeConnection() const;
    void sendCommand(const Command &command) const;
    void sendCommand(const QString &command) const;

    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *_tcpSocket = nullptr;
    Connection _connection = Connection();
    QByteArray *_messageBuffer = new QByteArray;

    void login() const;
};

#endif // IRC_H
