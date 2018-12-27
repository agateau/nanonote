#ifndef LINKEXTENSION_H
#define LINKEXTENSION_H

#include "TextEdit.h"

class LinkExtension : public TextEditExtension {
public:
    explicit LinkExtension(TextEdit *textEdit);

    bool keyPress(QKeyEvent *event) override;

    bool keyRelease(QKeyEvent *event) override;

    bool mouseRelease(QMouseEvent *event) override;

private:
    void openLinkUnderCursor();
};

#endif // LINKEXTENSION_H
