#include "adams.h"

extern int numTaxas,numTrees,rooted;
extern tree *T;
static int ***LCA, **LCA_POS, **LCA_DEPTH, *JUMP, *LCA_LEN;
static int sortConstant;

tree adamsConsensusSlow(){
  vector <int> V;
  for(int i=1;i<=numTaxas;++i) V.push_back(i);
  return adamsRecursionSlow(V);
}

tree adamsRecursionSlow(vector <int> V){
  tree ret;
  if(V.size() == 1){
    ret = tree(numTaxas,1,rooted);
    ret.idx[V[0]] = ret.root = 0;
    ret.leaf[0] = V[0];
    ret.parent[ret.root] = -1;
  }
  else if(V.size() == 2){
    ret = tree(numTaxas,3,rooted);
    ret.root = 0;
    ret.idx[V[0]] = 1; ret.leaf[1] = V[0];
    ret.idx[V[1]] = 2; ret.leaf[2] = V[1];
    ret.G[0].push_back(1); ret.G[0].push_back(2);
    ret.parent[1] = ret.parent[2] = 0;
    ret.parent[ret.root] = -1;
  }
  else {
    int arr[numTaxas+5];
    string ID[numTaxas+5];
    for(int i=0;i<numTaxas+5;++i) ID[i] = "";
    //for(int i=0;i<(int)V.size();++i) printf("%d ",V[i]); printf("\n");
    for(int z=0;z<numTrees;++z){	
      memset(T[z].size,0,(T[z].cnt+5)*sizeof(int));
      for(int i=0;i<(int)V.size();++i) ++T[z].size[T[z].idx[V[i]]];
      int a1 = -1,a2 = -1;
      stack <pair<int,int> > S;
      S.push(pair<int,int>(T[z].root,-1));
      while(!S.empty()){ // iterate through tree
        pair<int,int> t1 = S.top();
        S.pop();
        ++t1.second;
        if(t1.second < T[z].G[t1.first].size()){
          S.push(t1);
          S.push(pair<int,int>(T[z].G[t1.first][t1.second],-1));
        }
        else{
          if(T[z].parent[t1.first] > -1) T[z].size[T[z].parent[t1.first]] += T[z].size[t1.first];
          T[z].minL[t1.first] = 1;
          for(vector<int>::iterator it=T[z].G[t1.first].begin();it!=T[z].G[t1.first].end();++it)
            if(T[z].size[*it] == T[z].size[t1.first]){
              T[z].minL[t1.first] = 0;
              break;
            }
          if(T[z].minL[t1.first]){
            if(a1 == -1 || T[z].size[t1.first] >= T[z].size[a1]) a2 = a1, a1 = t1.first;
            else if(a2 == -1 || T[z].size[t1.first] > T[z].size[a2]) a2 = t1.first;
          }
        }
      }
      queue<int> bfs; bfs.push(a1);
      vector<int> subTreeRoot;
      while(!bfs.empty()){
        int cur=bfs.front(); bfs.pop();
        if(T[z].minL[cur]&&cur!=a1){
          subTreeRoot.push_back(cur);
          continue; // skip all children
        }
        for(int i=0;i<T[z].G[cur].size();++i){
          bfs.push(T[z].G[cur][i]);
        }
      }

      memset(arr,0,sizeof(arr));
      for(int i=0;i<subTreeRoot.size();++i) {
        S.push(pair<int,int>(subTreeRoot[i],-1));
        while(!S.empty()){ // go through second biggest subtree
          pair<int,int> t1 = S.top();
          S.pop();
          ++t1.second;
          if(t1.second < T[z].G[t1.first].size()){
            S.push(t1);
            S.push(pair<int,int>(T[z].G[t1.first][t1.second],-1));
          }
          else{
            if(T[z].minL[t1.first] && T[z].size[t1.first] == 1)
              arr[T[z].leaf[t1.first]] = subTreeRoot[i];
          }
        }
      }
      for(int i=0;i<(int)V.size();++i) {
        stringstream ss; ss << arr[V[i]];
        string t=ss.str(); while(t.size()< 13) t+="-";
        ID[V[i]] += (t); // build partition index
      }
    }
    vector <pair<string,int> > array;
    for(int i=0;i<(int)V.size();++i) array.push_back(pair<string,int>(ID[V[i]],V[i]));
    sort(array.begin(),array.end());
    vector <int> tmp;
    vector <tree> X;
    for(int i=0;i<(int)array.size();++i)
      if(!i || array[i].first == array[i-1].first) tmp.push_back(array[i].second);
      else{
        X.push_back(adamsRecursionSlow(tmp));
        tmp.clear();
        tmp.push_back(array[i].second);
      }
    if(!tmp.empty()) X.push_back(adamsRecursionSlow(tmp));
    int cnt = 0;
    for(int i=0;i<(int)X.size();++i) cnt += X[i].cnt;
    ++cnt;
    ret = tree(numTaxas,cnt,rooted);
    ret.root = 0;
    cnt = 1;
    for(int a=0;a<(int)X.size();++a){
      ret.G[ret.root].push_back(cnt + X[a].root);
      ret.parent[cnt + X[a].root] = ret.root;
      for(int i=0;i<X[a].cnt;++i){
        for(int j=0;j<X[a].G[i].size();++j){
          ret.G[cnt+i].push_back(cnt + X[a].G[i][j]);
          ret.parent[cnt + X[a].G[i][j]] = cnt+i;
        }
        ret.leaf[cnt+i] = X[a].leaf[i];
        if(X[a].leaf[i] > 0){
          ret.idx[X[a].leaf[i]] = cnt+i;
        }
      }
      cnt += X[a].cnt;
    }
  }
  return ret;
}

