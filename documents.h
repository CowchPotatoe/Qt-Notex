#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include <QWidget>

#include "markdownparser.h"

class QTextEdit;
class QWebEngineView;
class QSplitter;

class DocumentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentWidget(QWidget *parent = nullptr);

    // Get the Markdown editor.
    QTextEdit *editor() const;

    // Get the Markdown text.
    QString getText() const;

    // Get the parsed HTML.
    QString getHtml();

    // Set the Markdown text.
    void setText(const QString &text);

    // Load a file into this document.
    bool loadFile(const QString &fileName);

    // Save this document to a file.
    bool saveFile(const QString &fileName);

    // Get the path of the current file.
    QString fileName() const;

    // Edit operations.
    void clearText();

    // View operations.
    void showMarkdown();
    void showPreview();
    void showSplit();

    void zoomIn();
    void zoomOut();
    void resetZoom();

    // Theme operations.
    void setDarkMode(bool darkMode);

    // Insert operations.
    void insertBold();
    void insertItalic();

private slots:
    // Update preview when the Markdown changes.
    void updatePreview();

private:
    // Markdown editor.
    QTextEdit *textInput;

    // Markdown preview.
    QWebEngineView *preview;

    // Split the editor and preview horizontally.
    QSplitter *splitter;

    // Markdown parser.
    MarkdownParser parser;

    // Stores the path of the current file.
    QString currentFile;

    // Default zoom.
    int zoomLevel = 12;

    // Current theme.
    bool darkMode = true;

    // Render HTML using the current theme.
    QString renderHtml(const QString &html) const;
};

#endif // DOCUMENTS_H
