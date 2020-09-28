#include "SearchWidget.h"
#include "ui_SearchWidget.h"

#include <iostream>

#include "TextEdit.h"

SearchWidget::SearchWidget(TextEdit* textEdit, QWidget* parent)
        : QWidget(parent), mUi(new Ui::SearchWidget), mTextEdit(textEdit) {
    mUi->setupUi(this);
    layout()->setContentsMargins(0, 0, 0, 0);
    setFocusProxy(mUi->lineEdit);

    mUi->countLabel->hide();

    mUi->closeButton->setToolTip(tr("Close search bar"));

    connect(mUi->nextButton, &QToolButton::clicked, this, &SearchWidget::selectNextMatch);
    connect(mUi->previousButton, &QToolButton::clicked, this, &SearchWidget::selectPreviousMatch);
    connect(mTextEdit, &TextEdit::textChanged, this, &SearchWidget::onDocumentChanged);
    connect(mUi->lineEdit, &QLineEdit::textChanged, this, &SearchWidget::onLineEditChanged);
    connect(mUi->closeButton, &QToolButton::clicked, this, &SearchWidget::closeClicked);
    connect(mUi->lineEdit, &QLineEdit::returnPressed, this, &SearchWidget::selectNextMatch);
}

SearchWidget::~SearchWidget() {
}

void SearchWidget::initialize(const QString& text) {
    mUi->lineEdit->setFocus();
    bool textChanged = mUi->lineEdit->text() != text;
    mUi->lineEdit->setText(text);
    if (!textChanged) {
        search();
    }
}

void SearchWidget::uninitialize() {
    removeHighlights();
}

void SearchWidget::search() {
    mPreviousText = mTextEdit->toPlainText();

    QTextCursor cursor(mTextEdit->document());
    cursor.beginEditBlock();

    removeHighlights();
    updateMatchPositions();
    highlightMatches();

    cursor.endEditBlock();
    updateLineEdit();
    updateCountLabel();
}

void SearchWidget::selectNextMatch() {
    if (mMatchPositions.empty()) {
        return;
    }
    int minPosition = mTextEdit->textCursor().selectionStart();
    auto it = std::find_if(mMatchPositions.begin(),
                           mMatchPositions.end(),
                           [minPosition](int position) { return position > minPosition; });
    mCurrentMatch = it != mMatchPositions.end() ? std::distance(mMatchPositions.begin(), it) : 0;
    selectCurrentMatch();
}

void SearchWidget::selectPreviousMatch() {
    if (mMatchPositions.empty()) {
        return;
    }
    int maxPosition = mTextEdit->textCursor().selectionStart();
    auto it = std::find_if(mMatchPositions.rbegin(),
                           mMatchPositions.rend(),
                           [maxPosition](int position) { return position < maxPosition; });

    if (it == mMatchPositions.rend()) {
        mCurrentMatch = mMatchPositions.size() - 1;
    } else {
        // rlast is the first element of mMatchPosition
        auto rlast = std::prev(mMatchPositions.rend());
        mCurrentMatch = std::distance(it, rlast);
    }
    selectCurrentMatch();
}

void SearchWidget::highlightMatches() {
    QList<QTextEdit::ExtraSelection> extraSelections;
    QColor highlightColor = Qt::yellow;
    for (int position : mMatchPositions) {
        QTextCursor cursor = mTextEdit->textCursor();
        cursor.setPosition(position, QTextCursor::MoveAnchor);
        cursor.setPosition(position + mUi->lineEdit->text().size(), QTextCursor::KeepAnchor);

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
    if (!isVisible()) {
        return;
    }
    // When we highlight the search matches, documentChanged() is emitted. Compare current text with
    // the previous content and do not restart a search in this case, to prevent endless recursions.
    // This is not optimal, it would probably be better to use a syntax highlighter for matches, but
    // this is good enough for now.
    if (mPreviousText == mTextEdit->toPlainText()) {
        return;
    }
    search();
}

void SearchWidget::onLineEditChanged() {
    search();
    if (mCurrentMatch.has_value()) {
        selectCurrentMatch();
    }
}

void SearchWidget::updateMatchPositions() {
    auto* document = mTextEdit->document();
    QString searchString = mUi->lineEdit->text();

    mMatchPositions.clear();
    QTextCursor cursor(document);
    while (!cursor.isNull() && !cursor.atEnd()) {
        cursor = document->find(searchString, cursor);
        if (!cursor.isNull()) {
            mMatchPositions.push_back(cursor.selectionStart());
        }
    }
    if (mMatchPositions.empty()) {
        mCurrentMatch.reset();
    } else {
        mCurrentMatch = 0;
    }
}

void SearchWidget::selectCurrentMatch() {
    QTextDocument* document = mTextEdit->document();
    QTextCursor cursor(document);
    cursor.beginEditBlock();
    int startPosition = mMatchPositions.at(mCurrentMatch.value());
    cursor.setPosition(startPosition, QTextCursor::MoveAnchor);
    cursor.setPosition(startPosition + mUi->lineEdit->text().size(), QTextCursor::KeepAnchor);
    mTextEdit->setTextCursor(cursor);
    cursor.endEditBlock();
    updateCountLabel();
}

void SearchWidget::updateCountLabel() {
    if (mCurrentMatch.has_value()) {
        mUi->countLabel->show();
        QString str = QString("%1 / %2").arg(mCurrentMatch.value() + 1).arg(mMatchPositions.size());
        mUi->countLabel->setText(str);
    } else {
        mUi->countLabel->hide();
    }
}

static QColor mixColors(const QColor& c1, const QColor& c2, qreal k) {
    return QColor::fromRgbF(c1.redF() * (1 - k) + c2.redF() * k,
                            c1.greenF() * (1 - k) + c2.greenF() * k,
                            c1.blueF() * (1 - k) + c2.blueF() * k);
}

void SearchWidget::updateLineEdit() {
    static QPalette noMatchPalette = [this] {
        auto palette = mUi->lineEdit->palette();
        auto baseColor = palette.color(QPalette::Base);
        palette.setColor(QPalette::Base, mixColors(baseColor, Qt::red, 0.3));
        return palette;
    }();
    bool noMatch = mMatchPositions.empty() && !mUi->lineEdit->text().isEmpty();
    mUi->lineEdit->setPalette(noMatch ? noMatchPalette : palette());
}
