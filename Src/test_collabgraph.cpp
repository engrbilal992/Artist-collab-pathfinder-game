/**
 ** test_collabgraph.cpp
 **
 ** Project Two: Six Degrees of Collaboration
 **
 ** Purpose:
 **   Unit tests for CollabGraph class functions.
 **
 ** ChangeLog:
 **   April 2025: 
 **    
 **/

 #include <iostream>
 #include <cassert>
 #include <stack>
 #include <vector>
 
 #include "CollabGraph.h"
 #include "Artist.h"
 
 int main() {
     CollabGraph g;
 
     // Test 1: Insert vertices and check neighbors
     Artist a1("Ariana Grande");
     Artist a2("Nicki Minaj");
     Artist a3("Alicia Keys");
 
     g.insert_vertex(a1);
     g.insert_vertex(a2);
     g.insert_vertex(a3);
 
     std::vector<Artist> neighbors = g.get_vertex_neighbors(a1);
     assert(neighbors.empty()); // No edges yet
 
     // Test 2: Insert edges and verify neighbors
     g.insert_edge(a1, a2, "Bang Bang");
     g.insert_edge(a2, a3, "Girl On Fire (Inferno Version)");
 
     neighbors = g.get_vertex_neighbors(a2);
     assert(neighbors.size() == 2);
     bool found_a1 = false, found_a3 = false;
     for (const Artist &n : neighbors) {
         if (n == a1) found_a1 = true;
         if (n == a3) found_a3 = true;
     }
     assert(found_a1 && found_a3);
 
     // Test 3: Check edge retrieval
     assert(g.get_edge(a1, a2) == "Bang Bang");
     assert(g.get_edge(a2, a3) == "Girl On Fire (Inferno Version)");
     assert(g.get_edge(a1, a3) == "");
 
     // Test 4: Report path with no traversal
     std::stack<Artist> path = g.report_path(a1, a3);
     assert(path.empty()); // No predecessors set
 
     // Test 5: Report path after manual predecessor setup
     g.clear_metadata();
     g.mark_vertex(a1);
     g.mark_vertex(a2);
     g.mark_vertex(a3);
     g.set_predecessor(a2, a1);
     g.set_predecessor(a3, a2);
     path = g.report_path(a1, a3);
     assert(path.size() == 3);
     assert(path.top() == a1);
     path.pop();
     assert(path.top() == a2);
     path.pop();
     assert(path.top() == a3);
 
     std::cout << "All CollabGraph tests passed!\n";
     return 0;
 }