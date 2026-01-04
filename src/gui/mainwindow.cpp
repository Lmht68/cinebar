#include "mainwindow.h"
#include "../utils/constants.h"

#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow()
{
    setWindowTitle(WINDOW_TITLE);
    setWindowIcon(QIcon(ResourcePath::APP_ICON));
    resize(1280, 720);
    // Center the window on the primary screen
    move(QGuiApplication::primaryScreen()->availableGeometry().center() - this->rect().center());
}