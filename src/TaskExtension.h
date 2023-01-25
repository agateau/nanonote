#ifndef TASKEXTENSION_H
#define TASKEXTENSION_H

#include "TextEdit.h"

#include <QAction>

#include <memory>

class TaskExtension : public TextEditExtension {
    Q_OBJECT
public:
    explicit TaskExtension(TextEdit* textEdit);

    void aboutToShowContextMenu(QMenu* menu, const QPoint& pos) override;
    void aboutToShowEditContextMenu(QMenu* menu, const QPoint& pos) override;

    bool keyPress(QKeyEvent* event) override;

    bool mouseRelease(QMouseEvent* event) override;

    void insertTask();

private:
    bool toggleTaskUnderCursor();

    const std::unique_ptr<QAction> mInsertToggleTaskAction;
};

#endif // TASKEXTENSION_H
