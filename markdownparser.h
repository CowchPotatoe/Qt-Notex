#ifndef MARKDOWNPARSER_H
#define MARKDOWNPARSER_H

#include <QString>

#include <md4qt/parser.h>
#include <md4qt/traits.h>

class MarkdownParser
{
public:
    // Convert Markdown text into HTML.
    QString parse(const QString &text,
                  const QString &fileName = QString());

private:
    // md4qt Markdown parser.
    MD::Parser<MD::QStringTrait> parser;
};

#endif // MARKDOWNPARSER_H