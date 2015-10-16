#include "loose.h"
using namespace std;

extern tree *T;
extern int numTaxas,numTrees,rooted;
static int LEN;
static vector <vector <ll> > LF;

tree looseConsensusSlow(){
	stack <pair<int,int> > S;
  int cnt,tmp,sze;
  LEN = (numTaxas+BUCKET_SIZE-1)/BUCKET_SIZE;
  vector <ll> cluster[2*numTaxas+5];
  for(int i=0;i<2*numTaxas;++i)
    for(int j=0;j<LEN;++j) cluster[i].push_back(0);
  for(int i=0;i<numTrees;++i){
    for(int j=0;j<T[i].cnt;++j)
    	for(int k=0;k<LEN;++k) cluster[j][k] = 0;
    S.push(pair<int,int>(T[i].root,-1));
    while(!S.empty()){
      pair<int,int> t1 = S.top();
      S.pop();
      ++t1.second;
      if(t1.second < T[i].G[t1.first].size()){
        S.push(t1);
        S.push(pair<int,int>(T[i].G[t1.first][t1.second],-1));
      }
      else{
        if(T[i].leaf[t1.first] > 0){
          cnt = T[i].leaf[t1.first];
          cluster[t1.first][(cnt-1)/BUCKET_SIZE] |= (1ll<<((cnt-1)%BUCKET_SIZE));
        }
        else if(t1.first != T[i].root) LF.push_back(cluster[t1.first]);
        if(T[i].parent[t1.first] >= 0){
          cnt = T[i].parent[t1.first];
          for(int j=0;j<LEN;++j) cluster[cnt][j] |= cluster[t1.first][j];
        }
      }
    }
  }
  int L = LF.size(), A[L];
  for(int i=0;i<L;++i) A[i] = i;
  sort(A,A+L,cmpLeafSetLOOSE);
  cnt = 0;
  vector <int> good, curSet;
  for(int i=0;i<L;++i){
    if(sameLeafSetLOOSE(A[cnt],A[i])) continue;
    curSet.push_back(A[cnt]);
    cnt = i;
  }
  curSet.push_back(A[cnt]), cnt = curSet.size();
  bool flag;
  vector <string> sol;
  string str = "";
  for(int a=0;a<cnt;++a){
  	flag = 1;
  	for(int b=0;b<cnt;++b)
			if(!checkCompatible(LF,curSet[a],curSet[b],LEN)){
			flag = 0;
			break;
		}
		if(flag) good.push_back(curSet[a]);
	}
	for(int i=0;i<numTaxas;++i) str += '0';
	for(vector<int>::iterator it=good.begin();it!=good.end();++it){
    for(int i=0;i<LEN;++i)
    	for(int j=0;j<BUCKET_SIZE;++j)
    		if(i * BUCKET_SIZE + j < numTaxas) str[i * BUCKET_SIZE + j] = (char)(((LF[*it][i]&(1ll<<j)) > 0) + '0');
    		else break;
    sol.push_back(str);
  }
  for(int i=0;i<numTaxas;++i) str[i] = '0';
  for(int i=0;i<numTaxas;++i){
		++str[i];
		sol.push_back(str);
		--str[i];
	}
  tree ret = greedyConstruct(sol);
  return ret;
}

bool cmpLeafSetLOOSE(int a,int b){
  for(int i=0;i<LEN;++i)
    if(LF[a][i] != LF[b][i]) return LF[a][i] < LF[b][i];
  return 0;
}

bool sameLeafSetLOOSE(int a,int b){
  for(int i=0;i<LEN;++i)
    if(LF[a][i] != LF[b][i]) return 0;
  return 1;
}

tree looseConsensusFast(){
  tree ret;
  for(int i=0;i<numTrees;++i){
    if(i == 0) ret = T[0];
    else ret = looseMerge(ret,T[i],1); //Constructing the one-way compatible tree using the recursive definition
  }
  ret.precompute();
  for(int i=1;i<=numTaxas;++i) ret.good[i] = (ret.H[i] >= 0);
  for(int i=0;i<numTrees;++i) ret = looseMerge(ret,T[i],0); //Determining which vertices in the one-way compatible tree are in the loose consensus tree
  ret = contract(ret);
  return ret;
}

