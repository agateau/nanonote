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
    mUi->previousButton->setArrowType(Qt::UpArrow);
    mUi->previousButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mUi->nextButton->setArrowType(Qt::DownArrow);
    mUi->nextButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QObject::connect(mUi->nextButton, &QToolButton::clicked, this, &SearchWidget::onNextButtonClicked);
    QObject::connect(mUi->previousButton, &QToolButton::clicked, this, &SearchWidget::onPreviousButtonClicked);
    QObject::connect(mTextEdit, &TextEdit::textChanged, this, &SearchWidget::documentChange);
    QObject::connect(mUi->searchLine, &QLineEdit::textChanged, this, &SearchWidget::searchLineChanged);
    QObject::connect(mUi->closeButton, &QToolButton::clicked, this, &SearchWidget::closeSearch);
}

SearchWidget::~SearchWidget()
{
    delete mUi;
}

void SearchWidget::initialize(const QString & text)
{
    mSearchVisible = true;
    mUi->searchLine->setFocus();
    mUi->searchLine->setText(text);
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
        searchValue = mUi->searchLine->text();
    }
    mTextDocument = mTextEdit->toPlainText();
    mCurrentSelectedWord = -1;
    mPositionWords.clear();
    searchPositionsWordsInDocument(searchValue, selectNext);
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
    cursor.setPosition(wordStartPosition + mUi->searchLine->text().size(), QTextCursor::KeepAnchor);
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
    cursor.setPosition(wordStartPosition + mUi->searchLine->text().size(), QTextCursor::KeepAnchor);
    mTextEdit->setTextCursor(cursor);
    cursor.endEditBlock();
    setCountAndCurrentPosition();
}

void SearchWidget::highLightedWords(bool highLighted)
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QColor highlightColor = Qt::yellow;
    if (highLighted) {
        for(int position : mPositionWords) {
            QTextCursor cursor = mTextEdit->textCursor();
            cursor.setPosition(position, QTextCursor::MoveAnchor);
            cursor.setPosition(position + mUi->searchLine->text().size(), QTextCursor::KeepAnchor);

            QTextEdit::ExtraSelection currentWord;
            currentWord.format.setBackground(highlightColor);
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
    if (!mSearchVisible) {
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

void SearchWidget::searchPositionsWordsInDocument(const QString & searchString, bool selectNext)
{
    QTextDocument * document = mTextEdit->document();
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
    mUi->countLabel->setVisible(!isEmpty);
    QString str = QString("%1 / %2").arg(mCurrentSelectedWord + 1).arg(mPositionWords.size());
    mUi->countLabel->setText(str);
}
