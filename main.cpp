#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("agateau.com");
    app.setApplicationName("nanonote");
    MainWindow window;
    window.show();

    return app.exec();
}
