# **🚗 Strongly Connected Check**

## 📃 Problem Description

> Pending..

## 🏷️ Application Scenario

A traffic engineer has been given the task of improving the flow of traffic in a city. To do this, he has been given a map of the city defining all the intersections in the city and, for each block between two intersections, the direction of the section; that is, whether it is two-way or one-way, and which direction the one-way sections are. The map you've been given of the current organization guarantees that from any intersection in the city you can get to any other intersection. This is a fundamental requirement for the organization of the city's traffic, otherwise a citizen could be forced to disregard one-way streets in order to get home. The engineer has a proposal for how a reorganization of the hands could improve traffic flow; however, he can't guarantee that his proposal satisfies the fundamental requirement of allowing access to any intersection from any other intersection. So he has hired you, an experienced programmer, to assess whether his proposal satisfies this requirement or not. Your program must have a complexity limited to $O(n + m)$, where $n$ is the number of intersections and $m$ is the number of blocks.

### **Input**

The first line of each test case contains two integers $N$ and $M$, separated by a blank space, which represent, respectively, how many intersections $(2 ≤ N ≤ 300)$ and how many blocks between intersections there are in the city $(1 ≤ M ≤ 600)$. Each intersection is represented by an integer between 0 and N - 1. Each of the subsequent M lines of each test case contains three integers $A$, $B$ and $D$ $(0 ≤ A, B < N, 1 ≤ D ≤ 2)$, separated by a blank space, indicating that there is a block connecting intersections $A$ and $B$; if $D = 1$ then the stretch is one-way and goes from intersection $A$ to intersection $B$ and if $D = 2$ the stretch is two-way. For each pair of intersections $A$ and $B$ there is at most one block connecting them.

### **Output**

The output must be written to the standard output (terminal). The output must be a single line, containing the text `Appropriate.` whenever the engineer's proposal meets the fundamental requirement or the text `Inadequate.` otherwise.


## 🏷️ **Examples**

### Example 1

| Input |
|-------|
|10 18
0 1 2
1 2 1
1 3 1
1 4 2
2 3 1
4 0 1
5 1 1
5 7 2
6 1 1
6 2 1
6 3 1
6 9 1
7 4 1
7 8 2
8 0 1
8 9 2
9 3 1
9 5 2|
 
| Output |
|--------|
|Inadequate.|


### Example 2

| Input |
|-------|
|10 20
0 1 2
1 2 1
1 3 1
1 4 1
2 0 1
2 3 2
4 0 1
5 1 1
5 2 1
5 7 2
6 1 1
6 2 1
6 3 2
6 9 1
7 4 2
7 8 2
8 0 1
8 9 2
9 3 1
9 5 2 | 
 
| Output |
|--------|
|Appropriate.|