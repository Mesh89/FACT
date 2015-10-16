#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <stack>
#include <cassert>
#include <cstring>
#include "wrapper.h"
using namespace std;

inline bool is_numeric(char c);
struct tree{
  int N; //No. of leaves
  int cnt; // No. of nodes
  int root; // Root of the tree. Note that if the tree is unrooted, then the tree will be rooted at the vertex neighbouring taxa 1
  //IMPT NOTE: taxas are 1-indexed, nodes are 0-indexed
  //idx[x] stores node representing taxa x, leaf[x] stores taxa of node x (default 0)
  //size[x] stores size of subtree at x, label[x] stores the label of taxa x after relabelling (for Day's algorithm)
  //minH, maxH is used for days algorithm (range of taxa in a subtree)
  //minL, maxL stores the new max and min in a subtree after relabelling
  //parent[x] stores parent of node x.
	int *idx,*label,*size,*leaf,*minL,*maxL,*minH,*maxH,*H,*parent,*goodLabel;
	bool *good; //good[x] means x is good split
  vector <int> *G; //stores edges in the graph. All edges are directed edges directed away from the root ie each edge denotes a parent-child relationship
  tree(){}
  tree(int _N,int _cnt,bool RTD); //Constructor
	tree(int _N,string s,bool RTD); //Constructor
  void precompute(); //Precomputing all the arrays in the structure
  void printNex(); //Output the tree in NEXUS format
};
#endif
