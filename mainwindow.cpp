#include "mainwindow.h"
#include "./ui_mainwindow.h"

// File handling.
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
// Messages.
#include <QMessageBox>
// PDF export.
#include <QPdfWriter>
#include <QPageSize>
#include <QPageLayout>
#include <QMarginsF>
#include <QPainter>
#include <QTextDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupWindow();
    setupStatusBar();
    setupEditor();
    setupToolBar();
    setupEditActions();
    setupViewActions();
    setupInsertActions();
    setupFileActions();
}

void MainWindow::setupWindow()
{
    setWindowTitle("MarkTex");
}

void MainWindow::setupStatusBar()
{
    // Create the cursor position label.
    cursorPosition = new QLabel("Ln 1, Col 0", this);
    // Add the label to the right side of the status bar.
    ui->statusbar->addPermanentWidget(cursorPosition);
}

void MainWindow::setupEditor()
{
    // Distinguish between editor and preview
    ui->textInput->setPlaceholderText("Start writing in Markdown...");
    // Update preview when text changes.
    connect(ui->textInput, &QTextEdit::textChanged,
            this, &MainWindow::updatePreview);
    // Update the cursor position whenever the cursor moves.
    connect(ui->textInput, &QTextEdit::cursorPositionChanged,
            this, &MainWindow::updateCursorPosition);
    updatePreview();
}

void MainWindow::setupToolBar()
{
    // Undo.
    connect(ui->actionUndo, &QAction::triggered,
            ui->textInput, &QTextEdit::undo);
    // Redo.
    connect(ui->actionRedo, &QAction::triggered,
            ui->textInput, &QTextEdit::redo);
}

void MainWindow::setupEditActions()
{
    // Undo.
    connect(ui->actionUndo2, &QAction::triggered,
            ui->textInput, &QTextEdit::undo);
    // Redo.
    connect(ui->actionRedo2, &QAction::triggered,
            ui->textInput, &QTextEdit::redo);
    // Cut.
    connect(ui->actionCut, &QAction::triggered,
            ui->textInput, &QTextEdit::cut);
    // Copy.
    connect(ui->actionCopy, &QAction::triggered,
            ui->textInput, &QTextEdit::copy);
    // Paste.
    connect(ui->actionPaste, &QAction::triggered,
            ui->textInput, &QTextEdit::paste);
    // Select all.
    connect(ui->actionSelectAll, &QAction::triggered,
            ui->textInput, &QTextEdit::selectAll);
    // Clear text.
    connect(ui->actionClear, &QAction::triggered,
            this, &MainWindow::clearText);
}

void MainWindow::setupViewActions()
{
    // Show Markdown editor.
    connect(ui->actionMarkdown, &QAction::triggered,
            this, &MainWindow::Markdown);
    // Show preview.
    connect(ui->actionPreview, &QAction::triggered,
            this, &MainWindow::Preview);
    // Show both editor and preview.
    connect(ui->actionSplit, &QAction::triggered,
            this, &MainWindow::Split);
    // Zoom in.
    connect(ui->actionZoomIn, &QAction::triggered,
            this, &MainWindow::zoomIn);
    // Zoom out.
    connect(ui->actionZoomOut, &QAction::triggered,
            this, &MainWindow::zoomOut);
    // Reset zoom.
    connect(ui->actionResetZoom, &QAction::triggered,
            this, &MainWindow::resetZoom);
}

void MainWindow::setupInsertActions()
{
    // Bold selected text.
    connect(ui->actionBold, &QAction::triggered,
            this, &MainWindow::insertBold);
    // Italicize selected text.
    connect(ui->actionItalic, &QAction::triggered,
            this, &MainWindow::insertItalic);
}

void MainWindow::setupFileActions()
{
    // Create a new document.
    connect(ui->actionNew, &QAction::triggered,
            this, &MainWindow::newFile);
    // Open an existing document.
    connect(ui->actionOpen, &QAction::triggered,
            this, &MainWindow::openFile);
    // Save the current document.
    connect(ui->actionSave, &QAction::triggered,
            this, &MainWindow::saveFile);
    // Save the document with a new file name.
    connect(ui->actionSaveAs, &QAction::triggered,
            this, &MainWindow::saveFileAs);
    // Export the preview as a PDF.
    connect(ui->actionExportPDF, &QAction::triggered,
            this, &MainWindow::exportPDF);
    // Exit MarkTex.
    connect(ui->actionExit, &QAction::triggered,
            this, &MainWindow::exitApp);
}

