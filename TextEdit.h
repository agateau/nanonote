#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>

class TextEdit : public QPlainTextEdit {
public:
    TextEdit(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool canRemoveIndentation() const;
    void insertIndentation();
    void removeIndentation();
    void insertIndentedLine();
    void openLinkUnderCursor();
};

#endif /* TEXTEDIT_H */
