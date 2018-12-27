#include "TextEdit.h"

#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>

#include <memory>

#include "LinkSyntaxHighlighter.h"

// TextEditExtension ---------------------
TextEditExtension::TextEditExtension(TextEdit *textEdit)
    : QObject(textEdit)
    , mTextEdit(textEdit)
{}

void TextEditExtension::aboutToShowContextMenu(QMenu */*menu*/, const QPoint& /*pos*/)
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
}

void TextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    auto pos = event->pos();
    std::unique_ptr<QMenu> menu(createStandardContextMenu(pos));
    menu->addSeparator();
    for (auto extension : mExtensions) {
        extension->aboutToShowContextMenu(menu, pos);
    }
    menu->addSeparator();
    for (QAction *action : actions()) {
        menu->addAction(action);
    }
    menu->exec(event->globalPos());
}

void TextEdit::keyPressEvent(QKeyEvent *event)
{
    for (auto extension : mExtensions) {
        if (extension->keyPress(event)) {
            return;
        }
    }
    QPlainTextEdit::keyPressEvent(event);
}

void TextEdit::keyReleaseEvent(QKeyEvent *event)
{
    for (auto extension : mExtensions) {
        if (extension->keyRelease(event)) {
            return;
        }
    }
    QPlainTextEdit::keyReleaseEvent(event);
}

void TextEdit::mouseReleaseEvent(QMouseEvent *event)
{
    for (auto extension : mExtensions) {
        if (extension->mouseRelease(event)) {
            return;
        }
    }
    QPlainTextEdit::mouseReleaseEvent(event);
}

void TextEdit::addExtension(TextEditExtension *extension)
{
    mExtensions << extension;
}
