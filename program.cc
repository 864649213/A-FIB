#include "fileManager.hh"

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;


int k, sim_choose, nDocs, nWords;   // user chosen parameters

vector<string> union_kshingle(const vector<set<string>>& k_shingles){
    set<string> result = k_shingles[0];
    int size= k_shingles.size();
    for(int i = 1; i< size; ++i){
        result.insert(k_shingles[i].begin(), k_shingles[i].end());
    }
    vector<string> output(result.size());
    copy(result.begin(), result.end(), output.begin());
    return output;
}

vector<pair<int,int>> hash_funtion(const int& size){
    vector<pair<int,int>> has_function (size);
    srand (time(NULL));
    for(int i = 0; i< size;++i){
        
        has_function[i]= make_pair (rand() %size,rand() % size);
    }
    return has_function;
}

vector<vector<int>> minhash(const vector<set<string>>& k_shingles, vector< pair<int,int> > t){
    vector<string> shingles= union_kshingle(k_shingles);
    int size_hashF = t.size();
    int size_set = shingles.size();
    int k_size= k_shingles.size();
    vector<vector<int>> minhash (size_hashF,vector<int>(k_size));
    for(int h = 0; h<size_hashF; ++h){
        vector<int> permu (size_set);
        for(int i = 0; i < size_set;++i){
            permu[((i*t[h].first)+t[h].second)% size_set] = i;
        }
        for(int i = 0; i<size_set;++i){
            for(int j =0; j < size_set;++j){
                set<string>::iterator elem= k_shingles[j].find(shingles[permu[i]]) ;
                if(elem != k_shingles[j].end())  minhash[j][i] = permu[i];
            }
        }
        
    }
    return minhash;
}


double sim_2_docs(const vector<int>& doc1, const vector<int>& doc2){
    int size = doc1.size();// doc1.size == doc2.size
    int result = 0;
    for(int i = 0; i< size; ++i){
        if(doc1[i] == doc2[i])  ++result;
    }
    return (double)(result/size);
}

vector<vector<double>> sim_k_docs(const vector<set<string>>& k_shingles){
    vector<pair<int,int>> t_functions = hash_funtion(k_shingles.size());
    vector<vector<int>> mhash = minhash(k_shingles,t_functions);
    int size_set = mhash.size();
    //---- init matrix
    vector<vector<double>> result (size_set,vector<double> (size_set));
    for(int i = 0; i<size_set;++i){
            result[i][i]= 1.0;
    }
    vector<double> sim;
    
    for(int i = 0; i < size_set; ++i){
        for(int j =0;j<size_set;++j){
            if(j != i){
                result[i][j]= sim_2_docs(mhash[i],mhash[j]);
            }
        }
    }
    return result;

    
}

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
        for (int j = 0; j < nDocs; ++j) {
            if (i == j) cout << "same ";
            else if (i > j) cout << "xxxx ";
            else cout << setprecision(3) << jsim(shingles[i], shingles[j]) << " ";
            cout << endl;
        }
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
    }else{
        cout<< "This is the approach of Jaccard similarity table" <<endl;
    }
    
    
}
    
    
    
