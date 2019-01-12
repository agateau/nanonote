#ifndef INDENTEXTENSION_H
#define INDENTEXTENSION_H

#include "TextEdit.h"

#include <functional>

class IndentExtension : public TextEditExtension
{
public:
    explicit IndentExtension(TextEdit *textEdit);

    void aboutToShowContextMenu(QMenu *menu, const QPoint &pos) override;
    bool keyPress(QKeyEvent *event) override;

private:
    using ProcessSelectionCallback = std::function<void(QTextCursor&)>;
    bool canRemoveIndentation() const;
    void insertIndentation();
    void removeIndentation();
    void insertIndentedLine();
    void processSelection(ProcessSelectionCallback callback);
    void onTabPressed();

    QAction *mIndentAction;
    QAction *mUnindentAction;
};

#endif // INDENTEXTENSION_H
