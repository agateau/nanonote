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
    void aboutToShowViewContextMenu(QMenu* menu, const QPoint& /*pos*/) override;

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
    void setupSearchBar();
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
    void showSearchBar();
    void hideSearchBar();

    Settings* const mSettings;
    TextEdit* const mTextEdit;
    QTimer* const mAutoSaveTimer;
    SearchWidget* const mSearchWidget;
    QToolBar* const mSearchToolBar;

    QAction* const mIncreaseFontAction;
    QAction* const mDecreaseFontAction;
    QAction* const mResetFontAction;
    QAction* const mAlwaysOnTopAction;
    QAction* const mSettingsAction;
    QAction* const mSearchAction;
    QAction* const mCloseSearchAction;

    QPointer<SettingsDialog> mSettingsDialog;

    friend class MainWindowExtension;
};

#endif // MAINWINDOW_H
