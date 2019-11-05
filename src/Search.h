#pragma once

#include <QWidget>

#include <vector>

class TextEdit;

#include <QTextDocument>

#include <QTextCursor>

namespace Ui {
class SearchForm;
}

class Search : public QWidget
{
        Q_OBJECT

    public:
        explicit Search(TextEdit* textEdit, QWidget *parent=nullptr);
        ~Search();

    public: // General methods
        void initialize(const QString & text);
        void uninitialize();

        void keyPressEvent(QKeyEvent * event);

    public:
    signals:
        void closeSearchDialog(bool value);

    protected slots:
        void onSearchButtonCLicked(bool selectNext = true);
        void onNextButtonClicked();
        void onPreviousButtonClicked();
        void highLightedWords(bool highLighted);
        void documentChange();
        void closeSearch();

    private:
        void searchPositionsWordsInDocument(const QString & searchString,
                                            bool selectNext = true,
                                            QTextDocument * document = nullptr);

        void setCountAndCurrentPosition();

        QTextDocument::FindFlags getFindFlag();

    private: // Members
        Ui::SearchForm *mUi;

        int mCurrentSelectedWord = -1;
        bool mSearchVisible = false;

        std::vector<int> mPositionWords;
        TextEdit* mTextEdit = nullptr;
        QString mTextDocument;
};


