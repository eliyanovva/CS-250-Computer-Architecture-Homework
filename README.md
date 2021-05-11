# CS-250-Computer-Architecture-Homework
This repository contains some of my work for CS 250: Computer Architecture taught at Duke by Prof. Sorin.

homework 1: 
covidtracker.c - a C implementation of a binary search tree which is stored on the heap, and is printed using a DFS algorithm.

homework 2:
hw2_2.s - a MIPS implementation of a simple recursive function.
covidtracker.s - a MIPS implementation of the problem in homework 1. Has a severe logical error, which prevents the tree from storing correctly more than a few words.

homework 3:
adder.circ - logisim implementation of a 16-bit adder. Outputs A+B if the sub input is 0, and B-A if the sub input is 1.
fsm.circ - an implementation of a finite state machine which outputs 1 if the two inputs were identical only once in the last two cycles, and 0 in any other case.

homework 4:
hw4.circ - a single-core CPU which runs MIPS assembly. 

homework 5:
cachesim.c - a C simulation of a cache whose configuration is provided in the command line:
  name of the file which holds the loads and stores
  size in KB
  associativity
  "wb" if the cache is writeback and write-allocate or "wt" if the cache is writethrough and write-no-allocate.
  block size in bytes.
 The simulation reads and executes store and load commands, while also printing in the console if the command was a hit or miss.
