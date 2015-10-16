/*
FACT: Fast Algorithms for Consensus Trees (version 1.0)
Copyright (c) 2012 Jesper Jansson, Chuanqi Shen and Wing-Kin Sung

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstdlib>
#include <stack>
#include <ctime>
#include <sstream>
#include <sys/resource.h>
#define NUM_ALGO 11
#include "tree.h"
#include "wrapper.h"
#include "strict.h"
#include "majority.h"
#include "greedy.h"
#include "loose.h"
#include "adams.h"
#include "majorityplus.h"
using namespace std;

int numTrees,numTaxas,rooted,rule,pre,cur;
tree *T;
tree ans;
string input;
string consensus[NUM_ALGO] = {
  "Strict Consensus - O(kn)",

  "Majority Consensus - O(k^2 n)",
  "Majority Consensus - O(kn lg k)",
  "Majority Consensus - O(kn)",

  "Greedy Consensus - O(kn^2 + kn^3 + n^2)", 
  "Greedy Consensus - O(kn^2)",

  "Loose Consensus - O(k^2 n^3)",
  "Loose Consensus - O(kn)",
  
  "Majority-Rule(+) Consensus O(kn)",

  "Adams Consensus - O(kn^2)",
  "Adams Consensus - O(kn lg n)"
};

void endProg(string s){
	cout << s << endl;
	exit(0);
}

tree getConsensus(int x){
  tree ret;
  pre = clock();
  switch(x){
    case 0:
      ret = strictConsensus();
      break;
      
    case 1:
      ret = majorityConsensusSlow();
      break;
    case 2:
      ret = majorityConsensusFast();
      break;
    case 3:
      ret = majorityConsensusBest();
      break;
      
    case 4:
      ret = greedyConsensusSlow();
      break;
    case 5:
      ret = greedyConsensusFast();
      break;
    
    case 6:
      ret = looseConsensusSlow();
      break;
    case 7:
      ret = looseConsensusFast();
      break;
      
    case 8:
      ret = majorityPlusConsensus();
	break;    

    case 9:
      ret = adamsConsensusSlow();
      break;
    case 10:
      ret = adamsConsensusFast();
      break;
    
    default:
      break;
  }
  cur = clock();
  return ret;
}

int set_stack(long size){ // size is in term of MB
    const rlim_t kStackSize = size * 1024 * 1024;   // min stack size = 16 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
                exit(0);
            }
        }
    }
    return 0;
}

int main(int argc, char** argv){
  // set_stack(64);

  if(argc == 5 )
  {// ./prog -g k[tree_number] n[taxa_number] t[tree_type]
    // k : number of trees
    // n : number of leaves
    /*
        generate depth trees
        ./a -g 10 300 2

        generate binary tree
        ./a -g 10 300 1

        generate random tree
        ./a -g 10 300 0
     */
    int k, n, type;
    string sk(argv[2]); stringstream ssk(sk); ssk >> k;
    string sn(argv[3]); stringstream ssn(sn); ssn >> n;
    string st(argv[4]); stringstream sst(st); sst >> type;
    cout << "nexGen(k=" << k << ",n=" << n << ",type=" << type << ")" << endl;
    nexGen(k, n, type);
    return 0;
  }
  // ./a 
  // nex.txt 1536 0
  /*
    ran-k-n-100-300 1536 1
    ran-k-n-100-300.1 1536 1
    ran-k-n-100-300.2 1536 1
  */
	cin >> input >> rule >> rooted;
	nexReader(input);
	printf("%d Trees inputted, each with %d Leaves\n",numTrees,numTaxas);
	printf("-------------------------------------------------------------------------------------\n");
	for(int i=0;i<NUM_ALGO;++i){
    if( (rule&(1<<i)) > 0){
      ans = getConsensus(i);
      printf("%s Tree constructed, it has %d nodes\n",consensus[i].c_str(),ans.cnt);
      ans.printNex();
      printf("Time elapsed = %.3lf\n",(double)(cur-pre)/(double)CLOCKS_PER_SEC);
      printf("-------------------------------------------------------------------------------------\n");
    }
  }
}