nubTree::nubTree(int _N,int _cnt){
  N = _N;
  cnt = _cnt;
  leaf = new int[cnt+5];
  parent = new int[cnt+5];
  mL = new int[cnt+5];
  mR = new int[cnt+5];
  semiroot = new int[cnt+5];
  taxa_idx = new int[4*numTaxas+5];
  leaf_array = new int[N+5];
  fenwick = new int[N + 5];
  G = new vector<int>[cnt+5];
  memset(leaf,0,(cnt+5) * sizeof(int));
  memset(parent,-1,(cnt+5) * sizeof(int));
  memset(mL,-1,(cnt+5) * sizeof(int));
  memset(mR,-1,(cnt+5) * sizeof(int));
  memset(semiroot,-1,(cnt+5) * sizeof(int));
  memset(taxa_idx,-1,(4*numTaxas+5)*sizeof(int));
  memset(fenwick,0,(N+5)*sizeof(int));
  memset(leaf_array,0,(N+5)*sizeof(int));
  // for(int i=0;i<cnt+5;++i) G[i].clear();
  // L_POS.clear();
}

void nubTree::update(int x,int d){
  for(;x <= N;x += (x&-x)) fenwick[x] += d;
}

int nubTree::query(int x){
  if(x <= 0) return 0;
  for(sum=0;x>0;x -= (x&-x)) sum += fenwick[x];
  return sum;
}

tree adamsConsensusFast(){
  precomputeLCA();
  set <int> *S = new set<int>;
  for(int i=1;i<=numTaxas;++i) S->insert(i);
  retTree x = adamsRecurse(S);
  //printf("DONE TREE!!!\n");
  tree ret = tree(numTaxas,x.parent.size(),rooted);
  ret.root = x.root;
  int LENGTH = x.parent.size(), t1;
  for(int i=0;i<LENGTH;++i){
    if(x.parent[i] >= 0){
      t1 = x.parent[i];
      ret.G[t1].push_back(i);
      ret.parent[i] = t1;
    }
    ret.leaf[i] = x.leaf[i];
  }
  //Clear Mem for LCA
  for(int i=0;i<numTrees;++i){
		for(int j=0;j<LCA_LEN[i];++j) delete []LCA[i][j];
		delete []LCA_POS[i];
		delete []LCA_DEPTH[i];
		delete []LCA[i];
	}
	delete []LCA_POS;
	delete []LCA_DEPTH;
	delete []LCA;
	delete []LCA_LEN;
	delete []JUMP;
  return ret;
}

