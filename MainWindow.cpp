#include "MainWindow.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include <QStandardPaths>
#include <QTextEdit>
#include <QTimer>
#include <QWindow>

static QString notePath()
{
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return dirPath + "/nanonote.txt";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mTextEdit(new QTextEdit(this))
    , mAutoSaveTimer(new QTimer(this))
    , mIncreaseFontAction(new QAction(this))
    , mDecreaseFontAction(new QAction(this))
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowTitle("Nanonote");
    setCentralWidget(mTextEdit);

    mTextEdit->setAcceptRichText(false);
    mTextEdit->setFontFamily("Mono");

    mAutoSaveTimer->setInterval(1000);
    mAutoSaveTimer->setSingleShot(true);
    connect(mAutoSaveTimer, &QTimer::timeout, this, &MainWindow::saveNotes);

    connect(mTextEdit, &QTextEdit::textChanged, this, [this]() {
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

void MainWindow::setupActions()
{
    mIncreaseFontAction->setShortcut(QKeySequence::ZoomIn);
    mDecreaseFontAction->setShortcut(QKeySequence::ZoomOut);

    connect(mIncreaseFontAction, &QAction::triggered, [this] { adjustFontSize(1); });
    connect(mDecreaseFontAction, &QAction::triggered, [this] { adjustFontSize(-1); });

    addAction(mIncreaseFontAction);
    addAction(mDecreaseFontAction);
}

void MainWindow::loadNotes()
{
    QString path = notePath();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QString text = QString::fromUtf8(file.readAll());
    mTextEdit->setText(text);
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
    QSettings settings;
    QRect geometry = settings.value("geometry").toRect();
    if (geometry.isValid()) {
        QRect screenRect = QGuiApplication::primaryScreen()->availableGeometry();
        geometry = clampRect(geometry, screenRect);
        setGeometry(geometry);
    }

    QVariant fontVariant = settings.value("font");
    if (fontVariant.canConvert<QFont>()) {
        mTextEdit->setFont(fontVariant.value<QFont>());
    }
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("geometry", geometry());
    settings.setValue("font", mTextEdit->font());
}

void MainWindow::adjustFontSize(int delta)
{
    QFont font = mTextEdit->font();
    font.setPointSize(font.pointSize() + delta);
    mTextEdit->setFont(font);

    saveSettings();
}
