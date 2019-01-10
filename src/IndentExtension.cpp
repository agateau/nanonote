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
        insertIndentation();
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
    int count = INDENT_SIZE - (cursor.columnNumber() % INDENT_SIZE);
    cursor.insertText(QString(count, ' '));
}

void IndentExtension::processSelection(ProcessSelectionCallback callback)
{
    auto cursor = mTextEdit->textCursor();
    auto doc = mTextEdit->document();
    QTextBlock start, end;
    if (cursor.hasSelection()) {
        start = doc->findBlock(cursor.selectionStart());
        end = doc->findBlock(cursor.selectionEnd());
        if (cursor.columnNumber() > 0) {
            end = end.next();
        }
    } else {
        start = cursor.block();
        end = start.next();
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
