#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "documents.h"

#include <QApplication>
// File system.
#include <QDir>
#include <QFileSystemModel>
#include <QTreeView>

// Tabs and layout.
#include <QSplitter>
#include <QTabWidget>

// File handling.
#include <QFileDialog>
#include <QFileInfo>

// Messages.
#include <QMessageBox>

// PDF export.
#include <QPdfWriter>
#include <QPageSize>
#include <QPageLayout>
#include <QMarginsF>
#include <QPainter>
#include <QTextDocument>

// Cursor and editor.
#include <QTextCursor>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Create the cursor label before creating the first tab.
    setupStatusBar();
    // Create the file system and tab workspace.
    setupWorkspace();
    // Apply theme
    applyTheme();
    // Create the menu options
    setupToolBar();
    setupEditActions();
    setupViewActions();
    setupInsertActions();
    setupFileActions();
}

void MainWindow::setupWorkspace()
{
    // Create the main horizontal splitter.
    mainSplitter = new QSplitter(Qt::Horizontal);
    // Create the file system tree.
    fileTree = new QTreeView;
    // Create the tab widget.
    tabWidget = new QTabWidget;
    // Add the file system and tabs to the splitter.
    mainSplitter->addWidget(fileTree);
    mainSplitter->addWidget(tabWidget);
    // Allow the file system to be collapsed.
    mainSplitter->setCollapsible(0, true);
    // Keep the tab area visible.
    mainSplitter->setCollapsible(1, false);
    // Give the file system a smaller starting width.
    mainSplitter->setSizes({100, 1000});
    // Let the tab area take most of the available space.
    mainSplitter->setStretchFactor(0, 0);
    mainSplitter->setStretchFactor(1, 1);
    // Use the new workspace as the central widget.
    setCentralWidget(mainSplitter);
    // Create the file system model.
    fileModel = new QFileSystemModel(this);
    // Start the file system at the user's home directory.
    QString rootPath = QDir::homePath();
    // Set the root path for the model.
    fileModel->setRootPath(rootPath);
    // Give the tree the file system model.
    fileTree->setModel(fileModel);
    // Start the tree at the home directory.
    fileTree->setRootIndex(fileModel->index(rootPath));
    // Hide extra file information.
    fileTree->setColumnHidden(1, true);
    fileTree->setColumnHidden(2, true);
    fileTree->setColumnHidden(3, true);
    // Hide the header.
    fileTree->setHeaderHidden(true);
    // Open files from the file system when double-clicked.
    connect(fileTree, &QTreeView::doubleClicked,
            this, &MainWindow::openFromFileSystem);
    // Make the tabs look more like a document editor.
    tabWidget->setDocumentMode(true);
    // Allow tabs to be closed.
    tabWidget->setTabsClosable(true);
    // Allow tabs to be moved.
    tabWidget->setMovable(true);
    // Close a tab when its close button is clicked.
    connect(tabWidget, &QTabWidget::tabCloseRequested,
            this, &MainWindow::closeTab);
    // Update the window when the current tab changes.
    connect(tabWidget, &QTabWidget::currentChanged,
            this, &MainWindow::currentTabChanged);
    // Create the first document tab.
    newTab();
}

void MainWindow::setupStatusBar()
{
    // Create the cursor position label.
    cursorPosition = new QLabel("Ln 1, Col 0", this);
    // Add the label to the right side of the status bar.
    ui->statusbar->addPermanentWidget(cursorPosition);
}

