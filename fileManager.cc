#include "fileManager.hh"



FileManager :: FileManager (int nDocs, int nWords) {
            this->nDocs = nDocs;
            this->nWords = nWords;
            words = vector<string>(nWords);
            docs_name = vector<string>(nDocs);
            docs = vector<string> (nDocs);
            lengths = vector<int> (nDocs);
}
        
bool FileManager :: add_source_file (string file_name) {
    ifstream file (file_name);
    string word;
    if (file.is_open()) {
        for (int i = 0; i < nWords; i++) {
            file >> word;
            words[i] = word;
        }
    }
    else return false;
    
    return true;
    
}

string FileManager :: random_permutation(int actFile) {
    int max = words.size();
    string res = "";
    
    int rNum;
    for (int i = 0; i < nWords; i++) {
        rNum = rand()%max;
        res += words[rNum] + " ";
    }
    lengths[actFile] = res.length();
    docs[actFile] = res;
    return res;
}

bool FileManager :: create_docs() {
    int value_ascii = 97;
    string file_name;
    srand(time(0));
    for (int i = 0; i < nDocs; ++i) {
        file_name = (char)value_ascii;
        file_name += ".txt";
        ofstream file (file_name.c_str());
        if (file.is_open()) {
            string content = random_permutation(i);
            file << content;
            file.close();
        }
        else return false;
        ++value_ascii;
    }
    return true;
}

vector<set<string>> FileManager :: convert_to_shingle (int k) {
    vector<set<string> > shingles(nDocs);
    for (int actDoc = 0; actDoc < nDocs; actDoc++) {
        set<string> act_shingle;
        int n = lengths[actDoc];
        for (int i = 0; i < n-k; i++) {
            string s = "";
            for (int j = i; j <= i+k; ++j) s += docs[actDoc].at(j);
            act_shingle.insert(s);
        }
        shingles[actDoc] = act_shingle;
    }
    return shingles;
}
