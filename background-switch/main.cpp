#include <iostream> 
#include <dirent.h>
#include <vector>
#include <string>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

int printUsage()
{
    cout << "Usage:" << endl <<"    background-switch <directory> <tag|all> [gnome]";
    return 0;
}

int printNoFilesFound()
{
    cout << "No files with the given tag were found." << endl;
    return 0;
}

vector<string> getFiles(DIR* dir, char tag[])
{
    vector<string> files;

    dirent *ent;
    while((ent = readdir(dir)) != 0)
    {
        if(string(tag) == "all" || string(ent->d_name).find(tag) != string::npos )
            files.push_back(ent->d_name);
    }

    return files;
}

string getRandomFile(vector<string> files)
{
    srand(time(0) ^ getpid());
    int element = rand() % files.size();

    return files[element];
}

int main(int argc, char *argv[])
{
    if(argc < 3) return printUsage();
    char* directory = argv[1];
    char* tag = argv[2];
    bool isGnome = sizeof(argv) >= 4 && string(argv[3]) == "gnome";

    DIR* dirStream = opendir(directory);
    if(dirStream == 0) return printUsage();
    vector<string> files = getFiles(dirStream, tag);
    closedir(dirStream);

    if(files.size() == 0) return printNoFilesFound();

    string file = getRandomFile(files);
    string fullFileDir = string(directory).append("/").append(file);

    string command = !isGnome ? string("feh --bg-fill \"").append(fullFileDir).append("\"") : string("gsettings set org.gnome.desktop.background picture-uri \"file:\/\/").append(fullFileDir).append("\"");
    cout << "Executing " << command << endl;
    system(command.c_str());

    return 0;
}
