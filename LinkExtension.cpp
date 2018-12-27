#include "LinkExtension.h"

#include <QDesktopServices>

#include "LinkSyntaxHighlighter.h"

LinkExtension::LinkExtension(TextEdit *textEdit)
    : TextEditExtension(textEdit)
{}

bool LinkExtension::keyPress(QKeyEvent *event)
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

bool LinkExtension::keyRelease(QKeyEvent *event)
{
    if (event->modifiers() != Qt::CTRL) {
        mTextEdit->viewport()->setCursor(Qt::IBeamCursor);
    }
    return false;
}

bool LinkExtension::mouseRelease(QMouseEvent *event)
{
    if (event->modifiers() == Qt::CTRL) {
        openLinkUnderCursor();
    }
    return false;
}

void LinkExtension::openLinkUnderCursor()
{
    auto cursor = mTextEdit->textCursor();
    QUrl url = LinkSyntaxHighlighter::getLinkAt(cursor.block().text(), cursor.positionInBlock());
    if (url.isValid()) {
        QDesktopServices::openUrl(url);
    }
}
