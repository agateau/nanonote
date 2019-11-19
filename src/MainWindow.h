#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

#include "TextEdit.h"

class Settings;
class TextEdit;

class QAction;
class QTimer;

class MainWindowExtension;
class SettingsDialog;
class SearchWidget;

class MainWindow;

class MainWindowExtension : public TextEditExtension {
public:
    explicit MainWindowExtension(MainWindow* window);

    void aboutToShowContextMenu(QMenu* menu, const QPoint& /*pos*/) override;

private:
    MainWindow* mWindow;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void setupTextEdit();
    void setupAutoSaveTimer();
    void setupActions();
    void loadNotes();
    void saveNotes();
    void loadSettings();
    void saveSettings();
    void adjustFontSize(int delta);
    void resetFontSize();
    void setAlwaysOnTop(bool onTop);
    void showSettingsDialog();
    void loadSearchWidget();
    void showSearchBar();
    void hideSearchBar();

    Settings* mSettings;
    TextEdit* mTextEdit;
    QTimer* mAutoSaveTimer;
    SearchWidget* mSearchWidget = nullptr;
    QToolBar* mSearchToolBar = nullptr;

    QAction* mIncreaseFontAction;
    QAction* mDecreaseFontAction;
    QAction* mResetFontAction;
    QAction* mAlwaysOnTopAction;
    QAction* mSettingsAction;
    QAction* mSearchAction;
    QAction* mCloseSearchAction = nullptr;
    QPointer<SettingsDialog> mSettingsDialog;

    friend class MainWindowExtension;
};

#endif // MAINWINDOW_H