void MainWindow::applyTheme()
{
    if (darkMode)
    {
        // Apply the dark theme to the entire application.
        qApp->setStyleSheet(
            "QMainWindow {"
            "    background-color: #1c1c1c;"
            "    color: #dadada;"
            "}"
            "QWidget {"
            "    background-color: #1c1c1c;"
            "    color: #dadada;"
            "}"
            // Menu bar.
            "QMenuBar {"
            "    background-color: #212121;"
            "    color: #dadada;"
            "}"
            "QMenuBar::item:selected {"
            "    background-color: #282828;"
            "}"
            // Menus.
            "QMenu {"
            "    background-color: #212121;"
            "    color: #dadada;"
            "    border: 1px solid #333333;"
            "}"
            "QMenu::item:selected {"
            "    background-color: #333333;"
            "}"
            // Toolbar.
            "QToolBar {"
            "    background-color: #212121;"
            "    border: none;"
            "    border-bottom: 1px solid #333333;"
            "}"
            "QToolButton {"
            "    background-color: transparent;"
            "    color: #dadada;"
            "}"
            "QToolButton:hover {"
            "    background-color: #282828;"
            "}"
            // File system.
            "QTreeView {"
            "    background-color: #212121;"
            "    color: #dadada;"
            "    border: none;"
            "}"
            "QTreeView::item:hover {"
            "    background-color: #282828;"
            "}"
            "QTreeView::item:selected {"
            "    background-color: #333333;"
            "    color: #ffffff;"
            "}"
            // Tabs.
            "QTabWidget {"
            "    background-color: #1c1c1c;"
            "    border: none;"
            "}"
            "QTabWidget::pane {"
            "    background-color: #1c1c1c;"
            "    border: none;"
            "}"
            "QTabWidget::tab-bar {"
            "    background-color: #212121;"
            "}"
            "QTabBar {"
            "    background-color: #212121;"
            "    border: none;"
            "}"
            "QTabBar::tab {"
            "    background-color: #212121;"
            "    color: #999999;"
            "    padding: 7px 12px;"
            "}"
            "QTabBar::tab:hover {"
            "    background-color: #282828;"
            "    color: #dadada;"
            "}"
            "QTabBar::tab:selected {"
            "    background-color: #1c1c1c;"
            "    color: #dadada;"
            "}"
            // Splitter.
            "QSplitter::handle {"
            "    background-color: #333333;"
            "}"
            "QSplitter::handle:hover {"
            "    background-color: #555555;"
            "}"
            // Status bar.
            "QStatusBar {"
            "    background-color: #212121;"
            "    color: #999999;"
            "    border-top: 1px solid #333333;"
            "}"
            );
    }
    else
    {
        // Apply the light theme to the entire application.
        qApp->setStyleSheet(
            "QMainWindow {"
            "    background-color: #ffffff;"
            "    color: #222222;"
            "}"
            "QWidget {"
            "    background-color: #ffffff;"
            "    color: #222222;"
            "}"
            // Menu bar.
            "QMenuBar {"
            "    background-color: #f6f6f6;"
            "    color: #222222;"
            "}"
            "QMenuBar::item:selected {"
            "    background-color: #eeeeee;"
            "}"

            // Menus.
            "QMenu {"
            "    background-color: #ffffff;"
            "    color: #222222;"
            "    border: 1px solid #dddddd;"
            "}"

            "QMenu::item:selected {"
            "    background-color: #eeeeee;"
            "}"
            // Toolbar.
            "QToolBar {"
            "    background-color: #f6f6f6;"
            "    border: none;"
            "    border-bottom: 1px solid #dddddd;"
            "}"
            "QToolButton {"
            "    background-color: transparent;"
            "    color: #222222;"
            "}"
            "QToolButton:hover {"
            "    background-color: #eeeeee;"
            "}"
            // File system.
            "QTreeView {"
            "    background-color: #f6f6f6;"
            "    color: #222222;"
            "    border: none;"
            "}"

            "QTreeView::item:hover {"
            "    background-color: #eeeeee;"
            "}"

            "QTreeView::item:selected {"
            "    background-color: #dddddd;"
            "    color: #222222;"
            "}"
            // Tabs.
            "QTabWidget {"
            "    background-color: #ffffff;"
            "    border: none;"
            "}"
            "QTabWidget::pane {"
            "    background-color: #ffffff;"
            "    border: 0px;"
            "}"
            "QTabWidget::tab-bar {"
            "    background-color: #f6f6f6;"
            "}"
            "QTabBar {"
            "    background-color: #f6f6f6;"
            "    border: none;"
            "}"
            "QTabBar::tab {"
            "    background-color: #f6f6f6;"
            "    color: #707070;"
            "    padding: 7px 12px;"
            "}"
            "QTabBar::tab:hover {"
            "    background-color: #eeeeee;"
            "    color: #222222;"
            "}"
            "QTabBar::tab:selected {"
            "    background-color: #ffffff;"
            "    color: #222222;"
            "}"
            // Splitter.
            "QSplitter::handle {"
            "    background-color: #dddddd;"
            "}"
            // Status bar.
            "QStatusBar {"
            "    background-color: #f6f6f6;"
            "    color: #707070;"
            "    border-top: 1px solid #dddddd;"
            "}"
            );
    }
    // Apply the same theme to every open document.
    for (int i = 0; i < tabWidget->count(); i++)
    {
        DocumentWidget *document =
            qobject_cast<DocumentWidget *>(
                tabWidget->widget(i)
                );
        if (document != nullptr)
        {
            document->setDarkMode(darkMode);
        }
    }
}
void MainWindow::setupToolBar()
{
    // Undo.
    connect(ui->actionUndo, &QAction::triggered,
            this, [this]()
            {
                DocumentWidget *document = currentDocument();
                if (document != nullptr)
                {
                    document->editor()->undo();
                } });
    // Redo.
    connect(ui->actionRedo, &QAction::triggered,
            this, [this]()
            {
                DocumentWidget *document = currentDocument();

                if (document != nullptr)
                {
                    document->editor()->redo();
                } });
}

