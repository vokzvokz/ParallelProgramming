//
// Created by vladimir on 23.12.2021.
//

#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>

using namespace std;

unsigned int countWordsInString(std::string const& str)
{
    stringstream stream(str);
    return distance(istream_iterator<string>(stream), istream_iterator<std::string>());
}

string read_file(string path){
    std::ifstream t;
    t.open(path);
    std::string buffer;
    std::string line;
    while(t){
        std::getline(t, line);
        buffer = buffer+line;
    }
    t.close();
    return buffer;
}

int count_words(string s){
//    cout<<s<<endl;
    return s.max_size();
}

int main(int argc, char * argv[]){
//    string s = argv[1];
//    cout<<countWordsInString(s)<<endl;

    string s;
//    cout<<argc<<" "<<argv[1]<<" "<<string(argv[2])<<" "<<(string(argv[2]) == "f")<<endl;
    if(string(argv[2])=="f"){
        cout<<"READING FROM FILE"<<endl;
        s = read_file(argv[1]);
    }
    else{
        s = argv[1];
    }
    cout<<countWordsInString(s)<<endl;

    return 0;
}