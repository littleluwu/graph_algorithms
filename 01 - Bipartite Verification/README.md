# **🧪 Bipartite Verification**

## 📃 Problem Description

A graph is bipartite when its vertices can be divided into two disjoint and independent sets, and each edge only connects vertices from distinct sets. A particular characteristic of this class of graphs is that they will never contain an odd cycle, given that an odd cycle can't satisfy the bipartite definition. On cycles, the path with $m-1$ edges contained in the cycle must have the starting vertex and the ending vertex belonging to different vertex sets, then the edge that closes the cycle can respect the bipartite definition. Trying to divide an odd cycle into two disjoint sets, you will end up with a path containing an even number of edges, making the ends of the path belong to the same vertex set, thus breaking the definition of bipartite if we add the last edge and close the cycle. The other direction is also valid, a graph is only bipartite if it contains no odd cycle, granted by the **Kőnig's theorem**.

Thus, to determine if a graph is bipartite, we can use a depth-first search to check for odd cycles. A simple way to do this is to color the search tree while we traverse through the graph. Given an edge linking to a vertex already in the search tree, we have found a cycle and need to check if it's odd or even.  If they belong to the same set they will have the same color in the search tree, this edge closes an odd cycle, and thus the graph is not bipartite. If they belong to different sets, their color will be different, and the edge closes an even cycle, which is bipartite itself.

## 🏷️ Application Scenario

A chemical company is planning a new factory with the capacity for two separate production lines for chemical compounds. Each production line can produce several compounds, as long as it is equipped with specific machinery and there is adequate planning for the production of each one.

There is, however, an additional worrying issue to consider when planning. Some compounds are volatile, or leave solid residues on machinery, and certain combinations of compounds can react and cause problems, from corrosion of equipment to explosions. When planning a new plant, the industry must take into account the incompatibility of producing certain compounds on the same production line in order to understand whether it is possible to produce all the desired compounds. You have been hired to help the industry with this planning by building a program that solves the following problem: given the compounds you want to produce in the plant and a list of compounds that are incompatible, determine whether two production lines are sufficient to produce all the desired compounds without risk. Your program must have a complexity limited to $O(n^2)$, where $n$ is the number of compounds.

### **Input**

The first line of each test case contains two integers $N$ and $M$, separated by a blank space, which represent, respectively, how many compounds you want to produce in the factory
$(1 ≤ N ≤ 600)$ and how many incompatibility relationships there are between them $(1 ≤ M ≤ 10,000)$. Each compound is represented by an integer between $0$ and $N - 1$. Each of the subsequent $M$ lines of each test case contains two integers $A$ and $B$ (0 ≤ A, B ≤ N - 1)$, separated by a blank space, indicating that compound $A$ is incompatible with compound $B$ and vice versa.

### **Output**

The output must be written to the standard output (terminal). The output must be a single line with the answer `"It is possible to produce with two production lines."` or `"It is not possible to produce with two production lines."`. Make sure there is a line break at the end of the sentence.

##  **Examples**

### Example 1

| Input |
|-------|
| 6 9  
 0 1 
 0 3 
 0 5 
 2 1 
 2 3 
 2 5 
 4 1 
 4 3 
 4 5 
 
| Output |
|--------|
|It is possible to produce with two production lines.|


### Example 2

| Input |
|-------|
|11 13 
 0 1
 0 2
 0 3
 0 4
 1 2
 1 5
 3 6
 4 7
 5 8
 5 9
 7 8
 8 9
 8 10| 
 
| Output |
|--------|
|It is not possible to produce with two production lines.|

# References