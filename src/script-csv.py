#!/usr/bin/python

import sys

if __name__ == "__main__":
  fname=sys.argv[1]
  for line in file(fname):
    t = line.find("k=")
    if t > -1:# , 100 , binary tree k= n= ,,
      print ","+line[line.find("n=")+2:len(line)-1]+","+line[:len(line)-1]+',,'
    t = line.find("elapsed")
    if t > -1:
      print ',,'+line[line.find("=")+2:len(line)-1]+','+line[line.find("=")+2:len(line)-1]+','
    t = line.find("--")
    if t > -1:
      pass
    

# argv store arg array