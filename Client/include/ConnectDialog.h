#pragma once
#include "MainWindow.h"
#include <QDialog>


class QLineEdit;
class QPushButton;
class Client;


class ConnectDialog : public QDialog
{
    Q_OBJECT


public:

    explicit ConnectDialog(QWidget* parent = nullptr);


private slots:

    void connectClicked();


private:

    QLineEdit* serverIP;
    QLineEdit* port;
    QLineEdit* nickname;

    QPushButton* connectButton;

    Client* client;
    MainWindow* window;
};