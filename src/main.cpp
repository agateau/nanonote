#include "MainWindow.h"

#include <QCoreApplication>
#include <QDebug>
#include <QIcon>
#include <QLocale>
#include <QTranslator>

#include <singleapplication.h>

#include "Config.h"

static void loadTranslations(QObject *parent)
{
    auto appDir = QCoreApplication::applicationDirPath();
    auto translator = new QTranslator(parent);
    QLocale locale;
    for (const auto &dir : {appDir, }) {
        if (translator->load(locale, QCoreApplication::applicationName(), "_", dir)) {
            QCoreApplication::installTranslator(translator);
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv);
    Q_INIT_RESOURCE(nanonote);
    app.setOrganizationName("agateau.com");
    app.setApplicationName("nanonote");
    app.setApplicationVersion(NANONOTE_VERSION);
    app.setWindowIcon(QIcon(":/icons/appicon.svg"));

    loadTranslations(&app);

    MainWindow window;

    QObject::connect(&app, &SingleApplication::instanceStarted, [&window] {
        window.raise();
        window.activateWindow();
    });

    window.show();

    return app.exec();
}
