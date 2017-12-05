Sara Warren
srw0207
4110s001 
sp17proj

README
***************************************************************
How to compile: 
>standard compilation, no flags needed to compile

g++ -o projSaraWarren projSaraWarren.cpp

***************************************************************
How to execute:
>This program can run with or without any arguments
>This program only takes in one file at a time

./projSaraWarren
---OR---
./projSaraWarren filename

>no arguments tells the program to generate graphs
>arguments tells the program to read from file
***************************************************************
File input format:
>First line must be the number of verticies and nothing else
>Cost matrix values must be separated by a space
>input file may be named anything you wish, as long as it is readable

Input file example:

6
0 1 2 3 4 5
1 0 6 7 8 9
2 6 0 10 11 12
3 7 10 0 13 14
4 8 11 13 0 15
5 9 12 14 15 0

***************************************************************
File output format:
>2 files will be created
>paths.txt will contain a shortest path matrix of each algorithm 

--paths.txt will look like this

Graph: filename
Type: file
Algorithm type: Dijkstra or Floyd Warshall
# of verticies: 50
Vertex: 0
	List of shortest path costs for starting a vertex 0...
Vertex: 1
	List of shortest path costs for starting a vertex 1...

--

>results.txt will contain information on variable name, cpu runtime, 
graph generation type, algorithm type, and # of verticies of graph

--results.txt will look like this

Graph: name
Time: 5.36499999999993
Type: reverse
Algorithm type: dijkstra
# of verticies: 1000
--------------------------------------------

Graph: name
Time: 0.424
Type: reverse
Algorithm type: dijkstra
# of verticies: 500
--------------------------------------------
--

***************************************************************


