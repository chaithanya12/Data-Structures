Patrick Drumm, pdrumm

Programming Assignment 5
========================

_Your description here_

For this programming assignment, I first built off of the data structures that I used for PG4. The data structures which I used for PG5 from PG4 are two structs and an unordered_map. The structs created were *side* and *segment*. The unordered_map then used a *side* as the key and a vector of *segments* as the value.

The additional data structures that I used for PG5 are another struct and unordered_map. The struct I created was *edge*, which is used to keep track of relevant data within the undirected graph needed for this assignment. The unordered map was thus the undirected graph. This was called 'adjacency_list' and was used to map a node(int) to a vector of all of its *edges*.

This programming assignment required me to implement Dijkstra's Algorithm (which was probably the most difficult part of this assignment for me) to find the shortest path from one node to another in our graph. In doing so, I needed to additionally use a priority_queue and an unordered_map. I used the standard library priority queue to store all of the nodes on the frontier. To do so, I utilized my *edge* constructor, and took advantage of edge.length by storing the total distance from the source node there. By using the std::priority queue, there was not an easy way to use an increase_key function, so I simply allowed 'duplicates' in the frontier. Thus, I had to check every *edge* popped off of the frontier to make sure that it was not already 'marked'.

The unordered_map used for Dijkstra's was the group of *marked* nodes. This unordered_map mapped the destination node as the previous hop/node as the value. I used a map for this because the destination node should always be a unique value in *marked*, while the previous hop/node will likely occur multiple times in *marked*. 

After using Dijkstra's to find the shortest path, I then used the 'node pairs' stored in *marked* in conjunction with the *edge* information stored in adjacency_list to construct the steps required to get from the SOURCE to the TARGET as quickly as possible.

_Grader comments here_
