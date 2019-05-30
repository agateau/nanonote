#include "MainWindow.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QLocale>
#include <QTranslator>

#include <singleapplication.h>

#include "Config.h"

static void loadTranslations(QObject *parent)
{
    // Search in current path first, to give translators an easy way to test
    // their translations
    QStringList searchDirs = { QDir::currentPath(), ":/translations" };
    auto translator = new QTranslator(parent);
    QLocale locale;
    for (const auto &dir : searchDirs) {
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
    Q_INIT_RESOURCE(translations);
    app.setOrganizationName("agateau.com");
    app.setApplicationName("nanonote");
    app.setApplicationVersion(NANONOTE_VERSION);
    app.setWindowIcon(QIcon(":/icons/appicon.svg"));
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    loadTranslations(&app);

    MainWindow window;

    QObject::connect(&app, &SingleApplication::instanceStarted, [&window] {
        window.raise();
        window.activateWindow();
    });

    window.show();

    return app.exec();
}
