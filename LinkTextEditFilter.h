#ifndef LINKTEXTEDITFILTER_H
#define LINKTEXTEDITFILTER_H

#include "TextEdit.h"

class LinkTextEditFilter : public TextEditFilter {
public:
    explicit LinkTextEditFilter(TextEdit *textEdit);

    bool keyPress(QKeyEvent *event) override;

    bool keyRelease(QKeyEvent *event) override;

    bool mouseRelease(QMouseEvent *event) override;

private:
    void openLinkUnderCursor();
};



#endif // LINKTEXTEDITFILTER_H
