#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>

class TextEdit : public QPlainTextEdit {
public:
    TextEdit(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    bool canRemoveIndentation() const;
    void insertIndentation();
    void removeIndentation();
    void insertIndentedLine();
};

#endif /* TEXTEDIT_H */
