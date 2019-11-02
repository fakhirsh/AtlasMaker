
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>

#include <png.h>
#include <GuillotineBinPack.h>

#include "../../Common/Texture.hpp"

using namespace std;
using namespace FEngine;

struct IMAGE{
    IMAGE(){
        x = y = -1;
        width = height = -1;
        rotation = false;
    }
    string name;
    int x, y;
    int width, height;
    bool rotation;
};


vector<string> lines;
vector<IMAGE> rects;

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
 
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        string l = buffer.data();
        result += trim(l);
        lines.push_back(l);
    }
    return result;
}
 
int main(int argc, char ** argv)
{

    if(argc != 4){
        cout << "Usage: ./SpriteSheetMaker [WIDTH] [HEIGHT] [SPRITE_DIRECTORY]" << endl;
        return -1;
    }
    
    int IMG_WIDTH = std::stoi( argv[1] );
    int IMG_HEIGHT = std::stoi( argv[2] );
    
    cout << "Output image Width: " << IMG_WIDTH << " -- Height: " << IMG_HEIGHT << endl;    
    
    string command = "python3 listdir.py " + string(argv[3]);
    string result = exec(command.c_str());
    
    if(result == ""){
        cout << "Error: No PNG files found to begin with !" << endl;
        return -1;
    }
    
    for(auto it = lines.begin(); it != lines.end(); ++it){
        Texture tex;
        if(tex.LoadFromFile(*it)){
            tex.SetName(*it);
            IMAGE r;
            r.name = *it;
            r.x = 0;
            r.y = 0;
            r.width = tex.GetWidth();
            r.height = tex.GetHeight();
            rects.push_back(r);
        }
        else{
            cout << "Failed" << endl;
        }
    }
       
    
    for(auto it = rects.begin(); it != rects.end(); ++it){
        IMAGE r = *it;
        cout << r.name << " --- " << r.width << " : " << r.height << endl;
    }
    
    
    
    return 0;
}
