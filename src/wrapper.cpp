#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <queue>
#include <sstream>
#include "wrapper.h"
#include "tree.h"
#include "random.h"
using namespace std;

extern tree *T;
extern int numTrees,numTaxas,rooted;

void nexReader(string s){
  //freopen(s.c_str(),"r",stdin); //Read in data
  ifstream fin(s.c_str());
	string S;
  vector <string> trees;
  getline(fin,S);
  getline(fin,S);
	while(S != "translate"){
		if(S == "translate") break;
		getline(fin,S);
	}
  printf("Inputting Taxas ...\n");
  while(1){
    getline(fin,S);
    ++numTaxas;
    if(S[S.length()-1] == ';') break;
  }
  printf("Inputting Trees ...\n");
  while(1){
    getline(fin,S);
    if(S == "END;") break;
    for(unsigned int i=0;i<S.length();++i)
      if(S[i] == '('){
        trees.push_back(S.substr(i,S.length()-i));
        break;
      }
  }
  numTrees = trees.size();
  T = new tree[numTrees];
  for(int i=0;i<numTrees;++i){
		T[i] = tree(numTaxas,trees[i],rooted);
	}
	return;
}

void constructRandomTree(int *p, int len) {
  queue<int> l;
  p[0]=-1;
  int nodei=0;
  l.push(nodei);
  while(nodei<len) {
    if(Random::RANDOM.NextDouble() > 0.5) {
      int ntop=l.front();l.pop();
      int c1=nodei+1;
      int c2=nodei+2;
      nodei++;nodei++;
      p[c1]=ntop; p[c2]=ntop;
      l.push(c1); l.push(c2);
    }
    else {
      l.push(l.front()); l.pop();
    }
  }
}

void constructBinaryTree(int *p, int len) {
  queue<int> l;
  p[0]=-1;
  int nodei=0;
  l.push(nodei);
  while(nodei+1<len) {
    int ntop=l.front();l.pop();
    int c1=nodei+1;
    int c2=nodei+2;
    nodei++;nodei++;
    p[c1]=ntop; p[c2]=ntop;
    l.push(c1); l.push(c2);
  }
}

void constructDepthTree(int *p, int len) {
  queue<int> l;
  p[0]=-1;
  int nodei=0;
  l.push(nodei);
  while(nodei+1<len) {
    int ntop=l.front();l.pop();
    int c1=nodei+1;
    int c2=nodei+2;
    nodei++;nodei++;
    p[c1]=ntop; p[c2]=ntop;
    l.push(c1);
  }
}

void constructNonBinaryTree(int *p, int taxa_n, int *cnt) {
  queue<int> l;
  p[0]=-1;
  int nodei=0;
  l.push(nodei);
  int lc=1;
  (*cnt)=1;
  while(lc+2<=taxa_n) {
    int ntop=l.front();l.pop();
    int c1=nodei+1;
    int c2=nodei+2;
    int c3=nodei+3;
    nodei++;nodei++;nodei++;
    (*cnt)++;(*cnt)++;(*cnt)++;
    p[c1]=ntop; p[c2]=ntop; p[c3]=ntop;
    l.push(c1); l.push(c2); l.push(c3);
    lc+=2;
  }
}

string printTreeRecur(int* p, int node, int len, int* taxa_count, vector<int>* taxa_pool) {
  bool leaf = true;
  stringstream ss;
  for(int i=0;i<len;i++)
    if(p[i]==node) 
      leaf = false;
  if(leaf){
    ss << (*taxa_pool)[*taxa_count] << ":2.2222";
    (*taxa_count)++;
    return ss.str();
  }
  else{
    vector<int> cs;
    for(int i=0;i<len;i++)
      if(p[i]==node)
        cs.push_back(i);
    if (cs.size()==1)
      ss<<"("<<printTreeRecur(p,cs[0],len,taxa_count,taxa_pool)<<"):2.2222";
    else {
      ss << "(" ;
      for(int i=0;i<cs.size()-1;i++) {
        ss << printTreeRecur(p, cs[i], len, taxa_count, taxa_pool);
        ss << ",";
      }
    }
    ss << printTreeRecur(p, cs[cs.size()-1],len, taxa_count, taxa_pool);
    ss << ")";
    ss << ":2.2222";
    return ss.str();
  }
  return ss.str();
}

