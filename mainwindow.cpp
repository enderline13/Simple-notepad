#include "mainwindow.h"
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QMessageBox>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_text(new QTextEdit), current_file("")
{
    m_text->setFontPointSize(15);
    m_text->setStyleSheet("font: 15pt \"Consolas\";");
    m_text->setFrameStyle(QFrame::NoFrame);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_text);
    layout->setContentsMargins(7, 0, 10, 0);
    QWidget* widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

    menuBar()->setStyleSheet("QMenuBar::item { border: 1px solid black; padding: 5px 10px;}");
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->setStyleSheet("QMenu { border: 1px solid black; }");
    fileMenu->setStyleSheet("QMenu::item { "
                            "padding: 5px 10px; "
                            "border: 1px solid black; "
                            "margin: 3px; "
                            "}");
    QAction *newAction = fileMenu->addAction(tr("New"), QKeySequence::New);
    QAction *openAction = fileMenu->addAction(tr("Open"), QKeySequence::Open);
    QAction *saveAction = fileMenu->addAction(tr("Save"), QKeySequence::Save);
    QAction *saveAsAction = fileMenu->addAction(tr("Save as"), QKeySequence::SaveAs);
    QAction *exitAction = fileMenu->addAction(tr("Exit"));

    connect(newAction, &QAction::triggered, this, &MainWindow::onNew);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpen);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSave);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onSaveAs);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExit);
}

void MainWindow::onNew() {
    if(current_file != "") {
        saveWarning();
    }
    current_file = "";
    m_text->clear();
    setWindowTitle("");
}
void MainWindow::onOpen(){
    if(current_file != "") {
        saveWarning();
    }
    QString name = QFileDialog::getOpenFileName(this, "Open text file", "Downloads", "Text files (*.txt)");
    QFile file(name);
    if(file.open(QIODevice::ReadOnly)) {
        current_file = name;
        setWindowTitle(current_file);
        QTextStream stream(&file);
        this->m_text->setPlainText(stream.readAll());
    }
    file.close();
}
void MainWindow::onSave(){
    if(current_file != "") {
        QFile file(current_file);
        if(file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << m_text->toPlainText();
            file.close();
        }
    }
    else onSaveAs();
}
void MainWindow::onSaveAs(){
    QString newDir = QFileDialog::getSaveFileName(this, "Save file", current_file, "Text files (*.txt)");
    QFile file(newDir);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << m_text->toPlainText();
        current_file = newDir;
        setWindowTitle(current_file);
        file.close();
    }
}
void MainWindow::onExit(){
    saveWarning();
    QApplication::exit();
}

void MainWindow::saveWarning() {
    int ret = QMessageBox::warning(this, tr("My Application"),
                                   tr("Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard,
                                   QMessageBox::Save);

    switch (ret) {
    case QMessageBox::Save:
        onSave();
        break;
    case QMessageBox::Discard:
        break;
    default:

        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveWarning();
    QWidget::closeEvent(event);
}
