#include "strict.h"

extern tree *T;
extern int numTaxas,numTrees,rooted;

tree strictConsensus(){
	//Initialising T[0]
	T[0].precompute();
	for(int i=1;i<=T[0].N;++i) T[0].good[i] = (T[0].H[i] >= 0);
	//Determining which nodes in T[0] exist in all the trees. These nodes will form the strict consensus tree
	for(int i=1;i<numTrees;++i) days(T[0],T[i]);
	//Contract the tree so that it only contains vertices identified in the previous step
	tree ret = contract(T[0]);
	return ret;
}

void days(tree &A, tree &B){
	//Initialise data structures
	stack <pair<int,int> > S;
	bool good[A.N+5];
	memset(good,0,sizeof(good));
	for(int i=0;i<B.cnt;++i) B.minL[i] = A.cnt+5,B.maxL[i] = -1, B.size[i] = 0;
	//Relabelling the leaves of B with the corresponding labels of the leaves of A.
	S.push(pair<int,int>(B.root,-1));
	while(!S.empty()){
		pair<int,int> t1 = S.top();
		S.pop();
		if(B.leaf[t1.first])
			B.minL[t1.first] = B.maxL[t1.first] = A.label[B.leaf[t1.first]]; //Relabel nodes of B with A labels 
		++t1.second;
		if(t1.second < B.G[t1.first].size()){
			S.push(pair<int,int>(t1.first,t1.second));
			S.push(pair<int,int>(B.G[t1.first][t1.second],-1));
		}
		else{
			//Checks whether an internal node in A has the same hash value
			if(B.leaf[t1.first]) ++B.size[t1.first];
			if(!S.empty()){
				B.size[S.top().first] += B.size[t1.first];
				B.minL[S.top().first] = min(B.minL[S.top().first],B.minL[t1.first]);
				B.maxL[S.top().first] = max(B.maxL[S.top().first],B.maxL[t1.first]);
			}
			if(B.leaf[t1.first]) continue;
			int LC = B.minL[t1.first], RC = B.maxL[t1.first];
			if(A.minH[LC] == LC && A.maxH[LC] == RC && A.maxH[LC] - A.minH[LC] + 1 == B.size[t1.first]) good[LC] = 1;
			if(A.minH[RC] == LC && A.maxH[RC] == RC && A.maxH[RC] - A.minH[RC] + 1 == B.size[t1.first]) good[RC] = 1;
		}
	}
	//Updates the boolean array in A
	for(int i=1;i<=A.N;++i) A.good[i] &= good[i];
}

tree contract(tree A){
	//Initialise data structures
	bool keep[A.cnt+5];
	int label[A.cnt+5], anc[A.cnt+5],tmp = A.N;
	tree ret;
	memset(keep,0,sizeof(keep));
	memset(label,-1,sizeof(label));
	memset(anc,-1,sizeof(anc));
	//Mark all the good vertices that need to be kept
	for(int i=1;i<=A.N;++i){
		keep[A.idx[i]] = 1;
		if(A.good[i]) keep[A.H[i]] = 1, ++tmp;
	}
	ret = tree(A.N,tmp,rooted);
	tmp = 0;
	for(int i=0;i<A.cnt;++i){
		if(keep[i]) label[i] = tmp++;
		if(A.leaf[i]) ret.leaf[label[i]] = A.leaf[i];
	}
	ret.root = label[A.root];
	stack <pair<int,int> > S;
	S.push(pair<int,int>(A.root,-1));
	//Run a DFS through the tree. For each good vertex x, we find the lowest ancestor y such that y is also good. Then we we set as the parent of x.
	while(!S.empty()){
		pair<int,int> t1 = S.top();
		S.pop();
		if(t1.second == -1){
			if(t1.first != A.root && keep[t1.first]) 
				ret.G[label[anc[t1.first]]].push_back(label[t1.first]);
			if(keep[t1.first]) anc[t1.first] = t1.first;
			else anc[t1.first] = anc[anc[t1.first]];
		}
		++t1.second;
		if(t1.second < A.G[t1.first].size()){
			S.push(t1);
			S.push(pair<int,int>(A.G[t1.first][t1.second],-1));
			anc[A.G[t1.first][t1.second]] = anc[t1.first];
		}
	}
	for(int i=0;i<ret.cnt;++i)
		for(vector<int>::iterator it=ret.G[i].begin();it!=ret.G[i].end();++it) ret.parent[*it] = i;
	//Precompute the information in the new tree
	ret.precompute();
	return ret;
}

