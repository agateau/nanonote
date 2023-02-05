#ifndef TASKEXTENSION_H
#define TASKEXTENSION_H

#include "TextEdit.h"

#include <QAction>

#include <memory>

class TaskExtension : public TextEditExtension {
    Q_OBJECT
public:
    explicit TaskExtension(TextEdit* textEdit);

    void aboutToShowEditContextMenu(QMenu* menu, const QPoint& pos) override;

    bool mouseRelease(QMouseEvent* event) override;

    void insertOrToggleTask();

private:
    void toggleTaskUnderCursor();

    const std::unique_ptr<QAction> mTaskAction;
};

#endif // TASKEXTENSION_H