retTree adamsRecurse(set <int> *S){
  if(S->size() <= 2){
    retTree ret;
    for(set<int>::iterator it=S->begin();it!=S->end();++it){
      ret.parent.push_back(-1);
      ret.leaf.push_back(*it);
    }
    if(ret.parent.size() == 1) ret.root = 0;
    else{
      for(vector<int>::iterator it=ret.parent.begin();it!=ret.parent.end();++it) *it = 2;
      ret.parent.push_back(-1);
      ret.leaf.push_back(0);
    }
    return ret;
  }
  //Precompute all the trees
  int *arr, *parent, *sortArray, *LEAF, cnt=0, cur, LEN, ai;
  nubTree *A = new nubTree[numTrees];
  LEN = S->size();
  arr = new int[2 * LEN + 5];
  parent = new int[2 * LEN + 5];
  sortArray = new int[LEN + 5];
  LEAF = new int[2 * LEN + 5]; // idx - taxa
  pair<int,int> STK[2*LEN+5]; //stack
  for(set<int>::iterator it = S->begin();it != S->end();++it) sortArray[++cnt] = *it;
  for(int z=0;z<numTrees;++z){
    cnt = ai = 0;
    sortConstant = z;
    sort(sortArray+1,sortArray + LEN+1,adamsSortCmp);
    memset(parent,-1,(2*LEN+5)*sizeof(int));
    memset(LEAF,0,(2*LEN+5)*sizeof(int));
    //Construct Tree Relationships
    for(int i=1;i<=LEN;++i){
      if(i == 1){
        arr[cnt] = T[z].idx[sortArray[i]];
        LEAF[cnt] = sortArray[i];
      }
      else{
        int tmp = get_LCA(z,sortArray[i-1],sortArray[i]);
        while(parent[cur] != -1 && LCA_DEPTH[z][arr[parent[cur]]] > LCA_DEPTH[z][tmp]) cur = parent[cur];
        if(parent[cur] == -1){
					arr[cnt] = tmp;
					parent[cur] = cnt;
					cur = cnt++;
				}
        else if(LCA_DEPTH[z][arr[parent[cur]]] < LCA_DEPTH[z][tmp]){
          arr[cnt] = tmp;
          parent[cnt] = parent[cur];
          parent[cur] = cnt;
          cur = cnt++;
        }
        else if(LCA_DEPTH[z][arr[parent[cur]]] == LCA_DEPTH[z][tmp]) cur = parent[cur];//,printf("C\n");
        else{
          arr[cnt] = tmp;
          cur = parent[cur];
          parent[cur] = cnt;
          cur = cnt++;
        }
        arr[cnt] = T[z].idx[sortArray[i]];
        LEAF[cnt] = sortArray[i];
        parent[cnt] = cur;
      }
      cur = cnt++;
    }
    //Construct the Tree
    A[z] = nubTree(LEN,cnt);
    for(int i=1;i<=LEN;++i){
      A[z].leaf_array[i] = sortArray[i];
      A[z].L_POS[A[z].leaf_array[i]] = i;
      A[z].update(i,1);
    }
    for(int i=0;i<cnt;++i){
      if(parent[i] != -1){
        A[z].parent[i] = parent[i]; 
        A[z].G[parent[i]].push_back(i); 
      }
      else A[z].root = i;
      A[z].leaf[i] = LEAF[i];
      A[z].taxa_idx[LEAF[i]] = i;
    }
    ai = 0;
    STK[ai++] = pair<int,int>(A[z].root,-1);
    // stack<pair<int,int>> tSTK; tSTK.push(pair<int,int>(A[z].root,-1));
    while(ai){
    // while(!tSTK.empty()){
      pair<int,int> t1 = STK[--ai];
      // pair<int,int> t1 = tSTK.top(); tSTK.pop();
      if(t1.second == -1){
        if(A[z].leaf[t1.first] > 0) A[z].mL[t1.first] = A[z].mR[t1.first] = A[z].L_POS[A[z].leaf[t1.first]];
        else A[z].mL[t1.first] = numTaxas+5, A[z].mR[t1.first] = 0;
      }
      ++t1.second;
      if(t1.second < A[z].G[t1.first].size()){
        STK[ai++] = t1;
        // tSTK.push(t1);
        STK[ai++] = pair<int,int>(A[z].G[t1.first][t1.second],-1);
        // tSTK.push(pair<int,int>(A[z].G[t1.first][t1.second],-1));
      }
      else if(A[z].parent[t1.first] >= 0){
        int t2 = A[z].parent[t1.first];
        if(A[z].mL[t1.first] < A[z].mL[t2]) A[z].mL[t2] = A[z].mL[t1.first];
        if(A[z].mR[t1.first] > A[z].mR[t2]) A[z].mR[t2] = A[z].mR[t1.first];
      }
    }


  }
  //Recursing Down the Heavy Path
  vector <int> *recurLeaf = new vector<int>[LEN], *recurRoot = new vector<int>[numTrees], *recurBigTree = new vector<int>[numTrees];
  for(int z=0;z<numTrees;++z) recurRoot[z].push_back(A[z].root);
  int recurDepth = 0;
  while(S->size() > 2){
    int t1,t2,sze,tmp;
    //Find Big Subtree
    for(int z=0;z<numTrees;++z){
      t1 = recurRoot[z][recurDepth], sze = 0;
      for(vector<int>::iterator it=A[z].G[t1].begin();it!=A[z].G[t1].end();++it){
        tmp = A[z].query(A[z].mR[*it]) - A[z].query(A[z].mL[*it]-1);
        if(tmp > sze) cur = *it, sze = tmp;
      }
      recurBigTree[z].push_back(cur);
    }
    //Remove Leaves in Small Subtrees
    for(int z=0;z<numTrees;++z){
      t1 = A[z].mL[recurRoot[z][recurDepth]], t2 = A[z].mL[recurBigTree[z][recurDepth]];
      for(int i=t1;i<t2;++i){
        tmp = A[z].leaf_array[i];
        if(S->find(tmp) != S->end()){
          S->erase(tmp);
          recurLeaf[recurDepth].push_back(tmp);
          for(int y=0;y<numTrees;++y)
            A[y].update(A[y].L_POS[tmp],-1);
        }
      }
      t1 = A[z].mR[recurBigTree[z][recurDepth]], t2 = A[z].mR[recurRoot[z][recurDepth]];
      for(int i=t1+1;i<=t2;++i){
        tmp = A[z].leaf_array[i];
        if(S->find(tmp) != S->end()){
          S->erase(tmp);
          recurLeaf[recurDepth].push_back(tmp);
          for(int y=0;y<numTrees;++y)
            A[y].update(A[y].L_POS[tmp],-1);
        }
      } 
    }
		//Find new root
		for(int z=0;z<numTrees;++z){
			t1 = recurBigTree[z][recurDepth], t2 = A[z].query(A[z].mR[t1]) - A[z].query(A[z].mL[t1]-1);
			while(1){
				sze = 0;
				for(vector<int>::iterator it=A[z].G[t1].begin();it!=A[z].G[t1].end();++it)
					if(A[z].query(A[z].mR[*it]) - A[z].query(A[z].mL[*it]-1) == t2){
						t1 = *it;
						sze = 1;
						break;
					}
				if(!sze) break;
			}
			recurRoot[z].push_back(t1);
		}
		++recurDepth;
  }

  // build semiroot index for later partition (go up tree)
  for(int z=0;z<numTrees;++z){
    queue<int> bfs; bfs.push(recurBigTree[z][recurDepth-1]);
    while(!bfs.empty()){
      int n = bfs.front(); bfs.pop();
      A[z].semiroot[n]=recurBigTree[z][recurDepth-1];
      for(auto it=A[z].G[n].begin();it!=A[z].G[n].end();++it){
        bfs.push(*it);
      }
    }
  }
  int td=recurDepth-1;
  while(td>=0){
    for(int z=0;z<numTrees;++z){
      vector<int> semiroots;
      for(auto it=A[z].G[recurRoot[z][td]].begin();it!=A[z].G[recurRoot[z][td]].end();++it){
        if((*it)==recurBigTree[z][td]) continue;
        semiroots.push_back(*it);
      }
      for(int i=0;i<semiroots.size();++i){
        queue<int> bfs; bfs.push(semiroots[i]);
        while (!bfs.empty()){
          int n = bfs.front(); bfs.pop();
          A[z].semiroot[n]=semiroots[i];
          for(auto it=A[z].G[n].begin();it!=A[z].G[n].end();++it){
            bfs.push(*it);
          }
        } // bfs end
      } // semi root end
    }
    td--;
  }

  retTree ret;
  vector <int> *sortLeaf = new vector<int>[LEN];
  set <int> *subTree = new set<int>;
  //Recursing through the layers to construct the final tree
  while(recurDepth > 0){
    if(S->size() > 0){
      for(set<int>::iterator it=S->begin();it!=S->end();++it){
        ret.parent.push_back(-1);
        ret.leaf.push_back(*it);
      }
      if(ret.parent.size() == 1) ret.root = 0;
      else{
        ret.root = 2;
        for(vector<int>::iterator it=ret.parent.begin();it!=ret.parent.end();++it) *it = 2;
        ret.parent.push_back(-1);
        ret.leaf.push_back(0);
      }
      S->clear();
    }
    --recurDepth;
    if(!recurLeaf[recurDepth].empty()){
    	if(ret.parent.empty()) ret.root = 0;
    	else ret.parent[ret.root] = ret.root = ret.parent.size();
      ret.parent.push_back(-1);
      ret.leaf.push_back(0);
      cnt = 0;
      for(vector<int>::iterator it=recurLeaf[recurDepth].begin();it!=recurLeaf[recurDepth].end();++it,++cnt){
				sortLeaf[cnt].clear();
        for(int z=0;z<numTrees;++z){
          if(A[z].L_POS[*it] >= A[z].mL[recurBigTree[z][recurDepth]] && A[z].L_POS[*it] <= A[z].mR[recurBigTree[z][recurDepth]]){
            sortLeaf[cnt].push_back(recurBigTree[z][recurDepth]);
          }
          else{
            sortLeaf[cnt].push_back(A[z].semiroot[A[z].taxa_idx[*it]]); 
					}
				}
      }
      vector<pair<string,int>> IDLeafIdx; IDLeafIdx.resize(cnt);
      for(int i=0;i<cnt;++i) IDLeafIdx[i] = (pair<string,int>("",i));
      for(int i=0;i<cnt;++i)
        for(int j=0;j<sortLeaf[i].size();j++){
          stringstream ss; ss<<sortLeaf[i][j];
          string t; t=ss.str();
          while (t.size()<10) t+="0";
          IDLeafIdx[i].first += (t);
        }
      sort(IDLeafIdx.begin(),IDLeafIdx.end());

      vector<vector<int>> stmp; stmp.resize(cnt);
      for(int i=0;i<cnt;++i)
        stmp[i]=sortLeaf[IDLeafIdx[i].second];
      for(int i=0;i<cnt;++i)
        sortLeaf[i]=stmp[i];
      vector<int> rtmp; rtmp.resize(recurLeaf[recurDepth].size());
      for(int i=0;i<recurLeaf[recurDepth].size();++i)
        rtmp[i]=recurLeaf[recurDepth][IDLeafIdx[i].second];
      for(int i=0;i<rtmp.size();++i)
        recurLeaf[recurDepth][i]=rtmp[i];
      cur = 0;
      for(int i=0;i<cnt;++i)
        if(sameSubTree(cur,i,sortLeaf)) continue;
        else{
          for(int z=cur;z<i;++z) subTree->insert(recurLeaf[recurDepth][z]);
          retTree tmpT = adamsRecurse(subTree);
          int t1 = tmpT.parent.size(), t2 = ret.parent.size();
          for(int j=0;j<t1;++j){
            if(tmpT.parent[j] == -1) ret.parent.push_back(ret.root);
            else ret.parent.push_back(tmpT.parent[j] + t2);
            ret.leaf.push_back(tmpT.leaf[j]);
          }
          subTree->clear();
          cur = i;
        }
      for(int z=cur;z<cnt;++z) subTree->insert(recurLeaf[recurDepth][z]);
      retTree tmpT = adamsRecurse(subTree);
      int t1 = tmpT.parent.size(), t2 = ret.parent.size();
      for(int j=0;j<t1;++j){
        if(tmpT.parent[j] == -1) ret.parent.push_back(ret.root);
        else ret.parent.push_back(tmpT.parent[j] + t2);
        ret.leaf.push_back(tmpT.leaf[j]);
      }
      subTree->clear();
    }
  }
  //Clear Memory
  delete []A;
  delete []arr;
  delete []parent;
  delete []LEAF;
  delete []recurLeaf;
  delete []recurRoot;
  delete []recurBigTree;
  delete []sortLeaf;
  delete subTree;
  return ret;
}

