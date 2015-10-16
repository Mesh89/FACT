#ifndef GREEDY_H
#define GREEDY_H
#include "tree.h"
#include <string>
#include <cstring>
#include <stack>
#include <algorithm>
#include <vector>
#define BUCKET_SIZE 60  //This is the word length used for compression
using namespace std;

typedef long long int ll;

tree greedyConsensusSlow();
tree greedyConstruct(vector <string> sol);
bool checkCompatible(vector<vector<ll> > &S, int i,int j,int LENGTH);
bool cp(string a,string b);
//Greedy Consensus Algorithm
tree greedyConsensusFast();

//Comparison function used in sorting
bool cmpLeafSet(int a,int b);

//Determining if two bit vectors denote the same leaf set
bool sameLeafSet(int a,int b);

//Checking whether a certain bit is labelled 1 in the bit vector
bool bitExist(int a,int x);
#endif
