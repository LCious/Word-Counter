#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <regex>

#define URL_PATTERN "(((http|https)://)?www\\.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)"

using namespace std;

void del(string &line){            // elementu nereikalingu trinimas
    set<char> simboliai { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '!', '?', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_', '=', '+', '{', '}', '[', ']', ':', ';', '"', '`', '\\', '|', '\'', '<', '>', ',', '.', '/'};
    for(char simbolis : simboliai){     // trinami simboliai
        line.erase(remove(line.begin(), line.end(), simbolis), line.end());
    };
}

void count(string line, map<string, set<int>> &words, int &number){         // dedami zodziai i mapa
    stringstream ss(line);
    string word;
    while(ss >> word){
        auto word_search = words.find(word);

        if(word_search == words.end()){
            set<int> tempSet;
            tempSet.insert(number);
            words.insert(pair<string, set<int>> (word, tempSet));
        }
        else word_search->second.insert(number);
    }
}

void search_URL(string line, vector<string> &url){  //URL
    stringstream ss(line);
    string word;
    while(ss >> word)
        if(regex_match(word, regex(URL_PATTERN))) url.push_back(word);
}

void print(map<string, set<int>> words, vector<string> url){        // output
    ofstream output("output.txt");
    output << endl << "---------------------------------------------------" << endl << endl
    << "ZODIS" << " ----------- " << "PASIKARTOJA" << " - " << "EILUTES" << " ---------" << endl;
    for(auto it = words.begin(); it != words.end(); it++){      // ZODZIAI PRINT
        if(it->second.size() > 1) {
            output << left << setw(18) << it -> first << setw(14) << it -> second.size();
            for(int i : it -> second)
                output << i << " ";

            output << endl;
        }
    };
    cout<<endl;
    if(url.size() > 0){     //URL PRINT
        output << "------------------- URL ADRESAI: ------------------------" << endl;
        for(int i = 0; i < url.size(); i++){
            output << url[i] << endl;
        }
    }
    output.close();
}

int main(){
    string line;
    int number = 0;
    ifstream input("text.txt");             
    map<string, set<int>> words;
    vector<string> url;
    if(input.is_open()){
        while(getline(input, line)){
            number++;
            search_URL(line, url);
            del(line);           // nereikalingi elementai trinami
            count(line, words, number);     // zodziu skaiciavimo funkcija
        }
        print(words, url);
    }else {
        cout << "Failas nerastas.";
    }
    input.close();
}