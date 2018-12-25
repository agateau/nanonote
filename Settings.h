#ifndef SETTINGS_H
#define SETTINGS_H

#include <BaseSettings.h>

class Settings : public BaseSettings
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    void load();
    void save();
};

#endif // SETTINGS_H
