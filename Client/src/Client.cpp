#include "Client.h"

#include <QDebug>


/*
    Constructor

    Creates the TCP socket and connects
    Qt signals to our functions.
*/
Client::Client(QObject* parent)
    : QObject(parent)
{
    socket =
        new QTcpSocket(this);



    /*
        Called whenever data arrives
        from the server.
    */
    connect(
        socket,
        &QTcpSocket::readyRead,
        this,
        &Client::readData
    );



    /*
        Server connection succeeded.
    */
    connect(
        socket,
        &QTcpSocket::connected,
        this,
        &Client::connected
    );



    /*
        Server disconnected.
    */
    connect(
        socket,
        &QTcpSocket::disconnected,
        this,
        &Client::disconnected
    );
}






/*
    Opens a TCP connection
    to the server.
*/
void Client::connectToServer(
    const QString& address,
    quint16 port
)
{
    socket->connectToHost(
        address,
        port
    );
}






/*
    Sends a message to the server.

    Used for:

    LOGIN:Player

    and

    Chat messages
*/
void Client::sendMessage(
    const QString& message
)
{
    if(socket->state()
       == QAbstractSocket::ConnectedState)
    {
        socket->write(
            message.toUtf8()
        );


        socket->flush();
    }
}






/*
    Receives data from the server.

    IMPORTANT:

    We use readAll() instead of
    readLine() because TCP does not
    guarantee line endings.

    Example:

    Server sends:

    USERS:Alice,Bob,

    There may not be a "\n"
    character at the end.
*/
void Client::readData()
{
    QByteArray data =
        socket->readAll();



    if(data.isEmpty())
        return;



    QString message =
        QString::fromUtf8(
            data
        );



    qDebug()
        << "Received from server:"
        << message;



    emit messageReceived(
        message
    );
}