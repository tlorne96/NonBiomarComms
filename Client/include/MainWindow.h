#pragma once

#include <QMainWindow>


class QListWidget;
class QTextEdit;
class QLineEdit;
class QPushButton;
class Client;


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:

    explicit MainWindow(
        Client* client,
        QWidget* parent = nullptr
    );


private slots:

    void sendMessage();

    void receiveMessage(
        const QString& message
    );


private:

    Client* client;


    QListWidget* users;

    QTextEdit* chat;

    QLineEdit* messageInput;

    QPushButton* sendButton;
};