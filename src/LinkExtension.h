#ifndef LINKEXTENSION_H
#define LINKEXTENSION_H

#include "TextEdit.h"

class LinkExtension : public TextEditExtension
{
    Q_OBJECT
public:
    explicit LinkExtension(TextEdit *textEdit);

    void aboutToShowContextMenu(QMenu *menu, const QPoint& pos) override;

    bool keyPress(QKeyEvent *event) override;

    bool keyRelease(QKeyEvent *event) override;

    bool mouseRelease(QMouseEvent *event) override;

private:
    void openLinkUnderCursor();
};

#endif // LINKEXTENSION_H
