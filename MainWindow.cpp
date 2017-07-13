#include "MainWindow.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextEdit>
#include <QTimer>

static QString notePath()
{
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return dirPath + "/nanonote.txt";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mTextEdit(new QTextEdit(this))
    , mAutoSaveTimer(new QTimer(this))
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setCentralWidget(mTextEdit);

    mTextEdit->setAcceptRichText(false);
    mTextEdit->setFontFamily("Mono");

    mAutoSaveTimer->setInterval(1000);
    mAutoSaveTimer->setSingleShot(true);
    connect(mAutoSaveTimer, &QTimer::timeout, this, &MainWindow::saveNotes);

    connect(mTextEdit, &QTextEdit::textChanged, this, [this]() {
        mAutoSaveTimer->start();
    });

    loadNotes();
}

MainWindow::~MainWindow()
{
    saveNotes();
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
