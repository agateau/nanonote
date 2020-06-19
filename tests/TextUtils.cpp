#include "TextUtils.h"

#include "TextEdit.h"

#include <optional>

using std::optional;

QString dumpTextEditContent(TextEdit* edit) {
    QString dump = edit->toPlainText();
    int start = edit->textCursor().selectionStart();
    int end = edit->textCursor().selectionEnd();
    int pos = edit->textCursor().position();
    if (start == end) {
        // No selection
        dump.insert(start, '{');
        return dump;
    }
    char startCh = '{';
    char endCh = '}';
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
    optional<int> begin, end;
    int current = 0;
    for (const auto& qChar : text) {
        char ch = qChar.toLatin1();
        switch (ch) {
        case '{':
            Q_ASSERT(!begin.has_value());
            begin = current;
            break;
        case '}':
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
    Q_ASSERT(begin.has_value());
    cursor.setPosition(begin.value());
    if (end.has_value()) {
        cursor.setPosition(end.value(), QTextCursor::KeepAnchor);
    }
    edit->setTextCursor(cursor);
}
