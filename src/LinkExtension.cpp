#include "LinkExtension.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QMenu>
#include <QMimeData>

#include "LinkSyntaxHighlighter.h"

static QUrl getLinkUnderCursor(const QTextCursor &cursor)
{
    return LinkSyntaxHighlighter::getLinkAt(cursor.block().text(), cursor.positionInBlock());
}

LinkExtension::LinkExtension(TextEdit *textEdit)
    : TextEditExtension(textEdit)
{}

void LinkExtension::aboutToShowContextMenu(QMenu *menu, const QPoint& pos)
{
    auto cursor = mTextEdit->cursorForPosition(pos);
    QUrl url = getLinkUnderCursor(cursor);
    if (!url.isValid()) {
        return;
    }
    menu->addAction(tr("Copy link address"), this, [url] {
        auto data = new QMimeData;
        data->setUrls({url});
        qGuiApp->clipboard()->setMimeData(data);
    });
    menu->addAction(tr("Open link"), this, [url] {
        QDesktopServices::openUrl(url);
    });
}

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
    QUrl url = getLinkUnderCursor(mTextEdit->textCursor());
    if (url.isValid()) {
        QDesktopServices::openUrl(url);
    }
}
