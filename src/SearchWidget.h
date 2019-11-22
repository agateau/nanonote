#pragma once

#include <QTextCursor>
#include <QTextDocument>
#include <QWidget>

#include <memory>
#include <vector>

class TextEdit;

namespace Ui {
class SearchForm;
}

class SearchWidget : public QWidget {
    Q_OBJECT

public:
    explicit SearchWidget(TextEdit* textEdit, QWidget* parent = nullptr);
    ~SearchWidget();

    void initialize(const QString& text);
    void uninitialize();

signals:
    void closeClicked();

private:
    void onNextButtonClicked();
    void onPreviousButtonClicked();
    void onDocumentChanged();
    void selectWord();
    void setCountAndCurrentPosition();
    void highLightedWords(bool highLighted);
    void onSearchLineChanged(const QString& value);
    void searchWord(bool selectNext = true, QString searchValue = "");
    void searchPositionsWordsInDocument(const QString& searchString, bool selectNext = true);

    const std::unique_ptr<Ui::SearchForm> mUi;
    TextEdit* const mTextEdit;

    std::vector<int> mPositionWords;
    QString mTextDocument;
    bool mSearchVisible = false;
    int mCurrentSelectedWord = -1;
};
