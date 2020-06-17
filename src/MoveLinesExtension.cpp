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

void MoveLinesExtension::moveSelectedLines(int delta) {
    auto addFinalNewLine = [this](QTextCursor* editCursor) -> bool {
        if (mTextEdit->document()->lastBlock().text() == "") {
            return false;
        }
        editCursor->movePosition(QTextCursor::End);
        editCursor->insertBlock();
        return true;
    };
    auto removeFinalNewLine = [](QTextCursor* editCursor) {
        editCursor->movePosition(QTextCursor::End);
        editCursor->deletePreviousChar();
    };

    auto cursor = mTextEdit->textCursor();
    auto doc = mTextEdit->document();
    QTextCursor editCursor(doc);

    editCursor.beginEditBlock();

    // To avoid dealing with the special-case of the last line not ending with
    // a \n, we add one if there is none and remove it at the end
    bool addedFinalNewLine = addFinalNewLine(&editCursor);

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
    auto relativeStart = cursor.selectionStart() - startBlock.position();
    auto relativeEnd = cursor.selectionEnd() - startBlock.position();
    bool cursorAtStart = cursor.position() == cursor.selectionStart();
    if (cursorAtStart) {
        std::swap(relativeStart, relativeEnd);
    }

    // Cut the lines to move
    editCursor.setPosition(startBlock.position());
    editCursor.setPosition(endBlock.position(), QTextCursor::KeepAnchor);
    auto textToMove = editCursor.selectedText();
    editCursor.removeSelectedText();

    // Move and paste
    editCursor.movePosition(delta > 0 ? QTextCursor::Down : QTextCursor::Up);
    int position = editCursor.position();
    editCursor.insertText(textToMove);

    // Restore selection
    cursor.setPosition(position + relativeStart);
    cursor.setPosition(position + relativeEnd, QTextCursor::KeepAnchor);
    mTextEdit->setTextCursor(cursor);

    if (addedFinalNewLine) {
        removeFinalNewLine(&editCursor);
    }

    editCursor.endEditBlock();
}
