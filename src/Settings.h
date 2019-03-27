#ifndef SETTINGS_H
#define SETTINGS_H

#include <BaseSettings.h>

class Settings : public BaseSettings
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    static QString notePath();
    void load();
    void save();
    QFont defaultFont() const;
};

#endif // SETTINGS_H
