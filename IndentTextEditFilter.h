#ifndef INDENTTEXTEDITFILTER_H
#define INDENTTEXTEDITFILTER_H

#include "TextEdit.h"

#include <functional>

class IndentTextEditFilter : public TextEditFilter
{
public:
    explicit IndentTextEditFilter(TextEdit *textEdit);

    bool keyPress(QKeyEvent *event) override;

private:
    using ProcessSelectionCallback = std::function<void(QTextCursor&)>;
    bool canRemoveIndentation() const;
    void insertIndentation();
    void removeIndentation();
    void insertIndentedLine();
    void processSelection(ProcessSelectionCallback callback);
};

#endif // INDENTTEXTEDITFILTER_H
