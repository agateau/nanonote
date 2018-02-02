#include "MainWindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("agateau.com");
    app.setApplicationName("nanonote");
    app.setWindowIcon(QIcon(":/icons/appicon.svg"));
    MainWindow window;
    window.show();

    return app.exec();
}
