#ifndef MOVELINEEXTENSION_H
#define MOVELINEEXTENSION_H

#include "TextEdit.h"

#include <memory>

class Action;

class MoveLinesExtension : public TextEditExtension {
    Q_OBJECT
public:
    explicit MoveLinesExtension(TextEdit* textEdit);
    ~MoveLinesExtension();

    void aboutToShowEditContextMenu(QMenu* menu, const QPoint& pos) override;

    void moveUp();
    void moveDown();

private:
    void moveSelectedLines(int delta);

    const std::unique_ptr<QAction> mMoveUpAction;
    const std::unique_ptr<QAction> mMoveDownAction;
};

#endif // MOVELINEEXTENSION_H
