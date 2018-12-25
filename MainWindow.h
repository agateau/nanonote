#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

class Settings;
class TextEdit;

class QAction;
class QTimer;

class SettingsDialog;

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
    void setAlwaysOnTop(bool onTop);
    void showSettingsDialog();

    Settings* mSettings;
    TextEdit* mTextEdit;
    QTimer* mAutoSaveTimer;

    QAction* mIncreaseFontAction;
    QAction* mDecreaseFontAction;
    QAction* mAlwaysOnTopAction;
    QAction* mSettingsAction;
    QPointer<SettingsDialog> mSettingsDialog;
};

#endif // MAINWINDOW_H
