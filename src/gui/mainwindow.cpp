#include "mainwindow.h"

#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow()
{
    setWindowTitle(WINDOW_TITLE);
    setWindowIcon(QIcon(":/icons/app-icon.png"));
    resize(1280, 720);
    // Center the window on the primary screen
    move(QGuiApplication::primaryScreen()->availableGeometry().center() - this->rect().center());
}