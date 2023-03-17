// This file has been generated with qpropgen, any changes made to it will be lost!
#include <BaseSettings.h>

BaseSettings::BaseSettings(QObject* parent)
    : QObject(parent) {
}


bool BaseSettings::alwaysOnTop() const {
    return mAlwaysOnTop;
}
void BaseSettings::setAlwaysOnTop(bool value) {
    
    if (mAlwaysOnTop == value) {
    
        return;
    }
    mAlwaysOnTop = value;
    alwaysOnTopChanged(value);
}

QFont BaseSettings::font() const {
    return mFont;
}
void BaseSettings::setFont(const QFont& value) {
    
    if (mFont == value) {
    
        return;
    }
    mFont = value;
    fontChanged(value);
}

QRect BaseSettings::geometry() const {
    return mGeometry;
}
void BaseSettings::setGeometry(const QRect& value) {
    
    if (mGeometry == value) {
    
        return;
    }
    mGeometry = value;
    geometryChanged(value);
}
