#include "TextEdit.h"

#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QTextBlock>

static const int INDENT_SIZE = 4;

static QString getIndentation(const QString &line)
{
    int idx;
    for (idx = 0; idx < line.length(); ++idx) {
        if (line[idx] != ' ') {
            break;
        }
    }
    return line.left(idx);
}

TextEdit::TextEdit(QWidget *parent)
    : QTextEdit(parent)
{
}

void TextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    menu->addSeparator();
    for (QAction *action : actions()) {
        menu->addAction(action);
    }
    menu->exec(event->globalPos());
    delete menu;
}

void TextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        insertIndentation();
        event->accept();
    } else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        insertIndentedLine();
        event->accept();
    } else if (event->key() == Qt::Key_Backspace && canRemoveIndentation()) {
        removeIndentation();
        event->accept();
    } else {
        QTextEdit::keyPressEvent(event);
    }
}

bool TextEdit::canRemoveIndentation() const
{
    int col = textCursor().columnNumber();
    if (col == 0) {
        return false;
    }
    QString line = textCursor().block().text();
    for (int i = col - 1; i >= 0; --i) {
        if (line.at(i) != ' ') {
            return false;
        }
    }
    return true;
}

void TextEdit::insertIndentation()
{
    insertPlainText(QString(INDENT_SIZE, ' '));
}

void TextEdit::removeIndentation()
{
    QTextCursor cursor = textCursor();
    int col = cursor.columnNumber();
    int delta = (col % INDENT_SIZE == 0) ? INDENT_SIZE : (col % INDENT_SIZE);
    cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, delta);
    cursor.removeSelectedText();
}

void TextEdit::insertIndentedLine()
{
    QString indentation = getIndentation(textCursor().block().text());
    insertPlainText('\n' + indentation);
    ensureCursorVisible();
}
