#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QTextEdit* m_text;
    QString current_file;
public:
    MainWindow(QWidget *parent = nullptr);
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void saveWarning();
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onExit();
};
#endif // MAINWINDOW_H
