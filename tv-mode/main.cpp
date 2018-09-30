#include <iostream>
#include <string>
#include <sys/types.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include<boost/range/algorithm/count.hpp>
using namespace std;

int printUsage()
{
    cout << "v2 Usage:" << endl << "    tv-mode [tv|desktop]" << endl;
    return 0;
}

string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

bool isProgramRunning(string name)
{
    string result = exec(string("ps aux | grep ").append(name).c_str());
    size_t lines = boost::count(result, '\n');
    return lines > 2;
}

void launchI3(string config)
{
    if(isProgramRunning("i3")) system(string("i3 exit").c_str());
    system(string("i3 --force-xinerama -c ").append(config).append(" &").c_str());
}

void launchFranz()
{
    if(!isProgramRunning("Franz")) system("Franz");
}

void launchBigPicture() {
    system("/usr/games/steam -bigpicture");
}

void setAudioSink(string id)
{
    system(string("pacmd set-default-sink ").append(id).c_str());
}

int tvMode()
{
    setAudioSink("alsa_output.pci-0000_02_00.1.hdmi-stereo-extra1");
    launchI3("~/.i3/tv");
    launchBigPicture();
    return 0;
}

int desktopMode()
{
    setAudioSink("alsa_output.usb-Corsair_Corsair_VOID_Wireless_Gaming_Dongle-00.analog-stereo");
    launchI3("~/.i3/config");
    launchFranz();
    return 0;
}

int enter(string mode)
{
    if(mode == "tv")
        return tvMode();
    else if(mode == "desktop")
        return desktopMode();
    else
        return printUsage();
}

int enterFromKernelParameters()
{
    if(exec("cat /proc/cmdline").find("fernseher") != string::npos)
        return tvMode();
    else
        return desktopMode();
}

int main(int argc, char *argv[])
{
    if(argc > 1) return enter(argv[1]);
    else enterFromKernelParameters();

}
