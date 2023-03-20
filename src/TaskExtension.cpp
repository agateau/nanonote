#include "TaskExtension.h"

#include <QMenu>
#include <QRegularExpression>

#include "SyntaxHighlighter.h"

static const char INSERT_TASK_REGEX[] = "^(\\s*)([-\\*])?(\\s)?(\\[[x ]\\])?";

static int getTaskCheckmarkPosUnderCursor(const QTextCursor& cursor) {
    return SyntaxHighlighter::getTaskCheckmarkPosAt(cursor.block().text(),
                                                    cursor.positionInBlock());
}

static void toggleTask(QTextCursor* cursor, int pos) {
    // First insert the new character after the old one, then delete the old character.
    // This prevents the main cursor from moving as a side effect from the editing.
    cursor->beginEditBlock();
    cursor->setPosition(pos + cursor->block().position());
    cursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    QString text = cursor->selectedText();
    cursor->movePosition(QTextCursor::Right);
    if (text == "x") {
        cursor->insertText(" ");
    } else {
        cursor->insertText("x");
    }
    cursor->movePosition(QTextCursor::Left);
    cursor->deletePreviousChar();
    cursor->endEditBlock();
}

TaskExtension::TaskExtension(TextEdit* textEdit)
        : TextEditExtension(textEdit), mTaskAction(std::make_unique<QAction>()) {
    mTaskAction->setText(tr("Insert/toggle task"));
    QList<QKeySequence> shortcuts = {Qt::CTRL | Qt::Key_Return, Qt::CTRL | Qt::Key_Enter};
    mTaskAction->setShortcuts(shortcuts);
    connect(mTaskAction.get(), &QAction::triggered, this, &TaskExtension::insertOrToggleTask);
    mTextEdit->addAction(mTaskAction.get());
}

void TaskExtension::aboutToShowEditContextMenu(QMenu* menu, const QPoint& /*pos*/) {
    menu->addAction(mTaskAction.get());
    menu->addSeparator();
}

bool TaskExtension::mouseRelease(QMouseEvent* event) {
    if (event->modifiers() == Qt::CTRL) {
        toggleTaskUnderCursor();
    }
    return false;
}

void TaskExtension::insertOrToggleTask() {
    auto cursor = mTextEdit->textCursor();
    QString text = cursor.block().text();

    QRegularExpression expression(INSERT_TASK_REGEX);
    QRegularExpressionMatch match = expression.match(text);
    if (!match.hasMatch()) {
        // Should not happen
        return;
    } else if (!match.captured(3).isEmpty() && !match.captured(4).isEmpty()) {
        // Line already contains a task, toggle completion
        toggleTask(&cursor, match.capturedStart(4) + 1);
        return;
    } else if (match.captured(2).isEmpty()) {
        // Not in list
        cursor.beginEditBlock();
        cursor.setPosition(cursor.block().position() + match.capturedEnd(1));
        cursor.insertText("- [ ] ");
        cursor.endEditBlock();
    } else {
        // In list, only add checkbox
        bool trailingSpace = !match.captured(3).isEmpty();
        cursor.beginEditBlock();
        cursor.setPosition(cursor.block().position() + match.capturedEnd(2));
        cursor.insertText(trailingSpace ? " [ ]" : " [ ] ");
        cursor.endEditBlock();
    }
}

void TaskExtension::toggleTaskUnderCursor() {
    auto cursor = mTextEdit->textCursor();
    int pos = getTaskCheckmarkPosUnderCursor(cursor);
    if (pos == -1) {
        return;
    }
    toggleTask(&cursor, pos);
}