tree looseMerge(tree A, tree B, bool op){
  pair<int,int> S[max(A.cnt,B.cnt)+5];
  int ai = 0, cnt = 0, POS[numTaxas+5], DEPTH[B.cnt+5], pid[B.cnt+5], newNodes = 0;
  bool can[A.cnt+5]; memset(can,0,sizeof(can));
  memset(A.size,0,(A.cnt+5));
  //Relabelling leaves in A so that each subtree in A can be represented as a consecutive range of leaves
  A.precompute();
  
  for(int i=1;i<=numTaxas;++i) B.label[i] = A.label[i];
  
  vector <int> L[numTaxas+5],R[numTaxas+5],BEFORE[B.cnt+5],AFTER[B.cnt+5];
  S[ai++] = pair<int,int>(B.root,-1);
  //Relabelling leaves of B using leaf labels in A and change B into an ordered tree.
  while(ai){
    pair<int,int> t1 = S[--ai];
    if(t1.second == -1){
      if(B.leaf[t1.first] > 0) B.minL[t1.first] = B.label[B.leaf[t1.first]];
      else B.minL[t1.first] = numTaxas+1;
    }
    ++t1.second;
    if(t1.second < B.G[t1.first].size()){
      S[ai++] = t1;
      S[ai++] = pair<int,int>(B.G[t1.first][t1.second],-1);  
    }
    else{
      if(B.parent[t1.first] >= 0) L[B.minL[t1.first]].push_back(t1.first);
      B.G[t1.first].clear();
      if(B.parent[t1.first] >= 0){
        if(B.minL[t1.first] < B.minL[B.parent[t1.first]]) B.minL[B.parent[t1.first]] = B.minL[t1.first];  
      }
    }
  }
  for(int i=1;i<=numTaxas;L[i].clear(),++i)
    for(vector<int>::iterator it=L[i].begin();it!=L[i].end();++it)
      B.G[B.parent[*it]].push_back(*it);
  //Construct the left and right paths for the leaves in B
  cnt = 0, POS[numTaxas+1] = numTaxas+1, POS[numTaxas+2] = -1, DEPTH[B.root] = 0; 
  S[ai++] = pair<int,int>(B.root,-1);
  while(ai){
    pair<int,int> t1 = S[--ai];
    if(t1.second == -1){
      if(B.leaf[t1.first] > 0)
        B.minL[t1.first] = B.maxL[t1.first] = B.leaf[t1.first], POS[B.leaf[t1.first]] = ++cnt;
      else B.minL[t1.first] = numTaxas+1, B.maxL[t1.first] = numTaxas+2;
    }
    ++t1.second;
    if(t1.second < B.G[t1.first].size()){
      DEPTH[B.G[t1.first][t1.second]] = DEPTH[t1.first] + 1;
      S[ai++] = t1;
      S[ai++] = pair<int,int>(B.G[t1.first][t1.second],-1);
    }
    else{
      L[B.minL[t1.first]].push_back(t1.first);
      R[B.maxL[t1.first]].push_back(t1.first);
      int cur = 0;
      for(vector<int>::iterator it=B.G[t1.first].begin();it!=B.G[t1.first].end();++it){
        BEFORE[t1.first].push_back(0);
        AFTER[t1.first].push_back(0);
      }
      if(B.parent[t1.first] >= 0){
        int p = B.parent[t1.first];
        if(POS[B.minL[t1.first]] < POS[B.minL[p]]) B.minL[p] = B.minL[t1.first];
        if(POS[B.maxL[t1.first]] > POS[B.maxL[p]]) B.maxL[p] = B.maxL[t1.first];
      }
      cur = 0;
      for(vector<int>::iterator it=B.G[t1.first].begin();it!=B.G[t1.first].end();++it)
        pid[*it] = cur++;
    }
  }
  //Querying subtrees with A to see if they are compatible with B
  S[ai++] = pair<int,int>(A.root,-1);
  while(ai){
    pair<int,int> t1 = S[--ai];
    if(t1.second == -1){
      if(A.leaf[t1.first]) A.minL[t1.first] = A.maxL[t1.first] = A.leaf[t1.first];
      else A.minL[t1.first] = numTaxas+1, A.maxL[t1.first] = numTaxas+2;
    }
    ++t1.second;
    if(t1.second < A.G[t1.first].size()){
      S[ai++] = t1;
      S[ai++] = pair<int,int>(A.G[t1.first][t1.second],-1);
    }
    else{
      int a = A.minL[t1.first], b = A.maxL[t1.first];
    	if(POS[b] - POS[a] + 1 != A.size[t1.first]){
				can[t1.first] = 0;
				goto end;
			}
      if(DEPTH[L[a].back()] >= DEPTH[R[b].back()]){
        if(DEPTH[L[a].back()] - DEPTH[B.idx[b]] > 0){
          can[t1.first] = 0;
          goto end;
        }
        a = L[a].back();
        b = R[b][DEPTH[B.idx[b]]-DEPTH[a]];
      }
      else{
        if(DEPTH[R[b].back()] - DEPTH[B.idx[a]] > 0){
          can[t1.first] = 0;
          goto end;
        }
        b = R[b].back();
        a = L[a][DEPTH[B.idx[a]]-DEPTH[b]];
      }
   		if(a == b) can[t1.first] = 1;
      else if(B.parent[a] == B.parent[b]){
        can[t1.first] = 1;
        int t1 = pid[a], t2 = pid[b];
        ++newNodes;
        ++BEFORE[B.parent[a]][t1];
        ++AFTER[B.parent[a]][t2];
      }
      else can[t1.first] = 0; 
      end:;
      if(A.parent[t1.first] >= 0){
        int p = A.parent[t1.first];
        if(POS[A.minL[t1.first]] < POS[A.minL[p]]) A.minL[p] = A.minL[t1.first];
        if(POS[A.maxL[t1.first]] > POS[A.maxL[p]]) A.maxL[p] = A.maxL[t1.first];
      }
    }
  }
  if(op == 0){ //op == 0 means that the compatible vertices in A need to be marked and the incompatible vertices unmarked.
    for(int i=1;i<=numTaxas;++i)
      if(A.H[i] >= 0) A.good[i] &= can[A.H[i]];
    return A;
  }
  else{ //op == 1 means that a new tree needs to be created.
    if(newNodes == 0) return B;
    tree ret = tree(numTaxas,B.cnt+newNodes,rooted);
    ret.root = B.root;
    ret.parent[ret.root] = -1;
    int cnt = B.cnt,cur;
    for(int a=0;a<B.cnt;++a){
      ret.leaf[a] = B.leaf[a];
      cur = a;
      for(int i=0;i<(int)B.G[a].size();++i){
        while(BEFORE[a][i]){
          ret.parent[cnt] = cur;
          ret.G[cur].push_back(cnt);
          cur = cnt++;
          --BEFORE[a][i];
        }
        ret.G[cur].push_back(B.G[a][i]);
        ret.parent[B.G[a][i]] = cur;
        while(AFTER[a][i]){
          cur = ret.parent[cur];
          --AFTER[a][i];
        }
      }
    }
    for(int i=1;i<=numTaxas;++i) ret.idx[i] = B.idx[i];
    assert(cnt == ret.cnt);
    return ret;
  }
}
