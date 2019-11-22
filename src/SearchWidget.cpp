#include "SearchWidget.h"
#include "ui_SearchForm.h"

#include <iostream>

#include "TextEdit.h"

SearchWidget::SearchWidget(TextEdit* textEdit, QWidget* parent)
        : QWidget(parent), mUi(new Ui::SearchForm), mTextEdit(textEdit) {
    mUi->setupUi(this);
    layout()->setContentsMargins(0, 0, 0, 0);

    connect(mUi->nextButton, &QToolButton::clicked, this, &SearchWidget::selectNextMatch);
    connect(mUi->previousButton, &QToolButton::clicked, this, &SearchWidget::selectPreviousMatch);
    connect(mTextEdit, &TextEdit::textChanged, this, &SearchWidget::onDocumentChanged);
    connect(mUi->searchLine, &QLineEdit::textChanged, this, &SearchWidget::onSearchLineChanged);
    connect(mUi->closeButton, &QToolButton::clicked, this, &SearchWidget::closeClicked);
    connect(mUi->searchLine, &QLineEdit::returnPressed, this, &SearchWidget::selectNextMatch);
}

SearchWidget::~SearchWidget() {
}

void SearchWidget::initialize(const QString& text) {
    mSearchVisible = true;
    mUi->searchLine->setFocus();
    mUi->searchLine->setText(text);
    search();
    selectNextMatch();
}

void SearchWidget::uninitialize() {
    mSearchVisible = false;
    removeHighlights();
}

void SearchWidget::search() {
    mTextDocument = mTextEdit->toPlainText();
    mCurrentMatch = -1;

    QTextCursor cursor(mTextEdit->document());
    cursor.beginEditBlock();

    removeHighlights();
    updateMatchPositions();
    highlightMatches();

    cursor.endEditBlock();
    setCountAndCurrentPosition();
}

void SearchWidget::selectNextMatch() {
    if (mMatchPositions.empty()) {
        return;
    }
    mCurrentMatch = (mCurrentMatch + 1) % mMatchPositions.size();
    selectCurrentMatch();
}

void SearchWidget::selectPreviousMatch() {
    if (mMatchPositions.empty()) {
        return;
    }
    if (mCurrentMatch != 0) {
        mCurrentMatch--;
    } else {
        mCurrentMatch = int(mMatchPositions.size()) - 1;
    }
    selectCurrentMatch();
}

void SearchWidget::highlightMatches() {
    QList<QTextEdit::ExtraSelection> extraSelections;
    QColor highlightColor = Qt::yellow;
    for (int position : mMatchPositions) {
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

void SearchWidget::removeHighlights() {
    mTextEdit->setExtraSelections({});
}

void SearchWidget::onDocumentChanged() {
    if (!mSearchVisible) {
        return;
    }
    if (mTextDocument == mTextEdit->toPlainText()) {
        return;
    }
    search();
}

void SearchWidget::onSearchLineChanged() {
    search();
    selectNextMatch();
}

void SearchWidget::updateMatchPositions() {
    auto* document = mTextEdit->document();
    QString searchString = mUi->searchLine->text();

    mMatchPositions.clear();
    QTextCursor cursor(document);
    while (!cursor.isNull() && !cursor.atEnd()) {
        cursor = document->find(searchString, cursor);
        if (!cursor.isNull()) {
            mMatchPositions.push_back(cursor.selectionStart());
        }
    }
}

void SearchWidget::selectCurrentMatch() {
    QTextDocument* document = mTextEdit->document();
    QTextCursor cursor(document);
    cursor.beginEditBlock();
    int startPosition = mMatchPositions.at(mCurrentMatch);
    cursor.setPosition(startPosition, QTextCursor::MoveAnchor);
    cursor.setPosition(startPosition + mUi->searchLine->text().size(), QTextCursor::KeepAnchor);
    mTextEdit->setTextCursor(cursor);
    cursor.endEditBlock();
    setCountAndCurrentPosition();
}

void SearchWidget::setCountAndCurrentPosition() {
    bool isEmpty = mMatchPositions.empty();
    mUi->countLabel->setVisible(!isEmpty);
    QString str = QString("%1 / %2").arg(mCurrentMatch + 1).arg(mMatchPositions.size());
    mUi->countLabel->setText(str);
}
