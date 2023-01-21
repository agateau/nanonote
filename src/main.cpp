#include "MainWindow.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QLocale>
#include <QStyleHints>
#include <QTranslator>

#include <singleapplication.h>

#include "BuildConfig.h"

static void loadTranslations(QObject* parent) {
    // Search in current path first, to give translators an easy way to test
    // their translations
    QStringList searchDirs = {QDir::currentPath(), ":/translations"};
    auto translator = new QTranslator(parent);
    QLocale locale;
    for (const auto& dir : searchDirs) {
        if (translator->load(locale, "app", "_", dir)) {
            QCoreApplication::installTranslator(translator);
            return;
        }
    }
}

int main(int argc, char* argv[]) {
    SingleApplication app(argc, argv);
    Q_INIT_RESOURCE(app);
    Q_INIT_RESOURCE(translations);
    app.setOrganizationName(ORGANIZATION_NAME);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    auto iconName = QString(":/icons/sc-apps-%1.svg").arg(APP_NAME);
    app.setWindowIcon(QIcon(iconName));
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
#ifdef Q_OS_MACOS
    app.setAttribute(Qt::AA_DontShowShortcutsInContextMenus, false);
    QGuiApplication::styleHints()->setShowShortcutsInContextMenus(true);
#endif

    loadTranslations(&app);

    MainWindow window;

    QObject::connect(&app, &SingleApplication::instanceStarted, [&window] {
        window.raise();
        window.activateWindow();
    });

    window.show();

    return app.exec();
}
