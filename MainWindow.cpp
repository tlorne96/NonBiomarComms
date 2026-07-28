#include "MainWindow.h"

#include "Client.h"
#include <QRegularExpression>
#include <QStringList>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>



MainWindow::MainWindow(
    Client* client,
    QWidget* parent
)
    : QMainWindow(parent),
      client(client)
{

    setWindowTitle(
        "ChatAppTim"
    );


    resize(
        900,
        600
    );



    users =
        new QListWidget;



    chat =
        new QTextEdit;


    chat->setReadOnly(true);



    messageInput =
        new QLineEdit;



    sendButton =
        new QPushButton(
            "Send"
        );



    /*
        Bottom message area:

        [ message box ] [ Send ]
    */

    auto bottomLayout =
        new QHBoxLayout;


    bottomLayout->addWidget(
        messageInput
    );


    bottomLayout->addWidget(
        sendButton
    );



    /*
        Main layout:

        Users | Chat
    */

    auto mainLayout =
        new QHBoxLayout;



    mainLayout->addWidget(
        users
    );


    auto rightLayout =
        new QVBoxLayout;


    rightLayout->addWidget(
        chat
    );


    rightLayout->addLayout(
        bottomLayout
    );



    mainLayout->addLayout(
        rightLayout
    );



    auto container =
        new QWidget;


    container->setLayout(
        mainLayout
    );


    setCentralWidget(
        container
    );



    connect(
        sendButton,
        &QPushButton::clicked,
        this,
        &MainWindow::sendMessage
    );


    connect(
        messageInput,
        &QLineEdit::returnPressed,
        this,
        &MainWindow::sendMessage
    );



    connect(
        client,
        &Client::messageReceived,
        this,
        &MainWindow::receiveMessage
    );
}





void MainWindow::sendMessage()
{
    QString message =
        messageInput->text();


    if(message.isEmpty())
        return;



    client->sendMessage(
        message + "\n"
    );


    messageInput->clear();
}





void MainWindow::receiveMessage(
    const QString& message
)
{
    QStringList messages =
        message.split(
            QRegularExpression(
                "(?=SYSTEM:|USERS:)"
            ),
            Qt::SkipEmptyParts
        );


    for(QString msg : messages)
    {
        msg =
            msg.trimmed();



        if(msg.startsWith("USERS:"))
        {
            users->clear();


            QString userData =
                msg.mid(6);


            QStringList names =
                userData.split(
                    ",",
                    Qt::SkipEmptyParts
                );


            for(QString name : names)
            {
                users->addItem(
                    name
                );
            }


            continue;
        }



        chat->append(
            msg
        );
    }
}