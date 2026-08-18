#include "LinkExtension.h"

#include <QClipboard>
#include <QCursor>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QMenu>
#include <QMimeData>

#include "SyntaxHighlighter.h"

static QUrl getLinkUnderCursor(const QTextCursor& cursor) {
    return SyntaxHighlighter::getLinkAt(cursor.block().text(), cursor.positionInBlock());
}

LinkExtension::LinkExtension(TextEdit* textEdit)
        : TextEditExtension(textEdit), mOpenLinkAction(std::make_unique<QAction>()) {
    mOpenLinkAction->setText(tr("Go to link"));
    mOpenLinkAction->setShortcut(Qt::CTRL | Qt::Key_G);
    connect(mOpenLinkAction.get(), &QAction::triggered, this, &LinkExtension::openLinkUnderCursor);
    mTextEdit->addAction(mOpenLinkAction.get());
}

void LinkExtension::aboutToShowContextMenu(QMenu* menu, const QPoint& pos) {
    auto cursor = mTextEdit->cursorForPosition(pos);
    QUrl url = getLinkUnderCursor(cursor);
    if (!url.isValid()) {
        return;
    }
    auto* copyAction = menu->addAction(tr("Copy link address"));
    connect(copyAction, &QAction::triggered, this, [url] {
        auto data = new QMimeData;
        data->setUrls({url});
        qGuiApp->clipboard()->setMimeData(data);
    });

    auto* openAction = menu->addAction(mOpenLinkAction->text());
    openAction->setShortcut(mOpenLinkAction->shortcut());
    connect(openAction, &QAction::triggered, this, [url] { QDesktopServices::openUrl(url); });
}

bool LinkExtension::keyPress(QKeyEvent* event) {
    if (event->modifiers() == Qt::ControlModifier) {
        mTextEdit->viewport()->setMouseTracking(true);
        updateMouseCursor();
    }
    return false;
}

bool LinkExtension::keyRelease(QKeyEvent* event) {
    if (event->modifiers() != Qt::ControlModifier) {
        reset();
    }
    return false;
}

bool LinkExtension::mouseMove(QMouseEvent* event) {
    updateMouseCursor();
    return false;
}

bool LinkExtension::mouseRelease(QMouseEvent* event) {
    if (event->modifiers() == Qt::ControlModifier) {
        openLinkUnderCursor();
    }
    return false;
}

void LinkExtension::openLinkUnderCursor() {
    QUrl url = getLinkUnderCursor(mTextEdit->textCursor());
    if (url.isValid()) {
        QDesktopServices::openUrl(url);
    }
}

void LinkExtension::updateMouseCursor() {
    auto mousePos = mTextEdit->viewport()->mapFromGlobal(QCursor::pos());
    auto textCursor = mTextEdit->cursorForPosition(mousePos);
    auto shape =
        getLinkUnderCursor(textCursor).isValid() ? Qt::PointingHandCursor : Qt::IBeamCursor;
    mTextEdit->viewport()->setCursor(shape);
}

void LinkExtension::reset() {
    mTextEdit->viewport()->setMouseTracking(false);
    mTextEdit->viewport()->setCursor(Qt::IBeamCursor);
}
