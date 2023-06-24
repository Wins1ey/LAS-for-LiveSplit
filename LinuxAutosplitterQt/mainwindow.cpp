#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gui.hpp"
#include <QFileDialog>
#include <QMessageBox>


std::string user = std::getenv("USER");
std::string fileDir = "/home/" + user;

gui _gui;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->selectFile, &QAction::triggered, this, &MainWindow::on_selectFile_triggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectFile_triggered()
{
    _gui.fileName = QFileDialog::getOpenFileName(this, tr("Select Lua File"), QString::fromStdString(fileDir), "Lua File (*.lua);;All Files (*.*)");
}
