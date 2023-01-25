#ifndef LINKSYNTAXHIGHLIGHTER_H
#define LINKSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class LinkSyntaxHighlighter : public QSyntaxHighlighter {
public:
    LinkSyntaxHighlighter(QTextDocument* document);

    static QUrl getLinkAt(const QString& text, int position);
    static int getTaskCheckmarkPosAt(const QString& text, int position);

protected:
    void highlightBlock(const QString& text) override;
};

#endif // LINKSYNTAXHIGHLIGHTER_H
