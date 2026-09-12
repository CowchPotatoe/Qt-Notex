#include "mainwindow.h"
#include "./ui_mainwindow.h"

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

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}
