#include "LinkSyntaxHighlighter.h"

#include <QRegularExpression>

static const char LINK_REGEX[] = "\\bhttp[s]?://[-_a-zA-Z.0-9/?=&]+";

LinkSyntaxHighlighter::LinkSyntaxHighlighter(QTextDocument *document)
    : QSyntaxHighlighter(document)
{
}

void LinkSyntaxHighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat linkFormat;
    linkFormat.setForeground(Qt::blue);
    linkFormat.setUnderlineColor(Qt::blue);
    linkFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);

    QRegularExpression expression(LINK_REGEX);
    for (auto it = expression.globalMatch(text); it.hasNext();) {
        QRegularExpressionMatch match = it.next();
        setFormat(match.capturedStart(), match.capturedLength(), linkFormat);
    }
}

QUrl LinkSyntaxHighlighter::getLinkAt(const QString &text, int position)
{
    QRegularExpression expression(LINK_REGEX);
    for (auto it = expression.globalMatch(text); it.hasNext();) {
        QRegularExpressionMatch match = it.next();
        if (match.capturedStart() <= position && position < match.capturedEnd()) {
            return QUrl::fromUserInput(match.captured());
        }
    }
    return QUrl();
}
