#include "MainWindow.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QMenu>
#include <QScreen>
#include <QStandardPaths>
#include <QTimer>
#include <QWindow>

#include "IndentExtension.h"
#include "LinkExtension.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "TextEdit.h"

static QString notePath()
{
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return dirPath + "/nanonote.txt";
}

//- MainWindowExtension --------------------------------
MainWindowExtension::MainWindowExtension(MainWindow *window)
    : TextEditExtension(window->mTextEdit)
    , mWindow(window)
{}

void MainWindowExtension::aboutToShowContextMenu(QMenu *menu, const QPoint &)
{
    menu->addAction(mWindow->mIncreaseFontAction);
    menu->addAction(mWindow->mDecreaseFontAction);
    menu->addAction(mWindow->mAlwaysOnTopAction);
    menu->addSeparator();
    menu->addAction(mWindow->mSettingsAction);
}

//- MainWindow -----------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mSettings(new Settings(this))
    , mTextEdit(new TextEdit(this))
    , mAutoSaveTimer(new QTimer(this))
    , mIncreaseFontAction(new QAction(this))
    , mDecreaseFontAction(new QAction(this))
    , mAlwaysOnTopAction(new QAction(this))
    , mSettingsAction(new QAction(this))
{
    setWindowTitle("Nanonote");
    setCentralWidget(mTextEdit);

    setupTextEdit();

    mAutoSaveTimer->setInterval(1000);
    mAutoSaveTimer->setSingleShot(true);
    connect(mAutoSaveTimer, &QTimer::timeout, this, &MainWindow::saveNotes);

    connect(mTextEdit, &QPlainTextEdit::textChanged, this, [this]() {
        mAutoSaveTimer->start();
    });

    setupActions();
    loadNotes();
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveNotes();
    saveSettings();
}

void MainWindow::setupTextEdit()
{
    mTextEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    mTextEdit->addExtension(new LinkExtension(mTextEdit));
    mTextEdit->addExtension(new IndentExtension(mTextEdit));
    mTextEdit->addExtension(new MainWindowExtension(this));
}

void MainWindow::setupActions()
{
    mAlwaysOnTopAction->setCheckable(true);

    mIncreaseFontAction->setText(tr("Increase Font Size"));
    mIncreaseFontAction->setShortcut(QKeySequence::ZoomIn);

    mDecreaseFontAction->setText(tr("Decrease Font Size"));
    mDecreaseFontAction->setShortcut(QKeySequence::ZoomOut);

    connect(mIncreaseFontAction, &QAction::triggered, this, [this] { adjustFontSize(1); });
    connect(mDecreaseFontAction, &QAction::triggered, this, [this] { adjustFontSize(-1); });

    addAction(mIncreaseFontAction);
    addAction(mDecreaseFontAction);

    mAlwaysOnTopAction->setText(tr("Always on Top"));
    mAlwaysOnTopAction->setShortcut(Qt::CTRL + Qt::Key_T);
    connect(mAlwaysOnTopAction, &QAction::toggled, this, &MainWindow::setAlwaysOnTop);
    addAction(mAlwaysOnTopAction);

    mSettingsAction->setText(tr("Settings..."));
    connect(mSettingsAction, &QAction::triggered, this, &MainWindow::showSettingsDialog);
    addAction(mSettingsAction);

    // Add the standard "quit" shortcut
    auto quitAction = new QAction(this);
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);
    addAction(quitAction);
}

static QString readFile(const QString &path)
{
    QFile file(path);
    QString text;
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    return QString::fromUtf8(file.readAll());
}

void MainWindow::loadNotes()
{
    QString path = notePath();
    QString text;
    if (QFile::exists(path)) {
        text = readFile(path);
    } else {
        text = readFile(":/welcome.txt");
        text.replace("$NOTE_PATH", path);
    }
    mTextEdit->setPlainText(text);
}

void MainWindow::saveNotes()
{
    QString path = notePath();
    QString dirPath = QFileInfo(path).absolutePath();
    if (!QDir(dirPath).mkpath(".")) {
        qWarning() << "Failed to create" << dirPath;
        return;
    }
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to create" << path << ":" << file.errorString();
        return;
    }
    file.write(mTextEdit->toPlainText().toUtf8());
}

static QRect clampRect(const QRect& rect_, const QRect& container)
{
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

void MainWindow::loadSettings()
{
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

void MainWindow::saveSettings()
{
    mSettings->setGeometry(geometry());
    mSettings->save();
}

void MainWindow::adjustFontSize(int delta)
{
    QFont font = mSettings->font();
    font.setPointSize(font.pointSize() + delta);
    mSettings->setFont(font);
    saveSettings();
}

void MainWindow::setAlwaysOnTop(bool onTop)
{
    Qt::WindowFlags flags = windowFlags();
    flags.setFlag(Qt::WindowStaysOnTopHint, onTop);
    setWindowFlags(flags);
    show();
    mSettings->setAlwaysOnTop(onTop);
    saveSettings();
}

void MainWindow::showSettingsDialog()
{
    if (!mSettingsDialog) {
        mSettingsDialog = new SettingsDialog(mSettings, this);
    }
    mSettingsDialog->show();
}
