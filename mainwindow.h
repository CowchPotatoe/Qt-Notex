#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "markdownparser.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    // Stores the path of the currently open file.
    QString currentFile;
    // Default zoom
    int zoomLevel = 12;
    // cursor
    QLabel *cursorPosition;
    // Markdown parser.
    MarkdownParser parser;

    void setupStatusBar();
    void setupEditor();
    void setupToolBar();
    void setupEditActions();
    void setupViewActions();
    void setupInsertActions();
    void setupFileActions();

    void updatePreview();
    void updateCursorPosition();
    void openNewWindow();
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
    void loadFile(const QString &fileName);
    void saveFile();
    void saveFileAs();
    void exportPDF();
    void exitApp();

};
#endif // MAINWINDOW_H
