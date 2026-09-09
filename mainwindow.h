#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    // Updates the preview when the Markdown text changes.
    void updatePreview();

    // Markdown parser.
    MarkdownParser parser;
};
#endif // MAINWINDOW_H
