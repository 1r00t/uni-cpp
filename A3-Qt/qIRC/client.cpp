#include "client.h"
#include "mainwindow.h"



Client::Client(QObject* parent)
    : QObject(parent), tcpSocket(new QTcpSocket()){}


void Client::openConnection(QString host, QString port)
{
    connect(tcpSocket, SIGNAL(connected()),this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(tcpSocket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    tcpSocket->connectToHost(host, port.toInt());
    qDebug() << "connecting...";
    if (!tcpSocket->waitForConnected(5000))
    {
        qDebug() << "Error: " << tcpSocket->errorString();
    }
}


// SLOTS
void Client::connected()
{
    qDebug() << "Connected ...";
}

void Client::disconnected()
{
    qDebug() << "Disconnected ...";
}

void Client::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes Written ...";
}

void Client::readyRead()
{
    // qDebug() << tcpSocket->readAll();
    emit newMessage(tcpSocket->readAll());
}
