#!/usr/bin/env python3

import re

filename = "rules.txt"
regexp = re.compile(r'([abcd][0-9]{1,2}),([0-9]{1,2})[ ]*=[ ]*([0-1])|([abcd][0-9]{1,2})')

def handleLine(line):
    ones = []
    zeros = []
    assigns = []
    
    exprs = line.split(sep=", ")
    for e in exprs:
      t = e.split(sep=" = ")
      if t[1] == "1":
        ones.append(t[0])
      elif t[1] == "0":
        zeros.append(t[0])
      else:
        assigns.append(t[1])
    
    oneD = dict()
    zeroD = dict()
    mis = dict()

    for e in ones:
        name,index = e.split(",")
        index = int(index)
        if name not in oneD.keys():
            oneD[name] = 0
        val = oneD[name]
        val += 2**(index-1)
        oneD[name] = val
    
    for e in zeros:
        name,index = e.split(",")
        index = int(index)
        if name not in zeroD.keys():
            zeroD[name] = 0
        val = zeroD[name]
        val += 2**(index-1)
        zeroD[name] = val

    for name in assigns:
        if name not in mis.keys():
            mis[name] = ""
        val = mis[name]
        val += f"{e}"
        mis[name] = val
    
    one = dict()
    for el in oneD.keys():
        one[el] = hex(oneD[el])

    zero = dict()
    for el in zeroD.keys():
        zero[el] = hex(zeroD[el])

    print(f"ones: {one}, zeros: {zero}, assigns: {mis}")   

    

with open(filename) as f:
    for line in f.readlines():
        handleLine(line)