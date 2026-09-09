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
    // Undo.
    connect(ui->undoButton, &QAction::triggered,
            ui->textInput, &QTextEdit::undo);
    // Redo.
    connect(ui->redoButton, &QAction::triggered,
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

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}
