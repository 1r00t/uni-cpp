#ifndef CLIENT_H
#define CLIENT_H


#include <QObject>
#include <QDebug>

#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject* parent = 0); // singleton private constructor
    void openConnection(QString, QString);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

signals:
    void newMessage(QString message);

private:
    QTcpSocket* tcpSocket = nullptr;

};

#endif // CLIENT_H
