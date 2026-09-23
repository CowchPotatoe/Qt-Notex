#include "documents.h"

#include <QFile>
#include <QFont>
#include <QSplitter>
#include <QTextBrowser>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>

DocumentWidget::DocumentWidget(QWidget *parent)
    : QWidget(parent)
{
    // Create the Markdown editor.
    textInput = new QTextEdit(this);
    // Create the Markdown preview.
    preview = new QTextBrowser(this);
    // Distinguish between editor and preview.
    textInput->setPlaceholderText("Start writing in Markdown...");
    // Show the preview placeholder when the document first opens.
    preview->setHtml("<p>Your Markdown preview will appear here.</p>");
    // Create a horizontal splitter.
    splitter = new QSplitter(Qt::Horizontal, this);
    // Add the editor and preview to the splitter.
    splitter->addWidget(textInput);
    splitter->addWidget(preview);
    // Keep the editor and preview at equal widths.
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    // Start with the editor and preview at equal widths.
    splitter->setSizes({500, 500});
    // Create the layout for this document.
    QVBoxLayout *layout = new QVBoxLayout(this);
    // Remove extra space around the splitter.
    layout->setContentsMargins(0, 0, 0, 0);
    // Add the splitter to the document.
    layout->addWidget(splitter);
    // Update preview when text changes.
    connect(textInput, &QTextEdit::textChanged,
            this, &DocumentWidget::updatePreview);
    // Start with dark mode.
    setDarkMode(false);
}

QTextEdit *DocumentWidget::editor() const
{
    // Return this document's Markdown editor.
    return textInput;
}

QString DocumentWidget::getText() const
{
    // Return the Markdown text.
    return textInput->toPlainText();
}

QString DocumentWidget::getHtml()
{
    // Convert the Markdown input to HTML.
    return parser.parse(getText());
}

void DocumentWidget::setText(const QString &text)
{
    // Set the Markdown text.
    textInput->setPlainText(text);
}

bool DocumentWidget::loadFile(const QString &fileName)
{
    // Create a QFile using the file path.
    QFile file(fileName);
    // Try to open the file for reading.
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    // Create a text stream for reading the file.
    QTextStream in(&file);
    // Read the entire file and put it into the Markdown editor.
    textInput->setPlainText(in.readAll());
    // Close the file after reading.
    file.close();
    // Remember the file path.
    currentFile = fileName;
    // The document has been loaded from the file.
    textInput->document()->setModified(false);
    return true;
}

bool DocumentWidget::saveFile(const QString &fileName)
{
    // Create a QFile using the file path.
    QFile file(fileName);
    // Try to open the file for writing.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }
    // Create a text stream for writing to the file.
    QTextStream out(&file);
    // Write the Markdown editor's contents to the file.
    out << textInput->toPlainText();
    // Close the file after saving.
    file.close();
    // Remember the file path.
    currentFile = fileName;
    // The document has been saved.
    textInput->document()->setModified(false);
    return true;
}

QString DocumentWidget::fileName() const
{
    // Return the current file path.
    return currentFile;
}

void DocumentWidget::clearText()
{
    // Clear the Markdown editor.
    textInput->clear();
}

void DocumentWidget::showMarkdown()
{
    // Show only the Markdown editor.
    textInput->show();
    preview->hide();
}

void DocumentWidget::showPreview()
{
    // Show only the preview.
    textInput->hide();
    preview->show();
}

void DocumentWidget::showSplit()
{
    // Show both the editor and preview.
    textInput->show();
    preview->show();
    // Reset the editor and preview to equal widths.
    int half = splitter->width() / 2;
    splitter->setSizes({half, half});
}

void DocumentWidget::zoomIn()
{
    // Increase the zoom level.
    zoomLevel += 1;
    // Change the editor font size.
    QFont font = textInput->font();
    font.setPointSize(zoomLevel);
    textInput->setFont(font);
    // Update the preview.
    updatePreview();
}

void DocumentWidget::zoomOut()
{
    // Prevent the font from becoming too small.
    if (zoomLevel > 6)
    {
        // Decrease the zoom level.
        zoomLevel -= 1;
        // Change the editor font size.
        QFont font = textInput->font();
        font.setPointSize(zoomLevel);
        textInput->setFont(font);
        // Update the preview.
        updatePreview();
    }
}

void DocumentWidget::resetZoom()
{
    // Reset the zoom level.
    zoomLevel = 12;
    // Reset the editor font size.
    QFont font = textInput->font();
    font.setPointSize(zoomLevel);
    textInput->setFont(font);
    // Update the preview.
    updatePreview();
}

void DocumentWidget::insertBold()
{
    // Get the current text cursor from the Markdown editor.
    QTextCursor cursor = textInput->textCursor();
    // Check if the user has selected any text.
    if (cursor.hasSelection())
    {
        // Get the selected text.
        QString selectedText = cursor.selectedText();
        // Replace the selected text with Markdown bold syntax.
        cursor.insertText("**" + selectedText + "**");
    }
    else
    {
        // Insert an empty pair of bold markers.
        cursor.insertText("****");
        // Move the cursor left twice so it is between the markers.
        cursor.movePosition(QTextCursor::Left);
        cursor.movePosition(QTextCursor::Left);
        // Update the editor's cursor to the new position.
        textInput->setTextCursor(cursor);
    }
}

