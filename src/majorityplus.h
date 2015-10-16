#ifndef MAJORITYPLUS_H
#define MAJORITYPLUS_H
#include <iostream>
#include <cstdio>
#include "tree.h"
#include "strict.h"
#include "majority.h"
using namespace std;

tree majorityPlusConsensus();

void updateCounter(tree &A,tree B);
/*
 * updateCounter(tree &A, tree B): 
 * update clusters in A according to clusters in B
 * Decrement incompatible clusters in A, increment clusters that appear in B
 */

tree majorityPlusMerge(tree A,tree B);  //A is R(i-1), B is the T(i)

/*
 * majorityPlusMerge(tree A,tree B)
 * The counters in B are not changed
 * If the clusters in A are compatible in B, then they are added to B with count 1
 */
#endif
