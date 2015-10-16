#ifndef MAJORITY_H
#define MAJORITY_H
#include "tree.h"
#include "strict.h"
#include "loose.h"
#include <vector>
#include <list>
#include <cstring>
using namespace std;

tree majorityConsensusSlow();
void labelSplits(tree &A,tree &B);
void buildParentChildRelationship(tree &A, vector<int> &parent,vector<int> &splitSize);
tree majorityConsensusFast();
tree majorityRecursion(int a,int b, int *sum);
tree majorityConsensusBest();
tree majContract(tree X);
tree majorityMerge(tree A,tree B);  //A is R(i-1), B is the T(i)

#endif