void MainWindow::updatePreview()
{
    // Get the text from textInput.
    QString text = ui->textInput->toPlainText();
    // Check if the editor is empty.
    if (text.isEmpty())
    {
        QString text =
            "<p style='font-size: " + QString::number(zoomLevel) +
            "pt;'>Your Markdown preview will appear here.</p>";

        ui->preview->setHtml(text);
        return;
    }
    // Convert the Markdown input to HTML.
    QString html = parser.parse(text);
    // Apply the zoom level to the preview.
    html = "<div style='font-size: " + QString::number(zoomLevel) +
           "pt;'>" + html + "</div>";
    // Display the HTML.
    ui->preview->setHtml(html);
}

void MainWindow::updateCursorPosition()
{
    // Get the current cursor from the text editor.
    QTextCursor cursor = ui->textInput->textCursor();
    // Get the current line number.
    int line = cursor.blockNumber() + 1;
    // Get the current column number.
    int column = cursor.positionInBlock() + 1;
    // Create the status text.
    QString position = "Ln " + QString::number(line)
                       + ", Col " + QString::number(column);
    // Display the cursor position.
    cursorPosition->setText(position);
}

void MainWindow::clearText()
{
    ui->textInput->clear();
}

void MainWindow::Markdown()
{
    ui->textInput->show();
    ui->preview->hide();
}

void MainWindow::Preview()
{
    ui->textInput->hide();
    ui->preview->show();
}

void MainWindow::Split()
{
    ui->textInput->show();
    ui->preview->show();
}

void MainWindow::zoomIn()
{
    zoomLevel += 1;
    QFont font = ui->textInput->font();
    font.setPointSize(zoomLevel);
    ui->textInput->setFont(font);
    updatePreview();
}

void MainWindow::zoomOut()
{
    if (zoomLevel > 6)
    {
        zoomLevel -= 1;
        QFont font = ui->textInput->font();
        font.setPointSize(zoomLevel);
        ui->textInput->setFont(font);
        updatePreview();
    }
}

void MainWindow::resetZoom()
{
    zoomLevel = 12;
    QFont font = ui->textInput->font();
    font.setPointSize(zoomLevel);
    ui->textInput->setFont(font);
    updatePreview();
}

void MainWindow::insertBold()
{
    // Get the current text cursor from the Markdown editor.
    QTextCursor cursor = ui->textInput->textCursor();
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
        ui->textInput->setTextCursor(cursor);
    }
}

void MainWindow::insertItalic()
{
    // Get the current text cursor from the Markdown editor.
    QTextCursor cursor = ui->textInput->textCursor();
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
        ui->textInput->setTextCursor(cursor);
    }
}

void MainWindow::newFile()
{
    // Check if the Markdown editor already contains text.
    if (!ui->textInput->toPlainText().isEmpty())
    {
        // Open a new MarkTex window instead of deleting the current text.
        openNewWindow();
        return;
    }
    // Clear the editor if it is already empty.
    ui->textInput->clear();
    // Clear the current file path.
    currentFile.clear();
    // Reset the window title.
    setWindowTitle("MarkTex");
}

void MainWindow::openFile()
{
    // Open a file selection dialog.
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open Markdown File",
        "",
        "Markdown Files (*.md);;Text Files (*.txt);;All Files (*)"
        );

    // Stop if the user cancels the dialog.
    if (fileName.isEmpty())
    {
        return;
    }
    // Check if the current editor already contains text.
    if (!ui->textInput->toPlainText().isEmpty())
    {
        // Create a new MarkTex window.
        MainWindow *window = new MainWindow();
        // Load the selected file into the new window.
        window->loadFile(fileName);
        // Show the new window.
        window->show();
        return;
    }
    // Load the file into the current window.
    loadFile(fileName);
}

void MainWindow::loadFile(const QString &fileName)
{
    // Create a QFile using the file path.
    QFile file(fileName);
    // Try to open the file for reading.
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Show an error if the file could not be opened.
        QMessageBox::warning(
            this,
            "MarkTex",
            "Could not open the file."
            );

        return;
    }
    // Create a text stream for reading the file.
    QTextStream in(&file);
    // Read the entire file and put it into the Markdown editor.
    ui->textInput->setPlainText(in.readAll());
    // Close the file after reading.
    file.close();
    // Remember the file path.
    currentFile = fileName;
    // Show the file name in the window title.
    setWindowTitle(
        "MarkTex: " + QFileInfo(fileName).fileName()
        );
}

