#include <QApplication>

#include "ConnectDialog.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    ConnectDialog dialog;

    dialog.show();


    return app.exec();
}
