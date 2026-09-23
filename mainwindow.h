#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QFileSystemModel;
class QSplitter;
class QTabWidget;
class QTreeView;
class DocumentWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    // File system model used by the file tree.
    QFileSystemModel *fileModel;
    // File system tree.
    QTreeView *fileTree;
    // Main splitter between the file system and tabs.
    QSplitter *mainSplitter;
    // Tab widget containing open documents.
    QTabWidget *tabWidget;
    // cursor
    QLabel *cursorPosition;

    void setupWorkspace();
    void setupStatusBar();
    void setupToolBar();
    void setupEditActions();
    void setupViewActions();
    void setupInsertActions();
    void setupFileActions();

    // Get the document currently open in the active tab.
    DocumentWidget *currentDocument() const;

    void updateCursorPosition();

    // Edit operations
    void clearText();
    // View operations
    void Markdown();
    void Preview();
    void Split();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    // Insert operations
    void insertBold();
    void insertItalic();
    // File operations.
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void exportPDF();
    void exitApp();
    // Tab operations.
    void newTab();
    void openDocument(const QString &fileName);
    void openFromFileSystem(const QModelIndex &index);
    void closeTab(int index);
    void currentTabChanged(int index);

    // Theme operations.
    void toggleDarkMode();
    void applyTheme();
    // Current theme.
    bool darkMode = false;
};

#endif // MAINWINDOW_H