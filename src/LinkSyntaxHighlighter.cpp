#include "LinkSyntaxHighlighter.h"

#include <QRegularExpression>
#include <QGuiApplication>
#include <QPalette>

// These chars are allowed anywhere in the url
#define COMMON_CHARS "-_a-zA-Z0-9/?=&#"

// These chars are not allowed at the end of the url, because they can be used
// as punctuation, are separators in urls or are unlikely to appear at the end of an url.
#define MIDDLE_CHARS ".,;:%+~@"

static const char LINK_REGEX[] = "\\b(https?://|ftp://|file:/)[" COMMON_CHARS MIDDLE_CHARS "]+[" COMMON_CHARS "]";

LinkSyntaxHighlighter::LinkSyntaxHighlighter(QTextDocument *document)
    : QSyntaxHighlighter(document)
{
}

void LinkSyntaxHighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat linkFormat;
    QColor linkColor = QGuiApplication::palette().color(QPalette::Link);
    linkFormat.setForeground(linkColor);
    linkFormat.setUnderlineColor(linkColor);
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
