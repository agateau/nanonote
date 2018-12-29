#include "IndentExtension.h"

#include <QDebug>
#include <QTextBlock>

static const int INDENT_SIZE = 4;

static int findBulletSize(const QStringRef &ref) {
    static QSet<QString> bullets = {"- ", "* ", "> "};
    for (auto bullet : bullets) {
        if (ref.startsWith(bullet)) {
            return bullet.length();
        }
    }
    return 0;
}

static QString findCommonPrefix(const QString &line)
{
    int idx;
    for (idx = 0; idx < line.length(); ++idx) {
        if (line[idx] != ' ') {
            break;
        }
    }
    idx += findBulletSize(line.midRef(idx));
    return line.left(idx);
}

IndentExtension::IndentExtension(TextEdit *textEdit)
    : TextEditExtension(textEdit)
{
}

static void indent(QTextCursor &cursor)
{
    static QString spaces = QString(INDENT_SIZE, ' ');
    cursor.insertText(spaces);
}

static void unindent(QTextCursor &cursor)
{
    const auto text = cursor.block().text();
    for (int idx = 0; idx < std::min(INDENT_SIZE, text.size()) && text.at(idx) == ' '; ++idx) {
        cursor.deleteChar();
    }
}

bool IndentExtension::keyPress(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        if (mTextEdit->textCursor().hasSelection()) {
            processSelection(indent);
        } else {
            insertIndentation();
        }
        return true;
    }
    if (event->key() == Qt::Key_Backtab) {
        if (mTextEdit->textCursor().hasSelection()) {
            processSelection(unindent);
        }
        return true;
    }
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        insertIndentedLine();
        return true;
    }
    if (event->key() == Qt::Key_Backspace && canRemoveIndentation()) {
        removeIndentation();
        return true;
    }
    return false;
}

bool IndentExtension::canRemoveIndentation() const
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

void IndentExtension::insertIndentation()
{
    mTextEdit->insertPlainText(QString(INDENT_SIZE, ' '));
}

void IndentExtension::processSelection(ProcessSelectionCallback callback)
{
    auto doc = mTextEdit->document();
    auto cursor = mTextEdit->textCursor();

    auto start = doc->findBlock(cursor.selectionStart());
    auto end = doc->findBlock(cursor.selectionEnd());
    if (cursor.columnNumber() > 0) {
        end = end.next();
    }
    QTextCursor editCursor(start);
    editCursor.beginEditBlock();
    while (editCursor.block() != end) {
        callback(editCursor);
        if (!editCursor.movePosition(QTextCursor::NextBlock)) {
            break;
        }
    }
    editCursor.endEditBlock();
}

void IndentExtension::removeIndentation()
{
    auto cursor = mTextEdit->textCursor();
    int col = cursor.columnNumber();
    int delta = (col % INDENT_SIZE == 0) ? INDENT_SIZE : (col % INDENT_SIZE);
    cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, delta);
    cursor.removeSelectedText();
}

void IndentExtension::insertIndentedLine()
{
    auto cursor = mTextEdit->textCursor();
    if (cursor.columnNumber() > 0) {
        QString line = cursor.block().text();
        QString prefix = findCommonPrefix(line);
        mTextEdit->insertPlainText('\n' + prefix);
    } else {
        mTextEdit->insertPlainText("\n");
    }
    mTextEdit->ensureCursorVisible();
}
