#include "TextEdit.h"

#include <QContextMenuEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QMenu>

#include "LinkSyntaxHighlighter.h"

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

class LinkEventFilter : public QObject {
public:
    explicit LinkEventFilter(TextEdit *textEdit)
        : QObject(textEdit)
        , mTextEdit(textEdit)
    {}

protected:
    bool eventFilter(QObject *object, QEvent *event) override
    {
        if (object == mTextEdit->viewport()) {
            switch (event->type()) {
            case QEvent::MouseButtonRelease:
                mouseReleaseEvent(static_cast<QMouseEvent*>(event));
                break;
            default:
                break;
            }
        } else {
            switch (event->type()) {
            case QEvent::KeyPress:
                if (keyPressEvent(static_cast<QKeyEvent*>(event))) {
                    return true;
                }
                break;
            case QEvent::KeyRelease:
                keyReleaseEvent(static_cast<QKeyEvent*>(event));
                break;
            default:
                break;
            }
        }
        return false;
    }

private:
    bool keyPressEvent(QKeyEvent *event)
    {
        bool ctrlPressed = event->modifiers() == Qt::CTRL;
        bool enterPressed = event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return;
        if (ctrlPressed) {
            mTextEdit->viewport()->setCursor(Qt::PointingHandCursor);
        }
        if (ctrlPressed && enterPressed) {
            openLinkUnderCursor();
            return true;
        }
        return false;
    }

    void keyReleaseEvent(QKeyEvent *event)
    {
        if (event->modifiers() != Qt::CTRL) {
            mTextEdit->viewport()->setCursor(Qt::IBeamCursor);
        }
    }

    void mouseReleaseEvent(QMouseEvent *event)
    {
        if (event->modifiers() == Qt::CTRL) {
            openLinkUnderCursor();
        }
    }

    void openLinkUnderCursor()
    {
        auto cursor = mTextEdit->textCursor();
        QUrl url = LinkSyntaxHighlighter::getLinkAt(cursor.block().text(), cursor.positionInBlock());
        if (url.isValid()) {
            QDesktopServices::openUrl(url);
        }
    }

    TextEdit *mTextEdit;
};

TextEdit::TextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    new LinkSyntaxHighlighter(document());
    auto linkEventFilter = new LinkEventFilter(this);
    installEventFilter(linkEventFilter);
    viewport()->installEventFilter(linkEventFilter);
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
        QPlainTextEdit::keyPressEvent(event);
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
