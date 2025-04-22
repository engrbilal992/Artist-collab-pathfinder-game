/**
 ** SixDegrees.h
**
** Project Two: Six Degrees of Collaboration
**
** Purpose:
**   Implements the SixDegrees class to manage a collaboration graph of artists,
**   process commands (bfs, dfs, not, quit), and output paths of collaborations
**   between artists based on the provided input.
**
**/

#ifndef __SIX_DEGREES__
#define __SIX_DEGREES__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>

#include "CollabGraph.h"

class SixDegrees {
public:
    SixDegrees();
    
    // Populate the graph from a data file
    void populate_graph(std::istream &data_stream);
    
    // Run the command loop, reading from input and writing to output
    void run(std::istream &input, std::ostream &output);
    
private:
    CollabGraph graph_;
    
    // Process individual commands
    void process_command(const std::string &command, 
                        std::istream &input, 
                        std::ostream &output);
    
    // Traversal algorithms
    void bfs(const Artist &source, const Artist &dest, std::ostream &output);
    void dfs(const Artist &source, const Artist &dest, std::ostream &output);
    void not_search(const Artist &source, 
                    const Artist &dest, 
                    std::vector<Artist> &exclude, 
                    std::ostream &output);
    
    // Output path formatting
    void print_path(std::stack<Artist> path, std::ostream &output);
    
    // Check if artist exists and handle errors
    bool is_valid_artist(const std::string &name, std::ostream &output);
};

#endif /* __SIX_DEGREES__ */