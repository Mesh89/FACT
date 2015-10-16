#ifndef LOOSE_H
#define LOOSE_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include "tree.h"
#include "greedy.h"
#include "strict.h"
using namespace std;

typedef long long int ll;

tree looseConsensusSlow();
bool cmpLeafSetLOOSE(int a,int b);
bool sameLeafSetLOOSE(int a,int b);

//The loose consensus algorithm
tree looseConsensusFast();

//The compatible function that finds out which vertices in A are compatible with B. Depending on the parameter op, it either returns A with the compatible vertices marked, or returns a tree that contains all vertices in B and all vertices in A that are compatible with B.
tree looseMerge(tree A, tree B,bool op);

#endif
