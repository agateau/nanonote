#include "IndentTextEditFilter.h"

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

IndentTextEditFilter::IndentTextEditFilter(TextEdit *textEdit)
    : TextEditFilter(textEdit)
{
}

bool IndentTextEditFilter::keyPress(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        insertIndentation();
        event->accept();
        return true;
    }
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        insertIndentedLine();
        event->accept();
        return true;
    }
    if (event->key() == Qt::Key_Backspace && canRemoveIndentation()) {
        removeIndentation();
        event->accept();
        return true;
    }
    return false;
}

bool IndentTextEditFilter::canRemoveIndentation() const
{
    auto cursor = mTextEdit->textCursor();
    int col = cursor.columnNumber();
    if (col == 0) {
        return false;
    }
    QString line = cursor.block().text();
    for (int i = col - 1; i >= 0; --i) {
        if (line.at(i) != ' ') {
            return false;
        }
    }
    return true;
}

void IndentTextEditFilter::insertIndentation()
{
    mTextEdit->insertPlainText(QString(INDENT_SIZE, ' '));
}

void IndentTextEditFilter::removeIndentation()
{
    auto cursor = mTextEdit->textCursor();
    int col = cursor.columnNumber();
    int delta = (col % INDENT_SIZE == 0) ? INDENT_SIZE : (col % INDENT_SIZE);
    cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, delta);
    cursor.removeSelectedText();
}

void IndentTextEditFilter::insertIndentedLine()
{
    QString indentation = getIndentation(mTextEdit->textCursor().block().text());
    mTextEdit->insertPlainText('\n' + indentation);
    mTextEdit->ensureCursorVisible();
}