void MainWindow::setupEditActions()
{
    // Undo.
    connect(ui->actionUndo2, &QAction::triggered,
            this, [this]()
            {
                DocumentWidget *document = currentDocument();
                if (document != nullptr)
                {
                    document->editor()->undo();
                } });
    // Redo.
    connect(ui->actionRedo2, &QAction::triggered,
            this, [this]()
            {
                DocumentWidget *document = currentDocument();

                if (document != nullptr)
                {
                    document->editor()->redo();
                } });
    // Cut.
    connect(ui->actionCut, &QAction::triggered,
            this, [this]()
            {
                DocumentWidget *document = currentDocument();
                if (document != nullptr)
                {
                    document->editor()->cut();
                } });
    // Copy.
    connect(ui->actionCopy, &QAction::triggered,
            this, [this]()
            {
                DocumentWidget *document = currentDocument();
                if (document != nullptr)
                {
                    document->editor()->copy();
                } });
    // Paste.
    connect(ui->actionPaste, &QAction::triggered,
            this, [this]()
            {
                DocumentWidget *document = currentDocument();
                if (document != nullptr)
                {
                    document->editor()->paste();
                } });
    // Select all.
    connect(ui->actionSelectAll, &QAction::triggered,
            this, [this]()
            {
                DocumentWidget *document = currentDocument();
                if (document != nullptr)
                {
                    document->editor()->selectAll();
                } });
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
    // Toggle dark mode.
    connect(ui->actionDarkMode, &QAction::triggered,
            this, &MainWindow::toggleDarkMode);
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
    // Exit Notex.
    connect(ui->actionExit, &QAction::triggered,
            this, &MainWindow::exitApp);
}

DocumentWidget *MainWindow::currentDocument() const
{
    // Get the document currently displayed in the active tab.
    return qobject_cast<DocumentWidget *>(
        tabWidget->currentWidget());
}

void MainWindow::updateCursorPosition()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document == nullptr)
    {
        return;
    }
    // Get the current cursor from the text editor.
    QTextCursor cursor = document->editor()->textCursor();
    // Get the current line number.
    int line = cursor.blockNumber() + 1;
    // Get the current column number.
    int column = cursor.positionInBlock() + 1;
    // Create the status text.
    QString position = "Ln " + QString::number(line) + ", Col " + QString::number(column);
    // Display the cursor position.
    cursorPosition->setText(position);
}

