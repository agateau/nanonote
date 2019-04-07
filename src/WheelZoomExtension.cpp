#include "WheelZoomExtension.h"

#include <QDateTime>

static const int SCROLL_TIMEOUT = 1000; // in milliseconds

WheelZoomExtension::WheelZoomExtension(TextEdit *textEdit)
    : TextEditExtension(textEdit)
{}

bool WheelZoomExtension::wheel(QWheelEvent *event)
{
    if (event->modifiers() != Qt::CTRL) {
        return false;
    }

    int delta = event->angleDelta().y();
    if (delta == 0) {
        return false;
    }

    qint64 time = QDateTime::currentMSecsSinceEpoch();
    if (time - mLastUpdate > SCROLL_TIMEOUT) {
        mPartialDelta = 0;
    }

    mPartialDelta += delta;
    int steps = mPartialDelta / QWheelEvent::DefaultDeltasPerStep;
    if (steps != 0) {
        emit adjustFontSize(steps);
        mPartialDelta -= steps * QWheelEvent::DefaultDeltasPerStep;
    }
    mLastUpdate = time;
    return true;
}

