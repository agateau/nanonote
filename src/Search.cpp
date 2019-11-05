#include "Search.h"
#include "ui_SearchForm.h"

#include <iostream>

#include "TextEdit.h"

Search::Search(TextEdit* textEdit, QWidget *parent )
    : QWidget( parent )
    , mUi(new Ui::SearchForm)
    , mTextEdit(textEdit)
{
    mUi->setupUi(this);

    QObject::connect(mUi->guiNextButton, SIGNAL(clicked(bool)), this, SLOT(onNextButtonClicked()));
    QObject::connect(mUi->guiPreviousButton, SIGNAL(clicked(bool)), this, SLOT(onPreviousButtonClicked()));
    QObject::connect(mUi->guiHighlightedButton, SIGNAL(toggled(bool)), this, SLOT(highLightedWords(bool)));
    QObject::connect(mTextEdit, SIGNAL(textChanged()), this, SLOT(documentChange()));
    QObject::connect(mUi->guiCaseSensitive, SIGNAL(toggled(bool)), this, SLOT(onSearchButtonCLicked()));
    QObject::connect(mUi->guiWholeWords, SIGNAL(toggled(bool)), this, SLOT(onSearchButtonCLicked()));
    QObject::connect(mUi->guiSearchLine, SIGNAL(textChanged(QString)), this, SLOT(onSearchButtonCLicked()));
    QObject::connect(mUi->guiCloseButton, SIGNAL(clicked()), this, SLOT(closeSearch()));

    mUi->guiCountLabel->setText(tr("- / -"));
}

Search::~Search()
{
    delete mUi;
}

void Search::initialize(const QString & text)
{
    mSearchVisible = true;

    mUi->guiSearchLine->setFocus();
    mUi->guiSearchLine->setText(text);
    onSearchButtonCLicked();
}

void Search::uninitialize()
{
    mSearchVisible = false;

    highLightedWords(false);
}


void Search::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        onNextButtonClicked();
    }
}

void Search::onSearchButtonCLicked(bool selectNext)
{
    mTextDocument = mTextEdit->toPlainText();

    mCurrentSelectedWord = -1;
    mPositionWords.clear();

    QString searchString = mUi->guiSearchLine->text();
    QTextDocument *document = mTextEdit->document();

    searchPositionsWordsInDocument(searchString, selectNext, document);
}

void Search::onNextButtonClicked()
{
    if(mPositionWords.empty())
    {
        return;
    }

    if(mCurrentSelectedWord != ((int)mPositionWords.size() - 1))
    {
        mCurrentSelectedWord++;
    }
    else
    {
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

void Search::onPreviousButtonClicked()
{
    if(mPositionWords.empty())
    {
        return;
    }

    if(mCurrentSelectedWord != 0)
    {
        mCurrentSelectedWord--;
    }
    else
    {
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

void Search::highLightedWords(bool highLighted)
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QColor redColor = Qt::red;

    if(highLighted == true)
    {
        for(int position : mPositionWords)
        {
            QTextCursor cursor = mTextEdit->textCursor();

            if (mUi->guiWholeWords->isChecked())
            {
                cursor.setPosition(position);
                cursor.select(QTextCursor::WordUnderCursor);
            }
            else
            {
                cursor.setPosition(position, QTextCursor::MoveAnchor);
                cursor.setPosition(position + mUi->guiSearchLine->text().size(), QTextCursor::KeepAnchor);
            }

            QTextEdit::ExtraSelection currentWord;
            currentWord.format.setBackground(redColor);
            currentWord.cursor = cursor;
            extraSelections.append(currentWord);
        }

        mTextEdit->setExtraSelections(extraSelections);
    }
    else
    {
        mTextEdit->setExtraSelections(extraSelections);
    }
}

void Search::documentChange()
{
    if(mSearchVisible == false)
    {
        return;
    }

    if(mTextDocument == mTextEdit->toPlainText())
    {
        return;
    }

    onSearchButtonCLicked(false);
}

void Search::closeSearch()
{
    emit closeSearchDialog(false);
}

void Search::searchPositionsWordsInDocument(const QString & searchString, bool selectNext, QTextDocument * document)
{
    if (document == nullptr)
    {
        document = mTextEdit->document();
    }

    highLightedWords(false);

    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
        highlightCursor = document->find(searchString, highlightCursor, getFindFlag());

        if (!highlightCursor.isNull())
        {
            mPositionWords.push_back(highlightCursor.selectionStart());
        }
    }

    highLightedWords(mUi->guiHighlightedButton->isChecked());
    cursor.endEditBlock();

    setCountAndCurrentPosition();

    if(selectNext)
    {
        onNextButtonClicked();
    }
}

void Search::setCountAndCurrentPosition()
{
    if(mPositionWords.empty())
    {
        mUi->guiCountLabel->setText(tr("- / -"));
    }
    else
    {
        QString str = QString("%1 / %2").arg(mCurrentSelectedWord + 1).arg(mPositionWords.size());
        mUi->guiCountLabel->setText(str);
    }
}

QTextDocument::FindFlags Search::getFindFlag()
{
    QTextDocument::FindFlags flags;

    if (mUi->guiWholeWords->isChecked())
    {
        flags |= QTextDocument::FindWholeWords;
    }

    if (mUi->guiCaseSensitive->isChecked())
    {
        flags |= QTextDocument::FindCaseSensitively;
    }

    return flags;
}

