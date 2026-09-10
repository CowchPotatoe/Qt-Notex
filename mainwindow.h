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

    // Updates the preview when the Markdown text changes.
    void updatePreview();
    // Update cursorPosition
    void updateCursorPosition();
    // Markdown parser.
    MarkdownParser parser;

    // Editor menu stuff
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();
    void clearText();
};
#endif // MAINWINDOW_H
