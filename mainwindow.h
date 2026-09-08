#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    // used to update the preview
    void updatePreview();
    // Main Markdown parser
    QString markdownToHtml(const QString &markdown);

    // Block-level Markdown
    QString heading(const QString &line, int level);
    QString paragraph(const QString &line);
    QString unorderedList(const QString &line);
    QString orderedList(const QString &line);
    QString codeBlock(const QString &line);

    // Inline Markdown
    QString bold(const QString &line);
    QString italic(const QString &line);
};
#endif // MAINWINDOW_H
