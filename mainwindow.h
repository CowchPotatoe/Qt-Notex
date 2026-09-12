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

    int zoomLevel = 12;
    QLabel *cursorPosition;
    // Markdown parser.
    MarkdownParser parser;

    void setupWindow();
    void setupStatusBar();
    void setupEditor();
    void setupToolBar();
    void setupEditActions();
    void setupViewActions();
    void setupInsertActions();

    void updatePreview();
    void updateCursorPosition();
    void clearText();
    void Markdown();
    void Preview();
    void Split();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void insertBold();
    void insertItalic();
};
#endif // MAINWINDOW_H
