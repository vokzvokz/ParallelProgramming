#include <string>
#include <iostream>
#include <fstream>

using namespace std;

string read_file(const string& path){
    std::ifstream t;
    t.open(path);
    std::string buffer;
    std::string line;
    while(t){
        std::getline(t, line);
        buffer += "\n"+line;
    }
    t.close();
    return buffer;
}

int count_words(string s){
    int i = 0;
    int counter = 0;
    cout<<"START COUNTING WORDS IN LINE: "<<s<<endl;
    while(s[i]!='\0'){
        while(s[i]==' ' || s[i]=='\t' || s[i]=='\n'){
            i++;
            if(s[i]=='\0') return counter;
        }
        counter++;
        while(s[i]!=' ' && s[i]!='\t' && s[i]!='\n'){
            i++;
            if(s[i]=='\0') return counter;
        }
    }
    return counter;
}

int main(int argc, char* argv[]){
//    string s = argv[1];
//    cout<<countWordsInString(s)<<endl;
//    cout<<argc<<" "<<argv[0]<<" "<<argv[1]<<endl;
    string s;
//    cout<<argc<<" "<<argv[1]<<" "<<string(argv[2])<<" "<<(string(argv[2]) == "f")<<endl;
    if(argc == 2){
        s = argv[1];
    }
    else if(string(argv[2])=="f"){
        cout<<"READING FROM FILE"<<endl;
        s = read_file(argv[1]);
    }
    else{
        cout<<"Invalid nubmer of arguments"<<endl;
        return -1;
    }
    cout<<count_words(s)<<endl;

    return 0;
}