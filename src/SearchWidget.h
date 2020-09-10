#pragma once

#include <QTextCursor>
#include <QTextDocument>
#include <QWidget>

#include <memory>
#include <optional>
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
    void selectNextMatch();
    void selectPreviousMatch();
    void onDocumentChanged();
    void selectCurrentMatch();
    void updateCountLabel();
    void highlightMatches();
    void removeHighlights();
    void onSearchLineChanged();
    void search();
    void updateMatchPositions();

    const std::unique_ptr<Ui::SearchForm> mUi;
    TextEdit* const mTextEdit;

    std::vector<int> mMatchPositions;
    // The content of the TextEdit last time we did a search
    QString mPreviousText;
    std::optional<std::size_t> mCurrentMatch;
};