void precomputeLCA(){
  pair<int,int> STK[2*numTaxas+5];
  int cnt, arr[4 * numTaxas + 5], LEN, ai = 0;
  LCA_POS = new int*[numTrees+5];
  LCA_DEPTH = new int*[numTrees+5];
  LCA = new int**[numTrees+5];
  LCA_LEN = new int[numTrees+5];
  JUMP = new int[4 * numTaxas + 5];
  for(int z=0;z<numTrees;++z){
    STK[ai++] = pair<int,int>(T[z].root,-1);
    LCA_DEPTH[z] = new int[T[z].cnt+5];
    LCA_DEPTH[z][T[z].root] = 0;
    LCA_POS[z] = new int[numTaxas+5];
    cnt = LEN = 0;
    while(ai){
      pair<int,int> t1 = STK[--ai];
      if(t1.second == -1 && T[z].leaf[t1.first] > 0) {LCA_POS[z][T[z].leaf[t1.first]] = cnt; /*if(z==1) cout << T[z].leaf[t1.first] << ":" << cnt << ",";*/}
      arr[cnt++] = t1.first;
      ++t1.second;
      if(t1.second < (int)T[z].G[t1.first].size()){
        STK[ai++] = t1;
        STK[ai++] = pair<int,int>(T[z].G[t1.first][t1.second],-1);
        LCA_DEPTH[z][T[z].G[t1.first][t1.second]] = LCA_DEPTH[z][t1.first] + 1;
      }
    }
    while((1<<LEN) <= cnt) ++LEN;
    while((1<<LEN) > cnt) --LEN;
    LCA_LEN[z] = LEN;
    LCA[z] = new int*[LEN+1];
    for(int i=0;i<=LEN;++i){
      LCA[z][i] = new int[cnt - (1<<i) + 1];
      if(!i)
        for(int j=0;j<cnt;++j) LCA[z][i][j] = arr[j];
      else
        for(int j=0;j<cnt-(1<<i)+1;++j){
          LCA[z][i][j] = (LCA_DEPTH[z][LCA[z][i-1][j]] < LCA_DEPTH[z][LCA[z][i-1][j+(1<<(i-1))]])?LCA[z][i-1][j]:LCA[z][i-1][j+(1<<(i-1))];
				}
    }
  }
  //Precompute height to jump to
  JUMP[1] = 0;
  for(int i=2;i<4*numTaxas+5;++i) JUMP[i] = JUMP[i>>1] + 1;
}

