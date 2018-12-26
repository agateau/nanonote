#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>

class TextEdit;

class TextEditFilter : public QObject {
public:
    explicit TextEditFilter(TextEdit *textEdit);

    virtual bool keyPress(QKeyEvent *event);

    virtual bool keyRelease(QKeyEvent *event);

    virtual bool mouseRelease(QMouseEvent *event);

protected:
    TextEdit *mTextEdit;
};


class TextEdit : public QPlainTextEdit {
public:
    TextEdit(QWidget *parent = nullptr);

    void addFilter(TextEditFilter *filter);

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

    QList<TextEditFilter*> mFilters;
};

#endif /* TEXTEDIT_H */
