#ifndef WHEELZOOMEXTENSION_H
#define WHEELZOOMEXTENSION_H

#include "TextEdit.h"

class WheelZoomExtension : public TextEditExtension
{
    Q_OBJECT
public:
    explicit WheelZoomExtension(TextEdit *textEdit);

    bool wheel(QWheelEvent *event) override;

signals:
    void adjustFontSize(int delta);

private:
    int mPartialDelta;
    qint64 mLastUpdate;
};

#endif // WHEELZOOMEXTENSION_H
