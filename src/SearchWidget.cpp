#include "SearchWidget.h"
#include "ui_SearchForm.h"

#include <iostream>

#include "TextEdit.h"

SearchWidget::SearchWidget(TextEdit* textEdit, QWidget *parent )
    : QWidget( parent )
    , mUi(new Ui::SearchForm)
    , mTextEdit(textEdit)
{
    mUi->setupUi(this);
    mUi->guiPreviousButton->setArrowType(Qt::UpArrow);
    mUi->guiPreviousButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mUi->guiNextButton->setArrowType(Qt::DownArrow);
    mUi->guiNextButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QObject::connect(mUi->guiNextButton, &QToolButton::clicked, this, &SearchWidget::onNextButtonClicked);
    QObject::connect(mUi->guiPreviousButton, &QToolButton::clicked, this, &SearchWidget::onPreviousButtonClicked);
    QObject::connect(mTextEdit, &TextEdit::textChanged, this, &SearchWidget::documentChange);
    QObject::connect(mUi->guiSearchLine, &QLineEdit::textChanged, this, &SearchWidget::searchLineChanged);
    QObject::connect(mUi->guiCloseButton, &QToolButton::clicked, this, &SearchWidget::closeSearch);
}

SearchWidget::~SearchWidget()
{
    delete mUi;
}

void SearchWidget::initialize(const QString & text)
{
    mSearchVisible = true;
    mUi->guiSearchLine->setFocus();
    mUi->guiSearchLine->setText(text);
    searchWord(true);
}

void SearchWidget::uninitialize()
{
    mSearchVisible = false;
    highLightedWords(false);
}


void SearchWidget::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        onNextButtonClicked();
    }
}

void SearchWidget::searchWord(bool selectNext, QString searchValue)
{
    if (searchValue.isEmpty()) {
        searchValue = mUi->guiSearchLine->text();
    }
    mTextDocument = mTextEdit->toPlainText();
    mCurrentSelectedWord = -1;
    mPositionWords.clear();
    QTextDocument *document = mTextEdit->document();
    searchPositionsWordsInDocument(searchValue, selectNext, document);
}

void SearchWidget::onNextButtonClicked()
{
    if (mPositionWords.empty()) {
        return;
    }
    if (mCurrentSelectedWord != ((int)mPositionWords.size() - 1)) {
        mCurrentSelectedWord++;
    }
    else {
        mCurrentSelectedWord = 0;
    }
    QTextDocument *document = mTextEdit->document();
    QTextCursor cursor(document);
    cursor.beginEditBlock();
    int wordStartPosition = mPositionWords.at(mCurrentSelectedWord);
    cursor.setPosition(wordStartPosition, QTextCursor::MoveAnchor);
    cursor.setPosition(wordStartPosition + mUi->guiSearchLine->text().size(), QTextCursor::KeepAnchor);
    mTextEdit->setTextCursor(cursor);
    cursor.endEditBlock();
    setCountAndCurrentPosition();
}

void SearchWidget::onPreviousButtonClicked()
{
    if (mPositionWords.empty()) {
        return;
    }
    if (mCurrentSelectedWord != 0) {
        mCurrentSelectedWord--;
    }
    else {
        mCurrentSelectedWord = mPositionWords.size() - 1;
    }
    QTextDocument *document = mTextEdit->document();
    QTextCursor cursor(document);
    cursor.beginEditBlock();
    int wordStartPosition = mPositionWords.at(mCurrentSelectedWord);
    cursor.setPosition(wordStartPosition, QTextCursor::MoveAnchor);
    cursor.setPosition(wordStartPosition + mUi->guiSearchLine->text().size(), QTextCursor::KeepAnchor);
    mTextEdit->setTextCursor(cursor);
    cursor.endEditBlock();
    setCountAndCurrentPosition();
}

void SearchWidget::highLightedWords(bool highLighted)
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QColor redColor = Qt::red;
    if (highLighted == true) {
        for(int position : mPositionWords) {
            QTextCursor cursor = mTextEdit->textCursor();
            cursor.setPosition(position, QTextCursor::MoveAnchor);
            cursor.setPosition(position + mUi->guiSearchLine->text().size(), QTextCursor::KeepAnchor);

            QTextEdit::ExtraSelection currentWord;
            currentWord.format.setBackground(redColor);
            currentWord.cursor = cursor;
            extraSelections.append(currentWord);
        }
        mTextEdit->setExtraSelections(extraSelections);
    }
    else {
        mTextEdit->setExtraSelections(extraSelections);
    }
}

void SearchWidget::documentChange()
{
    if (mSearchVisible == false) {
        return;
    }
    if (mTextDocument == mTextEdit->toPlainText()) {
        return;
    }
    searchWord(false);
}

void SearchWidget::closeSearch()
{
    emit closeSearchDialog();
}

void SearchWidget::searchLineChanged(const QString & value)
{
    searchWord(true, value);
}

void SearchWidget::searchPositionsWordsInDocument(const QString & searchString, bool selectNext, QTextDocument * document)
{
    if (document == nullptr) {
        document = mTextEdit->document();
    }
    highLightedWords(false);
    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);
    cursor.beginEditBlock();
    while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
        highlightCursor = document->find(searchString, highlightCursor);
        if (!highlightCursor.isNull()) {
            mPositionWords.push_back(highlightCursor.selectionStart());
        }
    }
    highLightedWords(true);
    cursor.endEditBlock();
    setCountAndCurrentPosition();
    if (selectNext) {
        onNextButtonClicked();
    }
}

void SearchWidget::setCountAndCurrentPosition()
{
    bool isEmpty = mPositionWords.empty();
    mUi->guiCountLabel->setVisible(!isEmpty);
    QString str = QString("%1 / %2").arg(mCurrentSelectedWord + 1).arg(mPositionWords.size());
    mUi->guiCountLabel->setText(str);
}
