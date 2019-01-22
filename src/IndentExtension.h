#ifndef INDENTEXTENSION_H
#define INDENTEXTENSION_H

#include "TextEdit.h"

#include <functional>

class IndentExtension : public TextEditExtension
{
    Q_OBJECT
public:
    explicit IndentExtension(TextEdit *textEdit);

    void aboutToShowContextMenu(QMenu *menu, const QPoint &pos) override;
    bool keyPress(QKeyEvent *event) override;

private:
    using ProcessSelectionCallback = std::function<void(QTextCursor&)>;
    bool canRemoveIndentation() const;
    bool isAtStartOfListLine() const;
    void insertIndentation();
    void removeIndentation();
    void insertIndentedLine();
    void processSelection(ProcessSelectionCallback callback);
    void onTabPressed();

    QAction *mIndentAction;
    QAction *mUnindentAction;
};

#endif // INDENTEXTENSION_H
