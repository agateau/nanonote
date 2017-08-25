#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TextEdit;

class QAction;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setupActions();
    void loadNotes();
    void saveNotes();
    void loadSettings();
    void saveSettings();
    void adjustFontSize(int delta);

    TextEdit* mTextEdit;
    QTimer* mAutoSaveTimer;

    QAction* mIncreaseFontAction;
    QAction* mDecreaseFontAction;
};

#endif // MAINWINDOW_H
