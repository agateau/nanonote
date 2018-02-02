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
        insertPlainText(QString(INDENT_SIZE, ' '));
        event->accept();
    } else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        QString indentation = getIndentation(textCursor().block().text());
        insertPlainText('\n' + indentation);
        event->accept();
    } else {
        QTextEdit::keyPressEvent(event);
    }
}
