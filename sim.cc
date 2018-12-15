#include <iostream>
#include <string>
#include <set>

using namespace std;



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
    int size_shingles = shingles.size();
    int ndoc= k_shingles.size();
    vector<vector<int>> minhash (ndoc,vector<int>(size_hashF));
    for(int h = 0; h<size_hashF; ++h){
        vector<int> permu (size_shingles);
        for(int i = 0; i < size_shingles;++i){
            permu[((i*t[h].first)+t[h].second)% size_shingles] = i;
        }
        for(int i = 0; i<ndoc;++i){
            for(int j =0; j < size_shingles;++j){
                set<string>::iterator elem= k_shingles[i].find(shingles[permu[j]]) ;
                if(elem != k_shingles[i].end())  minhash[i][h] = permu[j];
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
    vector<vector<int>> mhash = minhash(k_shingles, hash_funtion(k_shingles.size()));
    int ndocs = k_shingles.size();
    //---- init matrix
    vector<vector<double>> result (ndocs,vector<double> (ndocs));
    for(int i = 0; i<ndocs;++i){
            result[i][i]= 1.0;
    }
    
    for(int i = 0; i < ndocs; ++i){
        for(int j =0;j<ndocs;++j){
            if(j != i){
                result[i][j]= sim_2_docs(mhash[i],mhash[j]);
            }
        }
    }
    return result;

    
}