void MainWindow::clearText()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document != nullptr)
    {
        document->clearText();
    }
}

void MainWindow::Markdown()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document != nullptr)
    {
        document->showMarkdown();
    }
}

void MainWindow::Preview()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document != nullptr)
    {
        document->showPreview();
    }
}

void MainWindow::Split()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document != nullptr)
    {
        document->showSplit();
    }
}

void MainWindow::zoomIn()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document != nullptr)
    {
        document->zoomIn();
    }
}

void MainWindow::zoomOut()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document != nullptr)
    {
        document->zoomOut();
    }
}

void MainWindow::resetZoom()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document != nullptr)
    {
        document->resetZoom();
    }
}

void MainWindow::insertBold()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document != nullptr)
    {
        document->insertBold();
    }
}

void MainWindow::insertItalic()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document != nullptr)
    {
        document->insertItalic();
    }
}

void MainWindow::newFile()
{
    // Create a new tab instead of a new window.
    newTab();
}

void MainWindow::openFile()
{
    // Open a file selection dialog.
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open Markdown File",
        "",
        "Markdown Files (*.md);;Text Files (*.txt);;All Files (*)");

    // Stop if the user cancels the dialog.
    if (fileName.isEmpty())
    {
        return;
    }
    // Open the file in a tab.
    openDocument(fileName);
}

void MainWindow::openDocument(const QString &fileName)
{
    // Check if the file is already open in another tab.
    for (int i = 0; i < tabWidget->count(); i++)
    {
        DocumentWidget *document =
            qobject_cast<DocumentWidget *>(tabWidget->widget(i));
        if (document != nullptr &&
            document->fileName() == fileName)
        {
            // Switch to the tab if the file is already open.
            tabWidget->setCurrentIndex(i);

            return;
        }
    }
    // Get the current document.
    DocumentWidget *current = currentDocument();
    // Reuse the first empty tab when possible.
    if (current != nullptr &&
        current->fileName().isEmpty() &&
        current->getText().isEmpty())
    {
        // Load the file into the current tab.
        if (!current->loadFile(fileName))
        {
            // Show an error if the file could not be opened.
            QMessageBox::warning(
                this,
                "Notex",
                "Could not open the file.");
            return;
        }
        // Use the file name as the tab title.
        QString tabName = QFileInfo(fileName).fileName();
        tabWidget->setTabText(tabWidget->currentIndex(), tabName);
        return;
    }
    // Create a new document for the file.
    DocumentWidget *document = new DocumentWidget;
    // Apply the current theme to the new document.
    document->setDarkMode(darkMode);
    // Load the selected file into the new document.
    if (!document->loadFile(fileName))
    {
        // Show an error if the file could not be opened.
        QMessageBox::warning(this, "Notex", "Could not open the file.");
        delete document;
        return;
    }
    // Use the file name as the tab title.
    QString tabName = QFileInfo(fileName).fileName();
    int index = tabWidget->addTab(document, tabName);
    // Make the newly opened document active.
    tabWidget->setCurrentIndex(index);
    // Update the cursor position whenever the cursor moves.
    connect(document->editor(), &QTextEdit::cursorPositionChanged,
            this, &MainWindow::updateCursorPosition);
}

void MainWindow::openFromFileSystem(const QModelIndex &index)
{
    // Stop if the selected item is a directory.
    if (fileModel->isDir(index))
    {
        return;
    }
    // Get the path of the selected file.
    QString fileName = fileModel->filePath(index);
    // Open the file in a tab.
    openDocument(fileName);
}

void MainWindow::newTab()
{
    // Create a new document.
    DocumentWidget *document = new DocumentWidget;
    // Add the document as a new tab.
    int index = tabWidget->addTab(document, "Untitled");
    // Make the new tab active.
    tabWidget->setCurrentIndex(index);
    // Update the cursor position whenever the cursor moves.
    connect(document->editor(), &QTextEdit::cursorPositionChanged,
            this, &MainWindow::updateCursorPosition);
    // Reset the window title for an untitled document.
    setWindowTitle("Notex");
    // Apply the current theme to the new document.
    document->setDarkMode(darkMode);
}

