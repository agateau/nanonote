#include "MainWindow.h"

#include <QIcon>

#include <singleapplication.h>

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv);
    app.setOrganizationName("agateau.com");
    app.setApplicationName("nanonote");
    app.setWindowIcon(QIcon(":/icons/appicon.svg"));

    MainWindow window;

    QObject::connect(&app, &SingleApplication::instanceStarted, [&window] {
        window.raise();
        window.activateWindow();
    });

    window.show();

    return app.exec();
}
