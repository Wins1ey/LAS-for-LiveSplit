#include <cstring>

#include "mainwindow.h"
#include "client.hpp"
#include "downloader.hpp"
#include "autosplitter.hpp"

#include <QApplication>

void launchArgs(int argc, char *argv[])
{
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-downloader") == 0)
        {
            startDownloader(autoSplittersDirectory);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    checkDirectories();
    launchArgs(argc, argv);
    chooseAutoSplitter();
    runAutoSplitter();
    return a.exec();
}

