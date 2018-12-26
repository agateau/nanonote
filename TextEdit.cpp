#include "TextEdit.h"

#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>

#include "IndentTextEditFilter.h"
#include "LinkSyntaxHighlighter.h"
#include "LinkTextEditFilter.h"

// TextEditFilter ------------------------
TextEditFilter::TextEditFilter(TextEdit *textEdit)
    : QObject(textEdit)
    , mTextEdit(textEdit)
{}

bool TextEditFilter::keyPress(QKeyEvent */*event*/) {
    return false;
}

bool TextEditFilter::keyRelease(QKeyEvent */*event*/) {
    return false;
}

bool TextEditFilter::mouseRelease(QMouseEvent */*event*/) {
    return false;
}
// TextEdit ------------------------------
TextEdit::TextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    new LinkSyntaxHighlighter(document());
    addFilter(new LinkTextEditFilter(this));
    addFilter(new IndentTextEditFilter(this));
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
    for (auto filter : mFilters) {
        if (filter->keyPress(event)) {
            return;
        }
    }
    QPlainTextEdit::keyPressEvent(event);
}

void TextEdit::keyReleaseEvent(QKeyEvent *event)
{
    for (auto filter : mFilters) {
        if (filter->keyRelease(event)) {
            return;
        }
    }
    QPlainTextEdit::keyReleaseEvent(event);
}

void TextEdit::mouseReleaseEvent(QMouseEvent *event)
{
    for (auto filter : mFilters) {
        if (filter->mouseRelease(event)) {
            return;
        }
    }
    QPlainTextEdit::mouseReleaseEvent(event);
}

void TextEdit::addFilter(TextEditFilter *filter)
{
    mFilters << filter;
}