void MainWindow::saveFile()
{
    // If there is no current file, use Save As instead.
    if (currentFile.isEmpty())
    {
        saveFileAs();
        return;
    }
    // Create a QFile using the current file path.
    QFile file(currentFile);
    // Try to open the file for writing.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // Show an error if the file could not be opened.
        QMessageBox::warning(
            this,
            "MarkTex",
            "Could not save the file."
            );

        return;
    }
    // Create a text stream for writing to the file.
    QTextStream out(&file);
    // Write the Markdown editor's contents to the file.
    out << ui->textInput->toPlainText();
    // Close the file after saving.
    file.close();
    // Update the window title.
    setWindowTitle(
        "MarkTex - " + QFileInfo(currentFile).fileName()
        );
}

void MainWindow::saveFileAs()
{
    // Open a file selection dialog for choosing a save location.
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Markdown File",
        "",
        "Markdown Files (*.md);;Text Files (*.txt);;All Files (*)"
        );
    // Stop if the user cancels the dialog.
    if (fileName.isEmpty())
    {
        return;
    }
    // Create a QFile using the selected file path.
    QFile file(fileName);
    // Try to open the file for writing.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // Show an error if the file could not be opened.
        QMessageBox::warning(
            this,
            "MarkTex",
            "Could not save the file."
            );
        return;
    }
    // Create a text stream for writing to the file.
    QTextStream out(&file);
    // Write the Markdown editor's contents to the file.
    out << ui->textInput->toPlainText();
    // Close the file after saving.
    file.close();
    // Remember this as the current file.
    currentFile = fileName;
    // Show the file name in the window title.
    setWindowTitle(
        "MarkTex - " + QFileInfo(currentFile).fileName()
        );
}

void MainWindow::exportPDF()
{
    // Open a dialog so the user can choose where to save the PDF.
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Export PDF",
        "",
        "PDF Files (*.pdf)"
        );
    // Stop if the user cancels the dialog.
    if (fileName.isEmpty())
    {
        return;
    }
    // Make sure the file has a .pdf extension.
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
    {
        fileName += ".pdf";
    }

    // Get the Markdown text from the editor.
    QString markdown = ui->textInput->toPlainText();
    // Stop if there is nothing to export.
    if (markdown.isEmpty())
    {
        QMessageBox::information(
            this,
            "MarkTex",
            "There is no Markdown to export."
            );

        return;
    }
    // use the parsed html
    QString html = parser.parse(markdown);
    // Add PDF-specific formatting.
    html =
        "<html>"
        "<head>"
        "<style>"

        // Normal body text.
        "body {"
        "    font-family: sans-serif;"
        "    font-size: 12pt;"
        "    line-height: 1.5;"
        "}"

        // Heading sizes.
        "h1 {"
        "    font-size: 24pt;"
        "    margin-bottom: 12pt;"
        "}"

        "h2 {"
        "    font-size: 20pt;"
        "    margin-bottom: 10pt;"
        "}"

        "h3 {"
        "    font-size: 16pt;"
        "    margin-bottom: 8pt;"
        "}"

        // Paragraph spacing.
        "p {"
        "    margin-top: 0;"
        "    margin-bottom: 10pt;"
        "}"

        // List spacing.
        "ul, ol {"
        "    margin-top: 0;"
        "    margin-bottom: 10pt;"
        "}"

        "</style>"
        "</head>"
        "<body>"
        + html +
        "</body>"
        "</html>";
    // Create the PDF writer.
    QPdfWriter pdf(fileName);
    // Use 72 DPI so PDF points and painter coordinates match.
    pdf.setResolution(72);
    // Set 1-inch margins.
    QMarginsF margins(72, 72, 72, 72);
    // Create the A4 portrait page layout.
    QPageLayout layout(
        QPageSize(QPageSize::A4),
        QPageLayout::Portrait,
        margins
        );
    // Apply the page layout to the PDF.
    pdf.setPageLayout(layout);
    // Create the text document.
    QTextDocument document;
    // Load the formatted HTML into the document.
    document.setHtml(html);
    // Set the document size to the printable area.
    document.setPageSize(
        pdf.pageLayout().paintRect(QPageLayout::Point).size()
        );
    // Create a painter for the PDF.
    QPainter painter(&pdf);
    // Draw the document onto the PDF.
    document.drawContents(&painter);
    // Finish writing the PDF.
    painter.end();
}

void MainWindow::exitApp()
{
    // Close the MarkTex window.
    close();
}

void MainWindow::openNewWindow()
{
    MainWindow *window = new MainWindow();
    window->show();
}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}