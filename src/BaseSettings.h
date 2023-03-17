// This file has been generated with qpropgen, any changes made to it will be lost!

#ifndef BASESETTINGS_H
#define BASESETTINGS_H

#include <QObject>
#include <QFont>
#include <QRect>

class BaseSettings : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool alwaysOnTop READ alwaysOnTop
            WRITE setAlwaysOnTop
            NOTIFY alwaysOnTopChanged
    )

    Q_PROPERTY(QFont font READ font
            WRITE setFont
            NOTIFY fontChanged
    )

    Q_PROPERTY(QRect geometry READ geometry
            WRITE setGeometry
            NOTIFY geometryChanged
    )

public:
    explicit BaseSettings(QObject* parent = nullptr);


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


protected:

    

    

    

private:
    bool mAlwaysOnTop = false;
    QFont mFont;
    QRect mGeometry;
};

#endif // BASESETTINGS_H