void MainWindow::saveFile()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document == nullptr)
    {
        return;
    }
    // If there is no current file, use Save As instead.
    if (document->fileName().isEmpty())
    {
        saveFileAs();
        return;
    }
    // Save the current document.
    if (!document->saveFile(document->fileName()))
    {
        // Show an error if the file could not be saved.
        QMessageBox::warning(this, "Notex", "Could not save the file.");
        return;
    }
    // Update the tab title.
    QString tabName = QFileInfo(document->fileName()).fileName();
    tabWidget->setTabText(tabWidget->currentIndex(), tabName);
}

void MainWindow::saveFileAs()
{
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document == nullptr)
    {
        return;
    }
    // Open a file selection dialog for choosing a save location.
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Markdown File",
        "",
        "Markdown Files (*.md);;Text Files (*.txt);;All Files (*)");
    // Stop if the user cancels the dialog.
    if (fileName.isEmpty())
    {
        return;
    }
    // Add .md if the user did not provide an extension.
    if (!fileName.endsWith(".md", Qt::CaseInsensitive))
    {
        fileName += ".md";
    }
    // Save the document.
    if (!document->saveFile(fileName))
    {
        // Show an error if the file could not be saved.
        QMessageBox::warning(
            this,
            "Notex",
            "Could not save the file.");
        return;
    }
    // Update the tab title.
    QString tabName = QFileInfo(fileName).fileName();
    tabWidget->setTabText(tabWidget->currentIndex(), tabName);
}

void MainWindow::closeTab(int index)
{
    // Get the widget inside the tab.
    QWidget *widget = tabWidget->widget(index);
    // Remove the tab.
    tabWidget->removeTab(index);
    // Delete the document.
    delete widget;
    // Keep one tab open.
    if (tabWidget->count() == 0)
    {
        newTab();
    }
}

void MainWindow::currentTabChanged(int index)
{
    if (index < 0)
    {
        return;
    }
    // Get the currently selected document.
    DocumentWidget *document = currentDocument();
    if (document == nullptr)
    {
        return;
    }
    // Update the cursor position.
    updateCursorPosition();
}

void MainWindow::exportPDF()
{
    // Open a dialog so the user can choose where to save the PDF.
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Export PDF",
        "",
        "PDF Files (*.pdf)");
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
    // Get the current document.
    DocumentWidget *document = currentDocument();
    if (document == nullptr)
    {
        return;
    }
    // Get the Markdown text from the editor.
    QString markdown = document->getText();
    // Stop if there is nothing to export.
    if (markdown.isEmpty())
    {
        QMessageBox::information(
            this,
            "Notex",
            "There is no Markdown to export.");
        return;
    }
    // use the parsed html
    QString html = document->getHtml();
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
        "<body>" +
        html +
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
        margins);
    // Apply the page layout to the PDF.
    pdf.setPageLayout(layout);
    // Create the text document.
    QTextDocument documentPDF;
    // Load the formatted HTML into the document.
    documentPDF.setHtml(html);
    // Set the document size to the printable area.
    documentPDF.setPageSize(pdf.pageLayout().paintRect(QPageLayout::Point).size());
    // Create a painter for the PDF.
    QPainter painter(&pdf);
    // Draw the document onto the PDF.
    documentPDF.drawContents(&painter);
    // Finish writing the PDF.
    painter.end();
}

void MainWindow::exitApp()
{
    // Close the Notex window.
    close();
}

void MainWindow::toggleDarkMode()
{
    // Switch between dark and light mode.
    darkMode = !darkMode;
    // Apply the new theme.
    applyTheme();
    // Change the menu option to show the next available mode.
    if (darkMode)
    {
        ui->actionDarkMode->setText("Light Mode");
    }
    else
    {
        ui->actionDarkMode->setText("Dark Mode");
    }
}
// destructor
MainWindow::~MainWindow()
{
    delete ui;
}