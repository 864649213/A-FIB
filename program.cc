#include "fileManager.hh"

#include <iostream>
#include <vector>
#include <chrono>

using namespace std;


int k, sim_choose, nDocs, nWords;   // user chosen parameters


int find_union(set<string> doc1 ,set<string> doc2){
    set<string> result = doc1;
    result.insert(doc2.begin(), doc2.end());
    return result.size();
    
}
int find_intersection(set<string> doc1, set<string> doc2){
    int intersect = 0;
    set<string>::iterator it;
    for(it = doc1.begin(); it!= doc1.end();++it){
        if(doc2.find(*it) != doc2.end()) ++intersect;
    }
    return intersect;
}


double jsim(set<string> doc1, set<string> doc2){
    double result = (double)find_intersection(doc1,doc2)/(double)find_union(doc1,doc2);
    return result;
}

void menu () {
    cout << "Enter the number of random documents to create" << endl;
    cin >> nDocs;
    cout << "Enter the number of words you want to generate for each document" << endl;
    cin >> nWords;
    cout << "Enter the parameter k of k-shingle, min 1, max 50 due to constraint of length of the document" << endl;
    cin >> k;
    cout << "Enter the way you want to calculate the similarity:\n 1) Jaccard similarity\n 2) Signature minhash aproximation" << endl;
    cin >> sim_choose;
}

void write_jacard(vector<set<string> > &shingles) {
    for (int i = 0; i < nDocs; ++i) {
        for (int j = 0; j < nDocs; ++j) cout << jsim(shingles[i], shingles[j]) << " ";
    cout << endl;
    }
    
}
 

int main () {
    
    menu();
    
    vector<vector<double> > tabla (nDocs, vector<double>(nDocs));     //  table of all similarities
    vector<set<string> > shingles;
    
    FileManager fileManager(nDocs, nWords);
    auto start_time = chrono::high_resolution_clock::now();
    if (not fileManager.add_source_file("words.txt")) {
        cout << "Error at reading the document that contains the words" << endl;
        return 0;
    }
    cout << "Read done!" << endl;
    if (not fileManager.create_docs()) {
        cout << "Error at creating the files" << endl;
        return 0;
    }
    cout << "Creatiion done!" << endl;
    auto end_time = chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    cout << "The file managing process took: " << chrono::duration_cast<chrono::microseconds>(time).count() << " us" << endl;
    
    shingles = fileManager.convert_to_shingle(k);
    
    
    if (sim_choose == 1) {
        cout << "This is the Jaccard similarity table:" << endl;
        start_time = chrono::high_resolution_clock::now();
        write_jacard(shingles);
        end_time = chrono::high_resolution_clock::now();
        time = end_time - start_time;
        cout << "The Jaccard similarity calculation process took: " << chrono::duration_cast<chrono::microseconds>(time).count() << " us" << endl;
    }
    
    
}
    
    
    
