#include "markdownparser.h"
#include <QStringList>

QString MarkdownParser::parse(const QString &markdown)
{
    // Split the document into individual lines.
    QStringList lines = markdown.split('\n');
    // Store the generated HTML.
    QString html;
    // Process each line separately.
    for (int i = 0; i < lines.size(); i++)
    {
        QString line = lines[i];
        // Ignore empty lines for now.
        if (line.isEmpty())
        {
            continue;
        }
        // Split the line using a space as the delimiter.
        QStringList words = line.split(' ');
        // Get the first section of the line.
        QString first = words[0];
        // Check for a level 1 heading.
        if (first == "#")
        {
            html += heading(line, 1);
        }
        // Check for a level 2 heading.
        else if (first == "##")
        {
            html += heading(line, 2);
        }
        // Check for a level 3 heading.
        else if (first == "###")
        {
            html += heading(line, 3);
        }
        // Check for an unordered list.
        else if (first == "-")
        {
            // Start the unordered list.
            html += "<ul>";
            // Keep processing lines while they are list items.
            while (i < lines.size())
            {
                QString currentLine = lines[i];
                QStringList currentWords = currentLine.split(' ');
                // Stop when the current line is not a list item.
                if (currentWords[0] != "-")
                {
                    break;
                }
                // Add the list item.
                html += unorderedList(currentLine);
                // Move to the next line.
                i++;
            }
            // End the unordered list.
            html += "</ul>";
            // Move back one because the for loop will increment i.
            i--;
        }
        else if (first.size() > 1 && first[first.size() - 1] == '.')
        {
            // Start the ordered list.
            html += "<ol>";
            // Keep processing lines while they are ordered-list items.
            while (i < lines.size())
            {
                QString currentLine = lines[i];
                QStringList currentWords = currentLine.split(' ');
                QString currentFirst = currentWords[0];
                // Find the period in the list number.
                int period = currentFirst.indexOf('.');
                // Stop if there is no period.
                if (period == -1)
                {
                    break;
                }
                // Convert the first part of the line from a QString to a standard C++ string.
                std::string currentText = currentFirst.toStdString();
                // Extract everything before the period (the list number).
                std::string numberText = currentText.substr(0, period);
                // Convert the extracted number back into a QString.
                QString number = QString::fromStdString(numberText);
                // Check that the number is actually a number.
                bool isNumber;
                number.toInt(&isNumber);
                if (!isNumber)
                {
                    break;
                }
                // Add the list item.
                html += orderedList(currentLine);
                // Move to the next line.
                i++;
            }
            // End the ordered list.
            html += "</ol>";
            // Move back one because the for loop will increment i.
            i--;
        }
        // Anything else is treated as a paragraph.
        else
        {
            html += paragraph(line);
        }
    }

    // Return the completed HTML.
    return html;
}

QString MarkdownParser::heading(const QString &line, int level)
{
    // Convert the QString to a standard C++ string.
    std::string text = line.toStdString();
    // Find the first space separating the Markdown symbol
    // from the heading text.
    size_t space = text.find(' ');
    // Extract everything after the first space.
    std::string headingText = text.substr(space + 1);
    // Convert the extracted text back into a QString.
    QString result = QString::fromStdString(headingText);
    // Store the generated HTML before returning it.
    QString html;
    // Create the appropriate HTML heading.
    if (level == 1)
    {
        html = "<h1>" + result + "</h1>";
    }
    else if (level == 2)
    {
        html = "<h2>" + result + "</h2>";
    }
    else
    {
        html = "<h3>" + result + "</h3>";
    }
    return html;
}

QString MarkdownParser::paragraph(const QString &line)
{
    // Store the original line so we can modify it.
    QString text = line;
    // Check the line for bold Markdown.
    text = bold(text);
    // Check the line for italic Markdown.
    text = italic(text);
    // Put the processed text inside an HTML paragraph.
    QString html = "<div>" + text + "</div>";
    return html;
}

QString MarkdownParser::unorderedList(const QString &line)
{
    // Convert the QString to a standard C++ string.
    std::string text = line.toStdString();
    // Find the first space separating "- " from the list item.
    size_t space = text.find(' ');
    // Extract everything after "- ".
    std::string itemText = text.substr(space + 1);
    // Convert the extracted text back into a QString.
    QString item = QString::fromStdString(itemText);
    // Store the generated HTML before returning it.
    QString html = "<li>" + item + "</li>";
    return html;
}

QString MarkdownParser::orderedList(const QString &line)
{
    // Convert the QString to a standard C++ string.
    std::string text = line.toStdString();
    // Find the period separating the number from the list item.
    size_t period = text.find('.');
    // Extract everything after the period.
    std::string itemText = text.substr(period + 1);
    // Remove the space after the period if there is one.
    if (!itemText.empty() && itemText[0] == ' ')
    {
        itemText = itemText.substr(1);
    }
    // Convert the extracted text back into a QString.
    QString item = QString::fromStdString(itemText);
    // Store the generated HTML before returning it.
    QString html = "<li>" + item + "</li>";
    return html;
}

QString MarkdownParser::bold(const QString &line)
{
    // Convert the QString to a standard C++ string.
    std::string text = line.toStdString();
    // Find the first "**".
    size_t start = text.find("**");
    // Keep looking for bold sections.
    while (start != std::string::npos)
    {
        // Find the closing "**".
        size_t end = text.find("**", start + 2);
        // Stop if there is no closing "**".
        if (end == std::string::npos)
        {
            break;
        }
        // Extract the text before the bold section.
        std::string before = text.substr(0, start);
        // Extract the text inside the bold section.
        std::string boldText = text.substr(start + 2, end - start - 2);
        // Extract the text after the bold section.
        std::string after = text.substr(end + 2);
        // Replace the Markdown with HTML.
        text = before + "<strong>" + boldText + "</strong>" + after;
        // Look for another "**".
        start = text.find("**", start + 8);
    }
    // Convert the result into a QString.
    QString html = QString::fromStdString(text);
    return html;
}

QString MarkdownParser::italic(const QString &line)
{
    // Convert the QString to a standard C++ string.
    std::string text = line.toStdString();
    // Find the first "*".
    size_t start = text.find("*");
    // Keep looking for italic sections.
    while (start != std::string::npos)
    {
        // Find the closing "*".
        size_t end = text.find("*", start + 1);
        // Stop if there is no closing "*".
        if (end == std::string::npos)
        {
            break;
        }
        // Extract the text before the italic section.
        std::string before = text.substr(0, start);
        // Extract the text inside the italic section.
        std::string italicText = text.substr(start + 1, end - start - 1);
        // Extract the text after the italic section.
        std::string after = text.substr(end + 1);
        // Replace the Markdown with HTML.
        text = before + "<em>" + italicText + "</em>" + after;
        // Look for another "*".
        start = text.find("*", start + 4);
    }
    // Convert the result into a QString.
    QString html = QString::fromStdString(text);
    return html;
}