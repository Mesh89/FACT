#ifndef STRICT_H
#define STRICT_H
#include "tree.h"
#include <stack>
#include <cstring>
#include <vector>
#include <ctime>
using namespace std;

//Constructs the Strict Consensus Tree
tree strictConsensus();

//Runs Days algorithm on Trees A and B. Updates a boolean array in A that denotes which internal nodes in B exist in A.
//Note that the algorithm assume that information in A is already precomputed using the A.precompute() function
void days(tree &A,tree &B);

//Constructs the induced subgraph of the marked nodes by removing all the unmarked nodes.
tree contract(tree A);


#endif
