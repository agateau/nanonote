#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>

class TextEdit;

/**
 * @brief Extension system for TextEdit
 *
 * event-like methods must return true if they processed the event and do not
 * want other extensions to receive it.
 */
class TextEditExtension : public QObject {
public:
    explicit TextEditExtension(TextEdit* textEdit);

    virtual void aboutToShowContextMenu(QMenu* menu, const QPoint& pos);

    virtual bool keyPress(QKeyEvent* event);

    virtual bool keyRelease(QKeyEvent* event);

    virtual bool mouseRelease(QMouseEvent* event);

    virtual bool wheel(QWheelEvent* event);

protected:
    TextEdit* mTextEdit;
};

class TextEdit : public QPlainTextEdit {
public:
    TextEdit(QWidget* parent = nullptr);

    void addExtension(TextEditExtension* extension);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    QList<TextEditExtension*> mExtensions;
};

#endif /* TEXTEDIT_H */
