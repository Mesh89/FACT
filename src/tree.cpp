#include "tree.h"

extern bool rooted;
inline bool is_numeric(char c){return (c >= '0' && c <= '9');}

tree::tree(int _N,int _cnt, bool RTD){
	N = _N, cnt = _cnt;
	idx = new int[N+5], label = new int[N+5],minH = new int[N+5], maxH = new int[N+5], H = new int[N+5];
	good = new bool[N+5];
	memset(label,0,(N+5)*sizeof(int));
	goodLabel = new int[cnt+5];
	memset(goodLabel,-1,(cnt+5)*sizeof(int));
	leaf = new int[cnt+5], size = new int[cnt+5], minL = new int[cnt+5], maxL = new int[cnt+5], parent = new int[cnt+5];
	memset(leaf,0,(cnt+5)*sizeof(int));
	memset(parent,-1,(cnt+5)*sizeof(int));
	G = new vector <int>[cnt+5];
}

tree::tree(int _N,string str, bool RTD){
  N = _N, idx = new int[N+5], label = new int[N+5],minH = new int[N+5], maxH = new int[N+5], H = new int[N+5], cnt = 0;
  good = new bool[N+5];
  memset(label,0,(N+5)*sizeof(int));
  memset(H,-1,(N+5)*sizeof(int));
  int LEN = str.length();
  for(int i=0;i<LEN;++i) cnt += (str[i] == '(');
	cnt += N+5;
	vector <int> V [cnt];
	goodLabel = new int[cnt];
	memset(goodLabel,-1,cnt*sizeof(int));
	cnt = 0;
  stack <int> stk;
	for(int i=0;i<LEN;++i){
    if(str[i] == ':'){   //ignore floating points
      ++i;
      while (i < LEN && !(str[i] == ',' || str[i] == '(' || str[i] == ')')) ++i; //while(i < LEN && is_numeric(str[i]) || str[i] == '.') ++i;
      --i;
    }
    else if(str[i] == '('){
      if(!stk.empty()){
        V[stk.top()].push_back(cnt);
        V[cnt].push_back(stk.top());
      }
      stk.push(cnt++);
    }
    else if(str[i] == ')') stk.pop();
    else if(is_numeric(str[i])){
      int t1 = 0, t2=0;
      while(i+t1 < LEN && is_numeric(str[i+t1])) ++t1;
      t2 = atoi(str.substr(i,t1).c_str());
      idx[t2] = cnt;
			V[stk.top()].push_back(cnt);
			V[cnt++].push_back(stk.top());
		  i += (t1-1);
    }
  }
  if(RTD) root = 0; //Rooted trees are rooted at the given root
  else root = V[idx[1]][0]; //Unrooted trees are rooted at the node adjacent to taxa 1
  //Initialise cnt stuff here
  G = new vector <int>[cnt+5];
  leaf = new int[cnt+5], size = new int[cnt+5], minL = new int[cnt+5], maxL = new int[cnt+5], parent = new int[cnt+5];
	parent[root] = -1;
	memset(leaf,0,(cnt+5)*sizeof(int));
  for(int i=1;i<=N;++i) leaf[idx[i]] = i;
	stack <pair<int,int> > S;
  bool visit[cnt+5];
	memset(visit,0,sizeof(visit));
	S.push(pair<int,int>(root,-1));
  while(!S.empty()){
		pair<int,int> t1 = S.top();
		S.pop();
		visit[t1.first] = 1;
		++t1.second;
		while(t1.second < V[t1.first].size()){
			if(!visit[V[t1.first][t1.second]]){
				G[t1.first].push_back(V[t1.first][t1.second]);
				parent[V[t1.first][t1.second]] = t1.first;
				S.push(t1);
				S.push(pair<int,int>(V[t1.first][t1.second],-1));
				break;
			}
			else ++t1.second;
		}
	}
}

void tree::precompute(){
	//Initialising Values
	stack <pair<int,int> > S;
	bool labelled[N+5];
	memset(labelled,0,sizeof(labelled));
	memset(label,0,(N+5)*sizeof(int));
	memset(size,0,(cnt+5)*sizeof(int));
	memset(H,-1,(N+5)*sizeof(int));
	for(int i=0;i<cnt;++i){
		size[i] = 0, minL[i] = cnt+5, maxL[i] = -1;
		if(leaf[i] > 0) idx[leaf[i]] = i;
	}
	int num = 0;
	//Relabelling the leaves and hashing them using the perfect hashing described in Day's algorithm
	S.push(pair<int,int>(root,-1));
	while(!S.empty()){
		pair<int,int> t1 = S.top();
		S.pop();
		if(leaf[t1.first] > 0 && !labelled[leaf[t1.first]]){
			label[leaf[t1.first]] = minL[t1.first] = maxL[t1.first] = ++num;
			size[t1.first] = labelled[leaf[t1.first]] = 1;
		}
		++t1.second;
		if(t1.second < G[t1.first].size()){
			S.push(pair<int,int>(t1.first,t1.second));
			S.push(pair<int,int>(G[t1.first][t1.second],-1));
		}
		else{
			if(!leaf[t1.first]){
				if(!S.empty() && S.top().second == 0){
					minH[maxL[t1.first]] = minL[t1.first];
					maxH[maxL[t1.first]] = maxL[t1.first];
					H[maxL[t1.first]] = t1.first;
				}
				else{
					minH[minL[t1.first]] = minL[t1.first];
					maxH[minL[t1.first]] = maxL[t1.first];
					H[minL[t1.first]] = t1.first;
				}
			}
			if(!S.empty()){
				size[S.top().first] += size[t1.first];
				minL[S.top().first] = min(minL[S.top().first],minL[t1.first]);
				maxL[S.top().first] = max(maxL[S.top().first],maxL[t1.first]);
			}
		}
	}
	assert(num == N);
}

void tree::printNex(){
	stack<pair<int,int> > S;
	string ans = "";
	S.push(pair<int,int>(root,-1));
	while(!S.empty()){
		pair<int,int> t1 = S.top();
		S.pop();
		if(leaf[t1.first]){
			char str[10];
			sprintf(str,"%d",leaf[t1.first]);
			int L = strlen(str);
			for(int i=0;i<L;++i) ans += str[i];
			continue;
		}
		if(t1.second == -1) ans += '(';
		++t1.second;
		if(t1.second < G[t1.first].size()){
			if(t1.second > 0) ans += ',';
			S.push(t1);
			S.push(pair<int,int>(G[t1.first][t1.second],-1));
		}
		else ans += ')';
	}
	ans += ';';
	cout << ans << endl;
}
