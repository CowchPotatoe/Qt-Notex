#include "markdownparser.h"

#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>

#include <md4qt/html.h>

QString MarkdownParser::parse(const QString &text,
                              const QString &fileName)
{
    // Store the Markdown text in a QString.
    QString markdown = text;

    // Create a text stream from the Markdown text.
    QTextStream stream(&markdown, QIODevice::ReadOnly);

    // Path and file name are used by md4qt.
    QString path;
    QString name = "Untitled.md";

    // Use the actual file path when the document has been saved.
    if (!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);

        path = fileInfo.absolutePath();
        name = fileInfo.fileName();
    }

    // Parse the Markdown text.
    auto document = parser.parse(stream, path, name, true);

    // Convert the parsed document into HTML.
    QString html = MD::toHtml(document);

    return html;
}