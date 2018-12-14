#include <iostream>
#include <string>
#include <set>

using namespace std;

int find_union(set<string> doc1 ,set<string> doc2){
    set<string> result = doc1;
    result.insert(doc2.begin(), doc2.end());
    return result.size();
    
}
int find_intersection(set<string> doc1, set<string> doc2){
    int intersect;
    set<string>::iterator it;
    for(it = doc1.begin(); it!= doc1.end();++it){
        if(doc2.find(*it) != doc2.end())   ++intersect;
    }
    return intersect;
}


double jsim(set<string> doc1, set<string> doc2){
    
    double result =(find_intersection(doc1,doc2)/find_union(doc1,doc2));
    return result;
}