bool adamsSortCmp(int a,int b){
  return LCA_POS[sortConstant][a] < LCA_POS[sortConstant][b];
}

int get_LCA(int z,int x,int y){
  int a = LCA_POS[z][x], b = LCA_POS[z][y], c;
  if(a > b) swap(a,b);
  c = JUMP[b-a];
  return (LCA_DEPTH[z][LCA[z][c][a]] < LCA_DEPTH[z][LCA[z][c][b - (1<<c) + 1]])?LCA[z][c][a]:LCA[z][c][b - (1<<c) + 1];
}

void adamsRadixSort(int *A,vector <int> *S,int L, vector<int> *recurLeaf, int recurDepth){
  int P[2][L],cnt;
  bool f = 0;
  for(int i=0;i<L;++i) P[f][i] = i;
  for(int j=numTrees-1;j>=0;--j){
    f = !f;
    cnt = 0;
    for(int i=0;i<L;++i)
      if(S[P[!f][i]][j] == 0) P[f][cnt++] = P[!f][i];
    for(int i=0;i<L;++i)
      if(S[P[!f][i]][j] == 1) P[f][cnt++] = P[!f][i];
  }
  for(int i=0;i<L;++i) A[i] = P[f][i];



  vector<int> tmpSort[L];
  for(int i=0;i<L;++i)
    tmpSort[i]=S[A[i]];
  for(int i=0;i<L;++i)
    S[i]=tmpSort[i];
  vector<int> tmpS;tmpS.resize(L);
  for(int i=0;i<L;++i)
    tmpS[i]=recurLeaf[recurDepth][A[i]];
  for(int i=0;i<L;++i)
    recurLeaf[recurDepth][i]=tmpS[i];
}

bool sameSubTree(int a,int b,vector <int> *S){
  for(int i=0;i<numTrees;++i)
    if(S[a][i] != S[b][i]) return 0;
  return 1;
}
