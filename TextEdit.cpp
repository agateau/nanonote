#include "TextEdit.h"

#include <QContextMenuEvent>
#include <QMenu>

static const int INDENT_SIZE = 4;

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
    } else {
        QTextEdit::keyPressEvent(event);
    }
}
