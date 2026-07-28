#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QString>


class Client : public QObject
{
    Q_OBJECT


public:

    explicit Client(QObject* parent = nullptr);


    void connectToServer(
        const QString& address,
        quint16 port
    );


    void sendMessage(
        const QString& message
    );


signals:

    void connected();

    void disconnected();

    void messageReceived(
        const QString& message
    );


private slots:

    void readData();


private:

    QTcpSocket* socket;
};
