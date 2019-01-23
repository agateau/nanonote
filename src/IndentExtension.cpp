#include "IndentExtension.h"

#include <QAction>
#include <QDebug>
#include <QMenu>
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

static QString findListPrefix(const QString &line)
{
    int idx;
    for (idx = 0; idx < line.length(); ++idx) {
        if (line[idx] != ' ') {
            break;
        }
    }

    int bulletSize = findBulletSize(line.midRef(idx));
    if (bulletSize) {
        return line.left(idx+bulletSize);
    }
    return QString();
}

static void indentLine(QTextCursor &cursor)
{
    static QString spaces = QString(INDENT_SIZE, ' ');
    cursor.insertText(spaces);
}

static void unindentLine(QTextCursor &cursor)
{
    const auto text = cursor.block().text();
    for (int idx = 0; idx < std::min(INDENT_SIZE, text.size()) && text.at(idx) == ' '; ++idx) {
        cursor.deleteChar();
    }
}

IndentExtension::IndentExtension(TextEdit *textEdit)
    : TextEditExtension(textEdit)
    , mIndentAction(new QAction(this))
    , mUnindentAction(new QAction(this))
{
    mIndentAction->setText(tr("Indent"));
    mIndentAction->setShortcut(Qt::CTRL | Qt::Key_I);
    connect(mIndentAction, &QAction::triggered, this, [this] {
        processSelection(indentLine);
    });
    mTextEdit->addAction(mIndentAction);

    mUnindentAction->setText(tr("Unindent"));
    mUnindentAction->setShortcuts({Qt::CTRL | Qt::Key_U, Qt::CTRL | Qt::SHIFT | Qt::Key_I});
    connect(mUnindentAction, &QAction::triggered, this, [this] {
        processSelection(unindentLine);
    });
    mTextEdit->addAction(mUnindentAction);
}

void IndentExtension::aboutToShowContextMenu(QMenu *menu, const QPoint &/*pos*/)
{
    menu->addAction(mIndentAction);
    menu->addAction(mUnindentAction);
    menu->addSeparator();
}

bool IndentExtension::keyPress(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab && event->modifiers() == 0) {
        onTabPressed();
        return true;
    }
    if (event->key() == Qt::Key_Backtab) {
        processSelection(unindentLine);
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
    auto cursor = mTextEdit->textCursor();

    if (cursor.columnNumber() > 0) {
        QString line = cursor.block().text();
        QString prefix = findListPrefix(line);

        // If the cursor is at the start of a list item, indent the line
        // instead of adding spaces at the cursor position.
        if (prefix.length() == cursor.columnNumber()) {
            processSelection(indentLine);
            return;
        }
    }

    int count = INDENT_SIZE - (cursor.columnNumber() % INDENT_SIZE);
    cursor.insertText(QString(count, ' '));
}

void IndentExtension::processSelection(ProcessSelectionCallback callback)
{
    auto cursor = mTextEdit->textCursor();
    auto doc = mTextEdit->document();
    QTextBlock startBlock, endBlock;
    if (cursor.hasSelection()) {
        auto start = cursor.selectionStart();
        auto end = cursor.selectionEnd();
        if (start > end) {
            std::swap(start, end);
        }
        startBlock = doc->findBlock(cursor.selectionStart());
        endBlock = doc->findBlock(cursor.selectionEnd());
        // If the end is not at the start of the block, select this block too
        if (end - endBlock.position() > 0) {
            endBlock = endBlock.next();
        }
    } else {
        startBlock = cursor.block();
        endBlock = startBlock.next();
    }

    QTextCursor editCursor(startBlock);
    editCursor.beginEditBlock();
    while (editCursor.block() != endBlock) {
        callback(editCursor);
        if (!editCursor.movePosition(QTextCursor::NextBlock)) {
            break;
        }
    }
    editCursor.endEditBlock();
}

void IndentExtension::onTabPressed()
{
    auto cursor = mTextEdit->textCursor();
    if (cursor.selectedText().contains(QChar::ParagraphSeparator)) {
        processSelection(indentLine);
    } else {
        insertIndentation();
    }
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
