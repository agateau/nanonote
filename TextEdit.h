#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>

class TextEdit : public QTextEdit {
public:
    TextEdit(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif /* TEXTEDIT_H */
