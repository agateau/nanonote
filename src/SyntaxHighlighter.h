#ifndef LINKSYNTAXHIGHLIGHTER_H
#define LINKSYNTAXHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter {
public:
    SyntaxHighlighter(QTextDocument* document);

    static QUrl getLinkAt(const QString& text, int position);
    static int getTaskCheckmarkPosAt(const QString& text, int position);

protected:
    void highlightBlock(const QString& text) override;

private:
    const QRegularExpression mLinkRegex;
    const QRegularExpression mTaskRegex;
    const QRegularExpression mHeadingRegex;
};

#endif // LINKSYNTAXHIGHLIGHTER_H
