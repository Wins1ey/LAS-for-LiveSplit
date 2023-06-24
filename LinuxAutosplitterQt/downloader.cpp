#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

#include <curl/curl.h>

#include "downloader.hpp"
#include "lasprint.hpp"
#include "autosplitter.hpp"

using std::cout;
using std::endl;
using std::cin;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::filesystem::create_directory;

string directory;

void startDownloader(string autoSplittersDirectory)
{
    directory = autoSplittersDirectory;
    lasPrint("clear");
    lasPrint("Auto Splitter Downloader\n");
    downloadFile("https://raw.githubusercontent.com/Wins1ey/LuaAutoSplitters/main/autosplitters.csv");

    ifstream file(directory + "/autosplitters.csv");
    string line;
    int i = 1;
    vector<string> gameNamesVector;
    vector<string> urlsVector;
    while (getline(file, line))
    {
        istringstream iss(line);
        string gameName;
        string url;
        getline(iss, gameName, ',');
        getline(iss, url, ',');
        cout << i << ". " + gameName + "\n";
        i++;
        gameNamesVector.push_back(gameName);
        urlsVector.push_back(url);
    }
    file.close();

    cout << "Which auto splitters would you like to download? (Numbers separated by spaces): ";
    vector<int> choiceOfAutoSplitters;
    string input;
    while (cin >> input)
    {
        istringstream iss(input);
        int singleChoice;
        if (iss >> singleChoice && singleChoice > 0 && singleChoice <= gameNamesVector.size())
        {
            choiceOfAutoSplitters.push_back(singleChoice);
        }

        if (cin.peek() == '\n')
        {
            break;
        }
    }
    cin.ignore();

    for (int i = 0; i < choiceOfAutoSplitters.size(); i++)
    {
        int singleChoice = choiceOfAutoSplitters[i];
        if (singleChoice > 0 && singleChoice <= gameNamesVector.size())
        {
            cout << "Downloading " + gameNamesVector[singleChoice - 1] + "'s auto splitter\n";
            downloadFile(urlsVector[singleChoice - 1]);
        }
    }
}

void downloadFile(string url)
{
    CURL *curl;
    CURLcode res;
    string filename = url.substr(url.find_last_of("/"));
    string filepath = directory + filename;

    curl = curl_easy_init();
    if (curl)
    {
        FILE *fp = fopen(filepath.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}
