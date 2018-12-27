#include "IndentTextEditFilter.h"

#include <QDebug>
#include <QTextBlock>

#include <optional>

static const int INDENT_SIZE = 4;

static std::optional<int> findBulletSize(const QStringRef &ref) {
    static QSet<QString> bullets = {"- ", "* ", "> "};
    for (auto bullet : bullets) {
        if (ref.startsWith(bullet)) {
            return bullet.length();
        }
    }
    return {};
}

static QString findCommonPrefix(const QString &line)
{
    int idx;
    for (idx = 0; idx < line.length(); ++idx) {
        if (line[idx] != ' ') {
            break;
        }
    }
    auto bulletSize = findBulletSize(line.midRef(idx));
    if (bulletSize) {
        idx += bulletSize.value();
    }
    return line.left(idx);
}

IndentTextEditFilter::IndentTextEditFilter(TextEdit *textEdit)
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

bool IndentTextEditFilter::keyPress(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        if (mTextEdit->textCursor().hasSelection()) {
            processSelection(indent);
        } else {
            insertIndentation();
        }
        event->accept();
        return true;
    }
    if (event->key() == Qt::Key_Backtab) {
        if (mTextEdit->textCursor().hasSelection()) {
            processSelection(unindent);
        }
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

void IndentTextEditFilter::processSelection(ProcessSelectionCallback callback)
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
