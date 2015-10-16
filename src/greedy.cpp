#include "greedy.h"
using namespace std;

extern tree *T;
extern int numTaxas,numTrees,rooted;
static int LEN;
static vector<vector<ll> > LF;

tree greedyConsensusSlow(){
	LF.clear();
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
  vector <int> CountingSort[numTrees+5];
  for(int i=0;i<L;++i) A[i] = i;
  sort(A,A+L,cmpLeafSet);
  cnt = 0;
  for(int i=0;i<L;++i){
    if(sameLeafSet(A[cnt],A[i])) continue;
    CountingSort[i-cnt].push_back(A[cnt]);
    cnt = i;
  }
  CountingSort[L-cnt].push_back(A[cnt]);
  cnt = 1;
  bool flag;
  vector <int> good;
  vector <string> sol;
  string str;
  for(int a=numTrees;a>0;--a)
    for(int b=0;b<(int)CountingSort[a].size();++b){
      tmp = CountingSort[a][b];
      flag = 1;
      for(vector<int>::iterator it=good.begin();it!=good.end();++it)
      	if(!checkCompatible(LF,*it,tmp,LEN)){
					flag = 0;
					break;
				}
			if(flag) good.push_back(tmp);
		}
	str = "";
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

tree greedyConstruct(vector <string> sol){
	sort(sol.begin(),sol.end(),cp);
	int parent[sol.size()+5],S[numTaxas+5],cnt = 0;
	memset(parent,0,sizeof(parent));
	memset(S,0,sizeof(S));
	parent[0] = -1;
	for(unsigned int a=0;a<sol.size();++a){
		bool flag = 0;
		for(int i=0;i<numTaxas;++i)
			if(sol[a][i] == '1'){
				if(!flag){
					parent[++cnt] = S[i];
					flag = 1;
				}
				S[i] = cnt;
			}
	}
	tree ret = tree(numTaxas,sol.size()+1,rooted);
	ret.root = 0;
	for(int i=0;i<numTaxas;++i) ret.leaf[S[i]] = i+1;
	for(int i=1;i<=cnt;++i){
		ret.G[parent[i]].push_back(i); 
		ret.parent[i] = parent[i];
	}
	return ret;
}

bool cp(string a,string b){
	int t1 = 0, t2 = 0;
	for(int i=0;i<a.length();++i) t1 += (a[i] == '1');
	for(int i=0;i<b.length();++i) t2 += (b[i] == '1');
	if(t1 != t2) return t1 > t2;
	return a < b;
}

bool checkCompatible(vector<vector<ll> > &S, int i,int j,int LENGTH){
	bool a=0,b=0,c=0,d=0;
	for(int z=0;z<LENGTH;++z){
		a |= ( ( (S[i][z]) & (S[j][z]) ) != 0);
		b |= ( ( (S[i][z]) & (~S[j][z]) ) != 0);
		c |= ( ( (~S[i][z]) & (S[j][z]) ) != 0);
		d |= ( ( (~S[i][z]) & (~S[j][z]) ) != 0);
	}
	return (a&b&c&d) == 0;
}

tree greedyConsensusFast(){
	LF.clear();
  stack <pair<int,int> > S;
  int cnt,tmp,sze;
  LEN = (numTaxas+BUCKET_SIZE-1)/BUCKET_SIZE;
  vector <ll> cluster[2*numTaxas+5];
  //Extracting all clusters in all trees as bit vectors. Bit vectors are compressed using words of length 60
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
      if(t1.second < (int)T[i].G[t1.first].size()){
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
  //Ordering the distinct clusters in descending order of number of occurrences
  int L = LF.size(), A[L];
  vector <int> CountingSort[numTrees+5];
  for(int i=0;i<L;++i) A[i] = i;
  sort(A,A+L,cmpLeafSet);
  cnt = 0;
  for(int i=0;i<L;++i){
    if(sameLeafSet(A[cnt],A[i])) continue;
    CountingSort[i-cnt].push_back(A[cnt]);
    cnt = i;
  }
  CountingSort[L-cnt].push_back(A[cnt]);
  tree ret = tree(numTaxas,numTaxas+1,rooted);
  ret.root = 0;
  for(int i=1;i<=numTaxas;++i) ret.leaf[i] = i, ret.parent[i] = 0, ret.G[0].push_back(i);
  cnt = 1;
  bool flag;
  //Checking whether the cluster is compatible with the current tree
  for(int a=numTrees;a>0;--a)
    for(int b=0;b<(int)CountingSort[a].size();++b){
      tmp = CountingSort[a][b];
      sze = 0;
      for(int i=0;i<LEN;++i)
        for(int j=0;j<BUCKET_SIZE;++j) sze += ((LF[tmp][i]&(1ll<<j)) > 0);
      while(!S.empty()) S.pop();
      S.push(pair<int,int>(ret.root,-1));
      while(!S.empty()){
        pair<int,int> t1 = S.top();
        S.pop();
        if(t1.second == -1){
          ret.size[t1.first] = ret.minL[t1.first] = 0;
          if(ret.leaf[t1.first] > 0){
            ++ret.size[t1.first];
            if(bitExist(tmp,ret.leaf[t1.first])) ++ret.minL[t1.first];
          }
        }
        ++t1.second;
        if(t1.second < (int)ret.G[t1.first].size()){
          S.push(t1);
          S.push(pair<int,int>(ret.G[t1.first][t1.second],-1));
        }
        else{
					if(ret.minL[t1.first] == sze){
						flag = 1;
						for(vector<int>::iterator it=ret.G[t1.first].begin();it!=ret.G[t1.first].end();++it)
							flag &= (ret.minL[*it] == 0 || ret.minL[*it] == ret.size[*it]);
						if(flag){
							tree newT = tree(numTaxas,ret.cnt+1,rooted);
							newT.root = 0;
							for(int i=1;i<=numTaxas;++i) newT.leaf[i] = i;
							for(int i=0;i<ret.cnt;++i)
								if(i != t1.first)
									for(vector<int>::iterator it=ret.G[i].begin();it!=ret.G[i].end();++it)
										newT.G[i].push_back(*it), newT.parent[*it] = i;
							for(vector<int>::iterator it=ret.G[t1.first].begin();it!=ret.G[t1.first].end();++it)
								if(ret.minL[*it] == ret.size[*it])
									newT.G[ret.cnt].push_back(*it), newT.parent[*it] = ret.cnt;
								else newT.G[t1.first].push_back(*it), newT.parent[*it] = t1.first;
							newT.G[t1.first].push_back(ret.cnt), newT.parent[ret.cnt] = t1.first;
							ret = newT;
							++cnt;
							if(cnt == numTaxas) return ret;
						}
						break;
					}
					if(ret.parent[t1.first] >= 0){
						int t2 = ret.parent[t1.first];
						ret.minL[t2] += ret.minL[t1.first];
						ret.size[t2] += ret.size[t1.first];
					}
				}
      }
    }
  return ret;
}

bool cmpLeafSet(int a,int b){
  for(int i=0;i<LEN;++i)
    if(LF[a][i] != LF[b][i]) return LF[a][i] < LF[b][i];
  return 0;
}

bool sameLeafSet(int a,int b){
  for(int i=0;i<LEN;++i)
    if(LF[a][i] != LF[b][i]) return 0;
  return 1;
}

bool bitExist(int a,int x){
  return (LF[a][(x-1)/BUCKET_SIZE]&(1ll<<((x-1)%BUCKET_SIZE))) > 0;
}



