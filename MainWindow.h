#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void loadNotes();
    void saveNotes();
    void loadGeometry();
    void saveGeometry();

    QTextEdit* mTextEdit;
    QTimer* mAutoSaveTimer;
};

#endif // MAINWINDOW_H