void generate_taxa_pool(vector<int>* pool, int n) {
  vector<int> tmp;
  for(int i=1; i<=n; i++)
    pool->push_back(i);
  for(int i=0;i<n;i++)
    if (Random::RANDOM.NextDouble() > 0.9)
      tmp.push_back(i);
  for(int i=0;i<tmp.size();i=i+2)
  {
    if(i+1 == tmp.size()) break;
    int t=(*pool)[tmp[i]];
    (*pool)[tmp[i]] = (*pool)[tmp[i+1]];
    (*pool)[tmp[i+1]] = t;
  }
}

string nexTree(int n, int type) {
  string ss;
  int* p = new int[n*2+5];
  for(int i=0;i<n*2+5;i++) p[i]=0;
  int taxa_count=0;
  vector<int> taxa_pool;
  switch(type) {
    case 0:
      cout << "constructRandomTree(p=[...],n="<<n<<",)"<<endl;
      constructRandomTree(p, n*2-1);
      for(int i=0;i<n*2-1;i++) cout<<p[i]<<",";
      cout<<endl;
      taxa_count=0;
      taxa_pool.clear();
      generate_taxa_pool(&taxa_pool, n);
      // for(int i=0;i<taxa_pool.size();i++) cout<<taxa_pool[i]<<",";
      ss = printTreeRecur(p, 0, n*2-1, &taxa_count, &taxa_pool);
      break;
    case 1:
      cout << "constructBinaryTree(p=[...],n="<<n<<",)"<<endl;
      constructBinaryTree(p, n*2-1);
      for(int i=0;i<n*2-1;i++) cout<<p[i]<<",";
      cout<<endl;
      taxa_count=0;
      taxa_pool.clear();
      generate_taxa_pool(&taxa_pool, n);
      // for(int i=0;i<taxa_pool.size();i++) cout<<taxa_pool[i]<<",";
      ss = printTreeRecur(p, 0, n*2-1, &taxa_count, &taxa_pool);
      break;
    case 2:
      cout << "constructDepthTree(p=[...],n="<<n<<",)"<<endl;
      constructDepthTree(p, n*2-1);
      for(int i=0;i<n*2-1;i++) cout<<p[i]<<",";
      cout<<endl;
      taxa_count=0;
      taxa_pool.clear();
      generate_taxa_pool(&taxa_pool, n);
      // for(int i=0;i<taxa_pool.size();i++) cout<<taxa_pool[i]<<",";
      ss = printTreeRecur(p, 0, n*2-1, &taxa_count, &taxa_pool);
      break;
    case 3:
      cout << "constructNonBinaryTree(p=[...],n="<<n<<",)"<<endl;
      int cnt;
      constructNonBinaryTree(p, n, &cnt);
      for(int i=0;i<n*2-1;i++) cout<<p[i]<<",";
      cout<<endl;
      taxa_count=0;
      taxa_pool.clear();
      generate_taxa_pool(&taxa_pool, n);
      // for(int i=0;i<taxa_pool.size();i++) cout<<taxa_pool[i]<<",";
      ss = printTreeRecur(p, 0, cnt, &taxa_count, &taxa_pool);
      break;
    default:
      break;
  }

  stringstream re;
  re << "(" << ss << ")";
  delete[] p;
  return re.str();
}

void nexGen(int k, int n, int type) {
  unsigned int seed = time(NULL); cout << "seed:"<<seed<<endl;
  Random::RANDOM = Random(seed);
  string r = "#NEXUS\n[random generated]\nBEGIN TREES;\ntranslate\n";
  stringstream rss; rss << r;
  for (int i=1;i<n;i++) { // taxa is 1 indexed
    rss<< i << " " << "random_taxa_" << i << "," << endl;
  }
  rss << n << " " << "random_taxa_" << n << ";" << endl;
  for (int i=1;i<=k;i++) {
    rss << "tree " << i << " = ";
    rss << nexTree(n, type);
    rss << ";" << endl;
  }
  rss << "END;";
  cout << rss.str() << endl;
}