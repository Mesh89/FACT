#!/usr/bin/python

import sys

if __name__ == "__main__":
  fname = sys.argv[len(sys.argv)-1]
  f = open(fname, 'a')
  subfnames = sys.argv[1:len(sys.argv)-1]
  for subfname in subfnames:
    subf = open(subfname, 'r')
    f.write(subf.read())
    subf.close()
    # input subf to end of fname
  f.close()

# argv store arg array