#ifndef MARKDOWNPARSER_H
#define MARKDOWNPARSER_H
#include <QString>

class MarkdownParser
{
public:
    QString parse(const QString &markdown);

private:
    // Block Markdown
    QString heading(const QString &line, int level);
    QString paragraph(const QString &line);
    QString unorderedList(const QString &line);
    QString orderedList(const QString &line);

    // Inline Markdown
    QString bold(const QString &line);
    QString italic(const QString &line);
};

#endif // MARKDOWNPARSER_H
