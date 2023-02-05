#ifndef LINKEXTENSION_H
#define LINKEXTENSION_H

#include "TextEdit.h"

#include <memory>

class LinkExtension : public TextEditExtension {
    Q_OBJECT
public:
    explicit LinkExtension(TextEdit* textEdit);

    void aboutToShowContextMenu(QMenu* menu, const QPoint& pos) override;

    bool keyPress(QKeyEvent* event) override;

    bool keyRelease(QKeyEvent* event) override;

    bool mouseRelease(QMouseEvent* event) override;

private:
    void openLinkUnderCursor();

    const std::unique_ptr<QAction> mOpenLinkAction;
};

#endif // LINKEXTENSION_H
