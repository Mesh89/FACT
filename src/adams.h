#ifndef ADAMS_H
#define ADAMS_H
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <sstream>
#include "tree.h"
using namespace std;

struct nubTree{
  int N,cnt,sum,root;
  int *leaf,*parent,*mL,*mR,*fenwick,*leaf_array; //leaf_array is 1-indexed
  int *semiroot;
  int *taxa_idx;
  vector<int> *G;
  map <int,int> L_POS;
  nubTree(){}
  nubTree(int _N,int _cnt);
  void update(int x,int d);
  int query(int x);
};
struct retTree{
  vector <int> parent, leaf;
  int root;
};

tree adamsConsensusSlow();
tree adamsRecursionSlow(vector <int> V);
bool cmp(pair<string,int> a,pair<string,int> b);

tree adamsConsensusFast();
retTree adamsRecurse(set <int> *S);
void precomputeLCA();
bool adamsSortCmp(int a,int b);
int get_LCA(int z,int x,int y);
void adamsRadixSort(int *A, vector<int> *S,int L, vector<int> *recurLeaf, int recurDepth);
bool sameSubTree(int a, int b, vector<int> *S);
#endif
