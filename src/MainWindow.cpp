#include "MainWindow.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QMenu>
#include <QSaveFile>
#include <QScreen>
#include <QTimer>
#include <QWindow>

#include <chrono>

#include "IndentExtension.h"
#include "LinkExtension.h"
#include "MoveLinesExtension.h"
#include "SearchWidget.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "TaskExtension.h"
#include "TextEdit.h"
#include "WheelZoomExtension.h"

#include <QLayout>
#include <QToolBar>

#include <QApplication>
#include <QClipboard>

using namespace std::chrono_literals;

static const auto AUTOSAVE_INTERVAL = 1s;

//- MainWindowExtension --------------------------------
MainWindowExtension::MainWindowExtension(MainWindow* window)
        : TextEditExtension(window->mTextEdit), mWindow(window) {
}

void MainWindowExtension::aboutToShowContextMenu(QMenu* menu, const QPoint&) {
    menu->addAction(mWindow->mSearchAction);
    menu->addSeparator();
    menu->addAction(mWindow->mSettingsAction);
}

void MainWindowExtension::aboutToShowViewContextMenu(QMenu* menu, const QPoint&) {
    menu->addAction(mWindow->mIncreaseFontAction);
    menu->addAction(mWindow->mDecreaseFontAction);
    menu->addAction(mWindow->mResetFontAction);
    menu->addSeparator();
    menu->addAction(mWindow->mAlwaysOnTopAction);
}

//- MainWindow -----------------------------------------
MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , mSettings(new Settings(this))
        , mTextEdit(new TextEdit(this))
        , mAutoSaveTimer(new QTimer(this))
        , mSearchWidget(new SearchWidget(mTextEdit, this))
        , mSearchToolBar(new QToolBar(this))
        , mIncreaseFontAction(new QAction(this))
        , mDecreaseFontAction(new QAction(this))
        , mResetFontAction(new QAction(this))
        , mAlwaysOnTopAction(new QAction(this))
        , mSettingsAction(new QAction(this))
        , mSearchAction(new QAction(this))
        , mCloseSearchAction(new QAction(this)) {
    setWindowTitle("Nanonote");

    setCentralWidget(mTextEdit);

    setupTextEdit();
    setupSearchBar();
    setupAutoSaveTimer();
    setupActions();
    loadNotes();
    loadSettings();
}

MainWindow::~MainWindow() {
    saveNotes();
    saveSettings();
}

void MainWindow::setupTextEdit() {
    mTextEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    mTextEdit->addExtension(new LinkExtension(mTextEdit));
    mTextEdit->addExtension(new TaskExtension(mTextEdit));
    mTextEdit->addExtension(new IndentExtension(mTextEdit));

    WheelZoomExtension* wheelZoomExtension = new WheelZoomExtension(mTextEdit);
    mTextEdit->addExtension(wheelZoomExtension);
    connect(
        wheelZoomExtension, &WheelZoomExtension::adjustFontSize, this, &MainWindow::adjustFontSize);

    mTextEdit->addExtension(new MainWindowExtension(this));
    mTextEdit->addExtension(new MoveLinesExtension(mTextEdit));
}

void MainWindow::setupAutoSaveTimer() {
    mAutoSaveTimer->setInterval(AUTOSAVE_INTERVAL);
    mAutoSaveTimer->setSingleShot(true);
    connect(mAutoSaveTimer, &QTimer::timeout, this, &MainWindow::saveNotes);

    connect(mTextEdit, &QPlainTextEdit::textChanged, this, [this]() { mAutoSaveTimer->start(); });
}

void MainWindow::setupActions() {
    mAlwaysOnTopAction->setCheckable(true);

    mIncreaseFontAction->setText(tr("Increase Font Size"));
    mIncreaseFontAction->setShortcut(QKeySequence::ZoomIn);

    mDecreaseFontAction->setText(tr("Decrease Font Size"));
    mDecreaseFontAction->setShortcut(QKeySequence::ZoomOut);

    mResetFontAction->setText(tr("Reset Font Size"));
    mResetFontAction->setShortcut(Qt::CTRL + Qt::Key_0);

    connect(mIncreaseFontAction, &QAction::triggered, this, [this] { adjustFontSize(1); });
    connect(mDecreaseFontAction, &QAction::triggered, this, [this] { adjustFontSize(-1); });
    connect(mResetFontAction, &QAction::triggered, this, &MainWindow::resetFontSize);

    addAction(mIncreaseFontAction);
    addAction(mDecreaseFontAction);
    addAction(mResetFontAction);

    mAlwaysOnTopAction->setText(tr("Always on Top"));
    mAlwaysOnTopAction->setShortcut(Qt::CTRL + Qt::Key_T);
    connect(mAlwaysOnTopAction, &QAction::toggled, this, &MainWindow::setAlwaysOnTop);
    addAction(mAlwaysOnTopAction);

    mSettingsAction->setText(tr("Settings | About..."));
    connect(mSettingsAction, &QAction::triggered, this, &MainWindow::showSettingsDialog);
    addAction(mSettingsAction);

    // Add find shortcut
    mSearchAction->setText(tr("Find"));
    mSearchAction->setShortcut(QKeySequence::Find);
    connect(mSearchAction, &QAction::triggered, this, &MainWindow::showSearchBar);
    addAction(mSearchAction);

    mCloseSearchAction->setShortcut(Qt::Key_Escape);
    connect(mCloseSearchAction, &QAction::triggered, this, &MainWindow::hideSearchBar);
    addAction(mCloseSearchAction);

    // Add the standard "quit" shortcut
    auto quitAction = new QAction(this);
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);
    addAction(quitAction);
}

