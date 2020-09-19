#ifndef INDENTEXTENSION_H
#define INDENTEXTENSION_H

#include "TextEdit.h"

#include <functional>

class IndentExtension : public TextEditExtension {
    Q_OBJECT
public:
    explicit IndentExtension(TextEdit* textEdit);

    void aboutToShowEditContextMenu(QMenu* menu, const QPoint& pos) override;
    bool keyPress(QKeyEvent* event) override;

private:
    using ProcessSelectionCallback = std::function<void(QTextCursor&)>;
    bool canRemoveIndentation() const;
    bool isAtStartOfListLine() const;
    bool isAtEndOfLine() const;
    bool isIndentedLine() const;
    void insertIndentation();
    void removeIndentation();
    void insertIndentedLine();
    void processSelection(ProcessSelectionCallback callback);
    void onTabPressed();
    void onEnterPressed();

    QAction* mIndentAction;
    QAction* mUnindentAction;
};

#endif // INDENTEXTENSION_H
