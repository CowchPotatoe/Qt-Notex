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
    QLabel *cursorPosition;
    // Markdown parser.
    MarkdownParser parser;

    void setupWindow();
    void setupStatusBar();
    void setupEditor();
    void setupToolBar();
    void setupEditActions();
    void setupViewActions();

    void updatePreview();
    void updateCursorPosition();
    void clearText();
    void showMarkdown();
    void showPreview();
    void splitView();
};
#endif // MAINWINDOW_H
