#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // change window title
    setWindowTitle("MarkTex");
    // Update preview when text changes.
    connect(ui->textInput, &QTextEdit::textChanged,
            this, &MainWindow::updatePreview);

    // Create the cursor position label.
    cursorPosition = new QLabel("Ln 1, Col 0", this);
    // Add the label to the right side of the status bar.
    ui->statusbar->addPermanentWidget(cursorPosition);
    // Update the cursor position whenever the cursor moves.
    connect(ui->textInput, &QTextEdit::cursorPositionChanged,
            this, &MainWindow::updateCursorPosition);

    // Undo.
    connect(ui->undoAction, &QAction::triggered,
            ui->textInput, &QTextEdit::undo);
    // Redo.
    connect(ui->redoAction, &QAction::triggered,
            ui->textInput, &QTextEdit::redo);
}

void MainWindow::updatePreview()
{
    // saves the text from textInput
    QString text = ui->textInput->toPlainText();
    // convert the input to Html
    QString html = parser.parse(text);
    // Displays the html
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

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}
