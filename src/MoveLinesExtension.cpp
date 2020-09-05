#include "MoveLinesExtension.h"

#include <QDebug>
#include <QTextBlock>

static constexpr Qt::KeyboardModifiers MODIFIERS = Qt::ShiftModifier | Qt::AltModifier;

MoveLinesExtension::MoveLinesExtension(TextEdit* textEdit) : TextEditExtension(textEdit) {
}

bool MoveLinesExtension::keyPress(QKeyEvent* event) {
    if (event->modifiers() == MODIFIERS) {
        if (event->key() == Qt::Key_Down) {
            moveSelectedLines(1);
            return true;
        } else if (event->key() == Qt::Key_Up) {
            moveSelectedLines(-1);
            return true;
        }
    }
    return false;
}

/**
 * Add a final \\n if needed. Returns true if it added one.
 */
static bool addFinalNewLine(TextEdit* textEdit) {
    if (textEdit->document()->lastBlock().text().isEmpty()) {
        return false;
    }
    // Use our own cursor to avoid altering the current one
    auto cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertBlock();
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
    bool addedFinalNewLine = addFinalNewLine(mTextEdit);

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
