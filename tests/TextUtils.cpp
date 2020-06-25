#include "TextUtils.h"

#include "TextEdit.h"

#include <optional>

static constexpr char SELECTION_START_CH = '*';
static constexpr char SELECTION_END_CH = '|';

using std::optional;

QString dumpTextEditContent(TextEdit* edit) {
    QString dump = edit->toPlainText();
    int start = edit->textCursor().selectionStart();
    int end = edit->textCursor().selectionEnd();
    int pos = edit->textCursor().position();
    if (start == end) {
        // No selection
        dump.insert(start, SELECTION_END_CH);
        return dump;
    }
    char startCh = SELECTION_START_CH;
    char endCh = SELECTION_END_CH;
    if (pos == start) {
        std::swap(startCh, endCh);
    }
    // Insert the selection indicator chars. Make sure we start from the end
    // one since inserting changes the indices
    dump.insert(end, endCh);
    dump.insert(start, startCh);
    return dump;
}

void setupTextEditContent(TextEdit* edit, const QString& text) {
    QString realText;
    optional<int> start, end;
    int current = 0;
    for (const auto& qChar : text) {
        char ch = qChar.toLatin1();
        switch (ch) {
        case SELECTION_START_CH:
            Q_ASSERT(!start.has_value());
            start = current;
            break;
        case SELECTION_END_CH:
            Q_ASSERT(!end.has_value());
            end = current;
            break;
        default:
            realText += ch;
            ++current;
        }
    }
    edit->setPlainText(realText);
    auto cursor = edit->textCursor();
    Q_ASSERT(end.has_value());
    if (start.has_value()) {
        cursor.setPosition(start.value());
        cursor.setPosition(end.value(), QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(end.value());
    }
    edit->setTextCursor(cursor);
}
