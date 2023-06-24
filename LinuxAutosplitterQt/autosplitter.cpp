#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

#include <lua.hpp>

#include "autosplitter.hpp"
#include "lasprint.hpp"
#include "downloader.hpp"
#include "client.hpp"
#include "readmem.hpp"
#include "gui.hpp"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::sort;
using std::filesystem::directory_iterator;
using std::filesystem::create_directory;
using std::filesystem::exists;
using std::filesystem::is_empty;

lua_State* L = luaL_newstate();

string autoSplittersDirectory;
string chosenAutoSplitter;

gui _gui;

void checkDirectories()
{
    string executablePath;
    string executableDirectory;

    // Get the path to the executable
    char result[ PATH_MAX ];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    executablePath = string(result, (count > 0) ? count : 0);
    executableDirectory = executablePath.substr(0, executablePath.find_last_of("/"));

    autoSplittersDirectory = executableDirectory + "/autosplitters";

    // Make the autosplitters directory if it doesn't exist
    if (!exists(autoSplittersDirectory))
    {#ifndef GUI_HPP
#define GUI_HPP

#include <QFileDialog>
#include <QMessageBox>

            class gui
        {
        public:
            QString fileName;
        };

#endif // GUI_HPP

        create_directory(autoSplittersDirectory);
    }
}

void chooseAutoSplitter()
{
    vector<string> fileNames;

    if (is_empty(autoSplittersDirectory))
    {
        startDownloader(autoSplittersDirectory);
    }

    lasPrint("clear");
    lasPrint("Auto Splitter: ");
    cout << endl;

    for (const auto & entry : directory_iterator(autoSplittersDirectory))
    {
        if (entry.path().extension() == ".lua")
        {
            fileNames.push_back(entry.path().string());
        }
    }
    sort(fileNames.begin(), fileNames.end());

    for (int i = 0; i < fileNames.size(); i++)
    {
        cout << i + 1 << ". " << fileNames[i].substr(fileNames[i].find_last_of("/") + 1) << endl;
    }

    switch (fileNames.size())
    {
    case 0:
    {
        startDownloader(autoSplittersDirectory);
        chooseAutoSplitter();
        return;
    }
    case 1:
    {
        chosenAutoSplitter = fileNames[0];
        break;
    }
    default:
    {
        int userChoice;
        cout << "Which auto splitter would you like to use? ";
        if (!(cin >> userChoice) || userChoice > fileNames.size() || userChoice < 1)
        {
            cin.clear(); // Clear error flags
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
            chooseAutoSplitter(); // Ask for input again
            return;
        }
        cin.ignore();
        chosenAutoSplitter = fileNames[userChoice - 1];
        break;
    }
    }
    lasPrint(chosenAutoSplitter.substr(chosenAutoSplitter.find_last_of("/") + 1) + "\n");
}

void runAutoSplitter()
{
    luaL_openlibs(L);
    lua_pushcfunction(L, findProcessID);
    lua_setglobal(L, "process");
    lua_pushcfunction(L, readAddress);
    lua_setglobal(L, "readAddress");
    lua_pushcfunction(L, sendCommand);
    lua_setglobal(L, "sendCommand");
    lua_pushcfunction(L, luaPrint);
    lua_setglobal(L, "lasPrint");

    luaL_dofile(L, _gui.fileName.toUtf8().constData());
    lua_close(L);
}
