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
#include "Resources.h"

static void loadTranslations(QObject* parent) {
    std::optional<QString> translationsDir = Resources::findDir("translations");
    if (!translationsDir.has_value()) {
        return;
    }
    auto translator = new QTranslator(parent);
    QLocale locale;
    if (translator->load(locale, APP_NAME, "_", translationsDir.value())) {
        QCoreApplication::installTranslator(translator);
    }
}

/**
 * Initialize QIcon so that QIcon::fromTheme() finds our icons on Windows and macOS
 */
static void initFallbackIcons() {
#if defined(Q_OS_WINDOWS) || defined(Q_OS_MACOS)
    // A theme name must be defined othewise QIcon::fromTheme won't look in fallbackSearchPaths
    QIcon::setThemeName(APP_NAME);
    QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << ":/icons");
#endif
}

int main(int argc, char* argv[]) {
    SingleApplication app(argc, argv);
    Q_INIT_RESOURCE(app);
    app.setOrganizationName(ORGANIZATION_NAME);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    auto iconName = QString(":/appicon/sc-apps-%1.svg").arg(APP_NAME);
    app.setWindowIcon(QIcon(iconName));
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
#ifdef Q_OS_MACOS
    app.setAttribute(Qt::AA_DontShowShortcutsInContextMenus, false);
    QGuiApplication::styleHints()->setShowShortcutsInContextMenus(true);
#endif

    initFallbackIcons();

    loadTranslations(&app);

    MainWindow window;

    QObject::connect(&app, &SingleApplication::instanceStarted, [&window] {
        window.raise();
        window.activateWindow();
    });

    window.show();

    return app.exec();
}
