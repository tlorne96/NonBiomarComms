#include "ConnectDialog.h"
#include "MainWindow.h"
#include "Client.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>



ConnectDialog::ConnectDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(
        "Connect to Server"
    );


    serverIP =
        new QLineEdit("127.0.0.1");


    port =
        new QLineEdit("10000");


    nickname =
        new QLineEdit("Player");


    connectButton =
        new QPushButton("Connect");
        
    client = new Client(this);


    connect(
        connectButton,
        &QPushButton::clicked,
        this,
        &ConnectDialog::connectClicked
    );



    auto layout =
        new QVBoxLayout;



    layout->addWidget(
        new QLabel("Server IP")
    );

    layout->addWidget(
        serverIP
    );


    layout->addWidget(
        new QLabel("Port")
    );

    layout->addWidget(
        port
    );


    layout->addWidget(
        new QLabel("Nickname")
    );

    layout->addWidget(
        nickname
    );


    layout->addWidget(
        connectButton
    );


    setLayout(layout);
}
void ConnectDialog::connectClicked()
{
    QString ip =
        serverIP->text();


    quint16 serverPort =
        port->text().toUShort();


    QString name =
        nickname->text();



    client->connectToServer(
        ip,
        serverPort
    );



    connect(
        client,
        &Client::connected,
        this,
        [this, name]()
        {
            client->sendMessage(
                "LOGIN:" + name + "\n"
            );


            window =
                new MainWindow(
                    client
                );


            window->show();


            this->hide();
        }
    );
}