#include "TextEdit.h"

#include <QContextMenuEvent>
#include <QMenu>

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
