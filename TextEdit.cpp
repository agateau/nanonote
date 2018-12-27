#include "TextEdit.h"

#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>

#include "IndentTextEditFilter.h"
#include "LinkSyntaxHighlighter.h"
#include "LinkTextEditFilter.h"

// TextEditExtension ---------------------
TextEditExtension::TextEditExtension(TextEdit *textEdit)
    : QObject(textEdit)
    , mTextEdit(textEdit)
{}

bool TextEditExtension::keyPress(QKeyEvent */*event*/) {
    return false;
}

bool TextEditExtension::keyRelease(QKeyEvent */*event*/) {
    return false;
}

bool TextEditExtension::mouseRelease(QMouseEvent */*event*/) {
    return false;
}

// TextEdit ------------------------------
TextEdit::TextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    new LinkSyntaxHighlighter(document());
    addExtension(new LinkTextEditFilter(this));
    addExtension(new IndentTextEditFilter(this));
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
    for (auto filter : mExtensions) {
        if (filter->keyPress(event)) {
            return;
        }
    }
    QPlainTextEdit::keyPressEvent(event);
}

void TextEdit::keyReleaseEvent(QKeyEvent *event)
{
    for (auto filter : mExtensions) {
        if (filter->keyRelease(event)) {
            return;
        }
    }
    QPlainTextEdit::keyReleaseEvent(event);
}

void TextEdit::mouseReleaseEvent(QMouseEvent *event)
{
    for (auto filter : mExtensions) {
        if (filter->mouseRelease(event)) {
            return;
        }
    }
    QPlainTextEdit::mouseReleaseEvent(event);
}

void TextEdit::addExtension(TextEditExtension *filter)
{
    mExtensions << filter;
}
