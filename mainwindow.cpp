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
    QString html = parser.parse(text);
    // Displays the html
    ui->preview->setHtml(html);
}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}
