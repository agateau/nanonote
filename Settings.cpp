#include "Settings.h"

#include <QFontDatabase>
#include <QSettings>

Settings::Settings(QObject *parent) : BaseSettings(parent)
{
}

void Settings::load()
{
    QSettings settings;
    QRect geometry = settings.value("geometry").toRect();
    if (geometry.isValid()) {
        setGeometry(geometry);
    }

    QVariant fontVariant = settings.value("font");
    if (fontVariant.canConvert<QFont>()) {
        setFont(fontVariant.value<QFont>());
    } else {
        setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    }

    setAlwaysOnTop(settings.value("alwaysOnTop").toBool());
}

void Settings::save()
{
    QSettings settings;
    settings.setValue("geometry", geometry());
    settings.setValue("font", font());
    settings.setValue("alwaysOnTop", alwaysOnTop());
}
