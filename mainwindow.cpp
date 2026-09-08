#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // change window title
    setWindowTitle("MarkTex");
    // updates in textInput signals changes in the slot Preview
    connect(ui->textInput, &QTextEdit::textChanged, this, &MainWindow::updatePreview);

}

void MainWindow::updatePreview()
{
    // saves the text from textInput
    QString text = ui->textInput->toPlainText();
    // convert the input to Html
    QString html = markdownToHtml(text);
    // Displays the html
    ui->preview->setHtml(html);
}

QString MainWindow::markdownToHtml(const QString &markdown)
{
    // Split the document into individual lines.
    QStringList lines = markdown.split('\n');
    // Store the generated HTML.
    QString html;
    // Process each line separately.
    for (QString line : lines)
    {
        // Ignore empty lines for now.
        if (line.isEmpty())
        {
            continue;
        }
        // Split the line using a space as the delimiter.
        // The first part tells us what type of Markdown element
        // the line represents.
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
        // Check for an unordered list item.
        else if (first == "-")
        {
            html += unorderedList(line);
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

QString MainWindow::heading(const QString &line, int level)
{
    // Convert the QString to a standard C++ string.
    std::string text = line.toStdString();
    // Find the first space separating the Markdown symbol from the heading text.
    size_t space = text.find(' ');
    // Extract everything after the first space.
    std::string headingText = text.substr(space + 1);
    // Convert the extracted text back into a QString.
    QString result = QString::fromStdString(headingText);
    // Store the generated HTML before returning it.
    QString html;
    // Create the appropriate HTML heading based on the heading level.
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

QString MainWindow::paragraph(const QString &line)
{
    // Store the original line so we can modify it.
    QString text = line;
    // Check the line for bold Markdown.
    text = bold(text);
    // Check the line for italic Markdown.
    text = italic(text);
    // Put the processed text inside an HTML paragraph.
    QString html = "<p>" + text + "</p>";
    return html;
}

QString MainWindow::unorderedList(const QString &line)
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
    QString html = "<ul><li>" + item + "</li></ul>";
    return html;
}

QString MainWindow::bold(const QString &line)
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
        // Look for another "**" after the HTML we just created.
        start = text.find("**", start + 8);
    }
    // Convert the result into a QString.
    QString html = QString::fromStdString(text);
    return html;
}

QString MainWindow::italic(const QString &line)
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
        // Look for another "*" after the HTML the initial one
        start = text.find("*", start + 4);
    }
    // Convert the result into a QString.
    QString html = QString::fromStdString(text);
    return html;
}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}
