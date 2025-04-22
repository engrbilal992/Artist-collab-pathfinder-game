/**
 ** CollabGraph.h
 **
 ** Project Two: Six Degrees of Collaboration
 **
 ** Purpose:
 **   Represent a collaboration graph of artists, where each artist is
 **   a vertex and an edge between two artists is a song in which those
 **   artists have collaborated. Accessor and mutator functions are provided,
 **   allowing for convenient traversal of the graph.
 **
 ** Notes:
 **   1) Implemented with adjacency lists (not an adjacency matrix)
 **   2) The graph is undirected
 **   3) Non-existent edges are represented by getting a new empty string
 **   4) Artists with the empty string as their name are forbidden 
 **   5) Loops (edges from a vertex to itself) are forbidden
 **
 ** ChangeLog:
 **   17 Nov 2020: zgolds01
 **       CollabGraph class created
 **   April 2021: CR Calabrese
 **       Edited the CollabGraph to use an Edge struct & remove vector of
 **       pointers to all vertices.
 **       Edited function contracts to improve consistency with course style
 **       guide and other starter code.
 **
 **/

 #ifndef __COLLAB_GRAPH__
 #define __COLLAB_GRAPH__
 
 #include <string>
 #include <vector>
 #include <stack>
 #include <unordered_map>
 
 #include "Artist.h"
 
 class CollabGraph {
 
 public:
     CollabGraph();
     ~CollabGraph();
     CollabGraph(const CollabGraph &source);
     CollabGraph &operator=(const CollabGraph &rhs);
 
     void insert_vertex(const Artist &artist);
     void insert_edge(const Artist &a1, const Artist &a2,
                     const std::string &edgeName);
 
     void mark_vertex(const Artist &artist);
     void unmark_vertex(const Artist &artist);
     void set_predecessor(const Artist &to, const Artist &from);
     void clear_metadata();
 
     bool is_vertex(const Artist &artist) const;
     bool is_marked(const Artist &artist) const;
     Artist get_predecessor(const Artist &artist) const;
     std::string get_edge(const Artist &a1, const Artist &a2) const;
 
     std::vector<Artist> get_vertex_neighbors(const Artist &artist) const;
     std::stack<Artist> report_path(const Artist &source,
                                   const Artist &dest) const;
 
     void print_graph(std::ostream &out);
 
 private:
 
     struct Edge {
         Artist neighbor;
         std::string song;
 
         Edge(const Artist &neighbor, const std::string &song)
             : neighbor(neighbor), song(song) {}
     };
 
     struct Vertex {
         Artist artist;
         std::vector<Edge> neighbors;
         Vertex *predecessor;
         bool visited;
 
         Vertex(const Artist &artist) 
             : artist(artist), predecessor(nullptr), visited(false) {}
         Vertex() 
             : predecessor(nullptr), visited(false) {}
     };
 
     std::unordered_map<std::string, Vertex*> graph;
 
     void self_destruct();
     void enforce_valid_vertex(const Artist &artist) const;
 };
 
 #endif /* __COLLAB_GRAPH__ */