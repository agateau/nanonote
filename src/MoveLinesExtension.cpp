#include "MoveLinesExtension.h"

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QTextBlock>

MoveLinesExtension::MoveLinesExtension(TextEdit* textEdit)
        : TextEditExtension(textEdit)
        , mMoveUpAction(std::make_unique<QAction>())
        , mMoveDownAction(std::make_unique<QAction>()) {
    mMoveUpAction->setText(tr("Move selected lines up"));
    mMoveUpAction->setShortcut(Qt::SHIFT | Qt::ALT | Qt::Key_Up);
    connect(mMoveUpAction.get(), &QAction::triggered, this, &MoveLinesExtension::moveUp);
    mTextEdit->addAction(mMoveUpAction.get());

    mMoveDownAction->setText(tr("Move selected lines down"));
    mMoveDownAction->setShortcut(Qt::SHIFT | Qt::ALT | Qt::Key_Down);
    connect(mMoveDownAction.get(), &QAction::triggered, this, &MoveLinesExtension::moveDown);
    mTextEdit->addAction(mMoveDownAction.get());
}

MoveLinesExtension::~MoveLinesExtension() {
}

void MoveLinesExtension::aboutToShowEditContextMenu(QMenu* menu, const QPoint& /*pos*/) {
    menu->addAction(mMoveUpAction.get());
    menu->addAction(mMoveDownAction.get());
}

void MoveLinesExtension::moveUp() {
    moveSelectedLines(-1);
}

void MoveLinesExtension::moveDown() {
    moveSelectedLines(1);
}

/**
 * Add a final \\n if needed. Returns true if it added one.
 */
static bool addFinalNewLine(TextEdit* textEdit, QTextCursor* cursor) {
    if (textEdit->document()->lastBlock().text().isEmpty()) {
        return false;
    }

    // The `cursor` from `moveSelectedLines()` must stay at the same position. A previous version of
    // this function created its own cursor using QTextEdit::textCursor(), but if the cursor is at
    // the very end of the document, then `moveSelectedLines()` cursor is moved when we insert the
    // \n. I assue this is because the cursor is considered to be *after* the insertion position
    // of the \n, so Qt maintains its position.
    // To avoid that, we save the cursor state manually before inserting the \n, and restore the
    // state before leaving this function.
    int oldStart = cursor->selectionStart();
    int oldEnd = cursor->selectionEnd();
    if (oldStart == cursor->position()) {
        std::swap(oldStart, oldEnd);
    }

    cursor->movePosition(QTextCursor::End);
    cursor->insertBlock();

    cursor->setPosition(oldStart);
    cursor->setPosition(oldEnd, QTextCursor::KeepAnchor);
    return true;
}

static void removeFinalNewLine(QTextCursor* editCursor) {
    editCursor->movePosition(QTextCursor::End);
    editCursor->deletePreviousChar();
}

void MoveLinesExtension::moveSelectedLines(int delta) {
    auto doc = mTextEdit->document();
    QTextCursor cursor = mTextEdit->textCursor();

    cursor.beginEditBlock();

    // To avoid dealing with the special-case of the last line not ending with
    // a \n, we add one if there is none and remove it at the end
    bool addedFinalNewLine = addFinalNewLine(mTextEdit, &cursor);

    // Find start and end of lines to move
    QTextBlock startBlock, endBlock;
    if (cursor.hasSelection()) {
        int end = cursor.selectionEnd();
        startBlock = doc->findBlock(cursor.selectionStart());
        endBlock = doc->findBlock(end);
        // If the end is not at the start of the block, select this block too
        if (end - endBlock.position() > 0) {
            endBlock = endBlock.next();
        }
    } else {
        startBlock = cursor.block();
        endBlock = startBlock.next();
    }
    auto startInsideBlock = cursor.selectionStart() - startBlock.position();
    auto endInsideBlock = cursor.selectionEnd() - startBlock.position();
    bool cursorAtStart = cursor.position() == cursor.selectionStart();
    if (cursorAtStart) {
        std::swap(startInsideBlock, endInsideBlock);
    }

    // Cut the lines to move
    cursor.setPosition(startBlock.position());
    cursor.setPosition(endBlock.position(), QTextCursor::KeepAnchor);
    auto textToMove = cursor.selectedText();
    cursor.removeSelectedText();

    // Move the cursor to the right position and paste the lines
    cursor.movePosition(delta < 0 ? QTextCursor::PreviousBlock : QTextCursor::NextBlock);
    int position = cursor.position();
    cursor.insertText(textToMove);

    // Restore selection
    cursor.setPosition(position + startInsideBlock);
    cursor.setPosition(position + endInsideBlock, QTextCursor::KeepAnchor);
    mTextEdit->setTextCursor(cursor);

    if (addedFinalNewLine) {
        removeFinalNewLine(&cursor);
    }

    cursor.endEditBlock();
}
