#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFont>
#include <QObject>
#include <QRect>

class Settings : public QObject {
    Q_OBJECT
public:
    explicit Settings(QObject* parent = nullptr);
    static QString notePath();
    void load();
    void save();
    QFont defaultFont() const;

    // User-modifiable settings
    bool alwaysOnTop() const;
    void setAlwaysOnTop(bool value);

    QFont font() const;
    void setFont(const QFont& value);

    QRect geometry() const;
    void setGeometry(const QRect& value);

signals:
    void alwaysOnTopChanged(bool alwaysOnTop);
    void fontChanged(const QFont& font);
    void geometryChanged(const QRect& geometry);

private:
    bool mAlwaysOnTop = false;
    QFont mFont;
    QRect mGeometry;
};

#endif // SETTINGS_H
