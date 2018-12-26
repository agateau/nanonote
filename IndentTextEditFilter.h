#ifndef INDENTTEXTEDITFILTER_H
#define INDENTTEXTEDITFILTER_H

#include "TextEdit.h"

class IndentTextEditFilter : public TextEditFilter
{
public:
    explicit IndentTextEditFilter(TextEdit *textEdit);

    bool keyPress(QKeyEvent *event) override;

private:
    bool canRemoveIndentation() const;
    void insertIndentation();
    void removeIndentation();
    void insertIndentedLine();
};

#endif // INDENTTEXTEDITFILTER_H