void DocumentWidget::insertItalic()
{
    // Get the current text cursor from the Markdown editor.
    QTextCursor cursor = textInput->textCursor();
    // Check if the user has selected any text.
    if (cursor.hasSelection())
    {
        // Get the selected text.
        QString selectedText = cursor.selectedText();
        // Replace the selected text with Markdown italic syntax.
        cursor.insertText("*" + selectedText + "*");
    }
    else
    {
        // Insert an empty pair of italic markers.
        cursor.insertText("**");
        // Move the cursor left once so it is between the markers.
        cursor.movePosition(QTextCursor::Left);
        // Update the editor's cursor to the new position.
        textInput->setTextCursor(cursor);
    }
}

void DocumentWidget::updatePreview()
{
    // Get the text from textInput.
    QString text = textInput->toPlainText();

    // Check if the editor is empty.
    if (text.isEmpty())
    {
        preview->setHtml(
            renderHtml(
                "<p>Your Markdown preview will appear here.</p>"
                )
            );
        return;
    }
    // Convert the Markdown input to HTML.
    QString html = parser.parse(text);
    // Apply the zoom level to the preview.
    html = "<div style='font-size: " +
           QString::number(zoomLevel) +
           "pt;'>" +
           html +
           "</div>";
    // Apply the current theme.
    html = renderHtml(html);
    // Display the HTML.
    preview->setHtml(html);
}


void DocumentWidget::setDarkMode(bool darkMode)
{
    // Store the current theme.
    this->darkMode = darkMode;
    // Dark theme.
    if (darkMode)
    {
        textInput->setStyleSheet(
            "QTextEdit {"
            "    background-color: #1c1c1c;"
            "    color: #dadada;"
            "    border: 1px solid #333333;"
            "    selection-background-color: #483b5c;"
            "    selection-color: #ffffff;"
            "}"
            );
        preview->setStyleSheet(
            "QTextBrowser {"
            "    background-color: #1c1c1c;"
            "    color: #dadada;"
            "    border: 1px solid #333333;"
            "}"
            );
    }
    // Light theme.
    else
    {
        textInput->setStyleSheet(
            "QTextEdit {"
            "    background-color: #ffffff;"
            "    color: #222222;"
            "    border: 1px solid #dddddd;"
            "    selection-background-color: #d9cfff;"
            "    selection-color: #222222;"
            "}"
            );
        preview->setStyleSheet(
            "QTextBrowser {"
            "    background-color: #ffffff;"
            "    color: #222222;"
            "    border: 1px solid #dddddd;"
            "}"
            );
    }
    // Re-render the preview using the new theme.
    updatePreview();
}

QString DocumentWidget::renderHtml(const QString &html) const
{
    QString css;

    // Dark theme.
    if (darkMode)
    {
        css =
            "<style>"
            "body {"
            "    background-color: #1c1c1c;"
            "    color: #dadada;"
            "    font-family: sans-serif;"
            "}"

            "h1, h2, h3, h4, h5, h6 {"
            "    color: #f0f0f0;"
            "}"

            "a {"
            "    color: #a882ff;"
            "}"

            "code {"
            "    background-color: #282828;"
            "    color: #dadada;"
            "}"

            "pre {"
            "    background-color: #282828;"
            "    color: #dadada;"
            "    padding: 10px;"
            "}"

            "blockquote {"
            "    color: #b3b3b3;"
            "    border-left: 3px solid #a882ff;"
            "    padding-left: 10px;"
            "}"

            "hr {"
            "    border: 0;"
            "    border-top: 1px solid #333333;"
            "}"
            "</style>";
    }
    // Light theme.
    else
    {
        css =
            "<style>"
            "body {"
            "    background-color: #ffffff;"
            "    color: #222222;"
            "    font-family: sans-serif;"
            "}"

            "h1, h2, h3, h4, h5, h6 {"
            "    color: #222222;"
            "}"

            "a {"
            "    color: #7852ee;"
            "}"

            "code {"
            "    background-color: #f2f2f2;"
            "    color: #222222;"
            "}"

            "pre {"
            "    background-color: #f2f2f2;"
            "    color: #222222;"
            "    padding: 10px;"
            "}"

            "blockquote {"
            "    color: #666666;"
            "    border-left: 3px solid #7852ee;"
            "    padding-left: 10px;"
            "}"

            "hr {"
            "    border: 0;"
            "    border-top: 1px solid #dddddd;"
            "}"
            "</style>";
    }
    // Add the CSS around the parsed HTML.
    QString renderedHtml =
        "<html>"
        "<head>"
        + css +
        "</head>"
        "<body>"
        + html +
        "</body>"
        "</html>";

    return renderedHtml;
}