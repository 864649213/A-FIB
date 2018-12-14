#ifndef FILE_MANAGER_HH
#define FILE_MANAGER_HH

#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <set>

using namespace std;

using namespace std;

class FileManager{
    
    private:
        int nDocs;
        int nWords;
        vector<string> words;
        vector<string> docs_name;
        vector<string> docs;
        vector<int> lengths;
        
        string random_permutation(int actFile);
    
    public:
        
        FileManager (int nDocs, int nWords);
        bool add_source_file (string file_name);
        bool create_docs();
        vector<set<string> > convert_to_shingle (int k);
        

};

#endif