void MainWindow::loadNotes() {
    QString path = Settings::notePath();
    QFile file(path);
    if (!file.exists()) {
        mTextEdit->setPlainText(tr(
            "Welcome to Nanonote!\n"
            "\n"
            "Nanonote is a minimalist note taking application.\n"
            "\n"
            "It's meant for short-lived notes. Anything you type here is automatically saved on "
            "your disk.\n"
            "\n"
            "The only UI is the context menu, try it out!\n"
            "\n"
            "As you can see in the context menu, Nanonote has an \"Always on Top\" mode. This "
            "feature is handy to keep the window around.\n"
            "\n"
            "It also has a few handy editing features, like auto-bullet lists:\n"
            "\n"
            "- Try to move the cursor at the end of this line and press Enter\n"
            "- This works for\n"
            "    - nested lists\n"
            "    * and asterisks\n"
            "\n"
            "You can also open urls using Control + click or Control + Enter while your cursor is "
            "inside a URL. You can try clicking on this one for example: "
            "https://github.com/agateau/nanonote.\n"
            "\n"
            "Finally, you can indent selected lines with Tab or Ctrl+I and unindent them with "
            "Shift+Tab or Ctrl+U.\n"
            "\n"
            "That's all there is to say, now you can erase this text and start taking notes!\n"));
        return;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open" << file.fileName();
        return;
    }
    QString text = QString::fromUtf8(file.readAll());
    mTextEdit->setPlainText(text);
}

void MainWindow::saveNotes() {
    QString path = Settings::notePath();
    QString dirPath = QFileInfo(path).absolutePath();
    if (!QDir(dirPath).mkpath(".")) {
        qWarning() << "Failed to create" << dirPath;
        return;
    }
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to create" << path << ":" << file.errorString();
        return;
    }
    file.write(mTextEdit->toPlainText().toUtf8());
    if (!file.commit()) {
        qWarning() << "Could not save notes to" << path;
    }
}

static QRect clampRect(const QRect& rect_, const QRect& container) {
    QRect rect = rect_;
    if (rect.right() > container.right()) {
        rect.moveRight(container.right());
    }
    if (rect.bottom() > container.bottom()) {
        rect.moveBottom(container.bottom());
    }
    if (rect.left() < container.left()) {
        rect.moveLeft(container.left());
    }
    if (rect.top() < container.top()) {
        rect.moveTop(container.top());
    }
    return rect;
}

void MainWindow::loadSettings() {
    mSettings->load();

    QRect geometry = mSettings->geometry();
    if (geometry.isValid()) {
        QRect screenRect = QGuiApplication::primaryScreen()->availableGeometry();
        geometry = clampRect(geometry, screenRect);
        setGeometry(geometry);
    }

    mAlwaysOnTopAction->setChecked(mSettings->alwaysOnTop());

    mTextEdit->setFont(mSettings->font());

    connect(mSettings, &Settings::fontChanged, mTextEdit, &QPlainTextEdit::setFont);
}

void MainWindow::saveSettings() {
    mSettings->setGeometry(geometry());
    mSettings->save();
}

void MainWindow::adjustFontSize(int delta) {
    QFont font = mSettings->font();
    font.setPointSize(font.pointSize() + delta);
    mSettings->setFont(font);
    saveSettings();
}

void MainWindow::resetFontSize() {
    QFont font = mSettings->font();
    QFont defaultFont = mSettings->defaultFont();
    font.setPointSize(defaultFont.pointSize());
    mSettings->setFont(font);
    saveSettings();
}

void MainWindow::setAlwaysOnTop(bool onTop) {
    Qt::WindowFlags flags = windowFlags();
    flags.setFlag(Qt::WindowStaysOnTopHint, onTop);
    setWindowFlags(flags);
    show();
    mSettings->setAlwaysOnTop(onTop);
    saveSettings();
}

void MainWindow::showSettingsDialog() {
    if (!mSettingsDialog) {
        mSettingsDialog = new SettingsDialog(mSettings, this);
    }
    mSettingsDialog->show();
}

void MainWindow::setupSearchBar() {
    connect(mSearchWidget, &SearchWidget::closeClicked, this, &MainWindow::hideSearchBar);

    mSearchToolBar->addWidget(mSearchWidget);
    mSearchToolBar->setVisible(false);
    mSearchToolBar->setMovable(false);
    addToolBar(Qt::BottomToolBarArea, mSearchToolBar);
}

void MainWindow::showSearchBar() {
    if (mSearchToolBar->isVisible()) {
        mSearchWidget->setFocus();
    } else {
        mSearchWidget->initialize(mTextEdit->textCursor().selectedText());
        mSearchToolBar->show();
    }
}

void MainWindow::hideSearchBar() {
    mSearchWidget->uninitialize();
    mSearchToolBar->hide();
}
