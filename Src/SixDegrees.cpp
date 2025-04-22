/**
 ** SixDegrees.cpp
 **
 ** Project Two: Six Degrees of Collaboration
 **
 ** Purpose:
 **   Implements the SixDegrees class to manage a collaboration graph of artists,
 **   process commands (bfs, dfs, not, quit), and output paths of collaborations
 **   between artists based on the provided input.
 **
 **/

 #include <iostream>
 #include <fstream>
 #include <string>
 #include <vector>
 #include <queue>
 #include <stack>
 #include <sstream>
 
 #include "SixDegrees.h"
 #include "Artist.h"
 #include "CollabGraph.h"
 
 using namespace std;
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * @function: constructor
  * @purpose: initialize a SixDegrees instance
  *
  * @preconditions: none
  * @postconditions: a new SixDegrees instance is created with an empty collaboration graph
  *
  * @parameters: none
  * @returns: none
  */
 SixDegrees::SixDegrees() {
     // Initialize empty CollabGraph
 }
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * @function: populate_graph
  * @purpose: build the collaboration graph from input data
  *
  * @preconditions: none
  * @postconditions: the collaboration graph is populated with vertices (artists) and edges (songs) based on the input data
  *
  * @parameters: a std::istream reference, the input stream containing artist names, songs, and '*' delimiters
  * @returns: none
  */
 void SixDegrees::populate_graph(istream &data_stream) {
     string line;
     Artist current_artist;
     vector<Artist> artists;
     vector<pair<Artist, vector<string>>> artist_songs;
 
     // Step 1: Parse input to collect artists and their songs
     while (getline(data_stream, line)) {
         if (line.empty()) continue;
 
         if (line == "*") {
             if (!current_artist.get_name().empty()) {
                 // Store the artist and their songs
                 graph_.insert_vertex(current_artist);
                 vector<string> songs;
                 // Songs are already added via add_song; we'll handle edges later
                 artists.push_back(current_artist);
                 artist_songs.emplace_back(current_artist, songs);
                 current_artist = Artist();
             }
         } else if (current_artist.get_name().empty()) {
             current_artist.set_name(line);
         } else {
             current_artist.add_song(line);
         }
     }
 
     // Handle the last artist if file doesn't end with '*'
     if (!current_artist.get_name().empty()) {
         graph_.insert_vertex(current_artist);
         artists.push_back(current_artist);
         artist_songs.emplace_back(current_artist, vector<string>());
     }
 
     // Step 2: Create edges by checking collaborations
     for (size_t i = 0; i < artists.size(); ++i) {
         Artist a1 = artists[i];
         for (size_t j = i + 1; j < artists.size(); ++j) {
             Artist a2 = artists[j];
             string song = a1.get_collaboration(a2);
             if (!song.empty() && a1 != a2) {
                 try {
                     graph_.insert_edge(a1, a2, song);
                 } catch (const runtime_error &e) {
                     throw; // Propagate as per instructions
                 }
             }
         }
     }
 }
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * @function: run
  * @purpose: process commands from an input stream and write results to an output stream
  *
  * @preconditions: the collaboration graph should be populated
  * @postconditions: commands are processed, and results are written to the output stream
  *
  * @parameters: 1) a std::istream reference, the input stream containing commands
  *             2) a std::ostream reference, the output stream for results
  * @returns: none
  */
 void SixDegrees::run(istream &input, ostream &output) {
     string command;
     
     while (getline(input, command)) {
         if (command.empty()) continue;
         process_command(command, input, output);
     }
 }
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * @function: process_command
  * @purpose: execute a single command (bfs, dfs, not, quit) with source and destination artists
  *
  * @preconditions: the collaboration graph should be populated
  * @postconditions: the command is executed, and results are written to the output stream
  *
  * @parameters: 1) a const std::string reference, the command to execute
  *             2) a std::istream reference, the input stream for additional command data
  *             3) a std::ostream reference, the output stream for results
  * @returns: none
  */
 void SixDegrees::process_command(const string &command, 
                             istream &input, 
                             ostream &output) {
     if (command == "quit") {
         return;
     }
     
     string source_name, dest_name;
     getline(input, source_name);
     if (source_name.empty()) return;
     
     getline(input, dest_name);
     if (dest_name.empty()) return;
     
     // Validate artists
     bool source_valid = is_valid_artist(source_name, output);
     bool dest_valid = is_valid_artist(dest_name, output);
     
     if (!source_valid || !dest_valid) {
         // Skip further processing if artists are invalid
         if (command == "not") {
             // Consume exclude list until '*'
             string line;
             while (getline(input, line) && line != "*") {
                 if (!line.empty()) {
                     is_valid_artist(line, output);
                 }
             }
         }
         return;
     }
     
     Artist source(source_name);
     Artist dest(dest_name);
     
     if (command == "bfs") {
         bfs(source, dest, output);
     } else if (command == "dfs") {
         dfs(source, dest, output);
     } else if (command == "not") {
         vector<Artist> exclude;
         string line;
         while (getline(input, line) && line != "*") {
             if (!line.empty()) {
                 if (is_valid_artist(line, output)) {
                     exclude.emplace_back(line);
                 }
             }
         }
         not_search(source, dest, exclude, output);
     } else {
         output << "\"" << command << "\" is not a valid command." << endl;
     }
 }
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * @function: is_valid_artist
  * @purpose: determine whether an artist exists in the collaboration graph
  *
  * @preconditions: none
  * @postconditions: none
  *
  * @parameters: 1) a const std::string reference, the name of the artist to check
  *             2) a std::ostream reference, the output stream for error messages
  * @returns: a bool, true if the artist is in the graph, false otherwise
  */
 bool SixDegrees::is_valid_artist(const string &name, ostream &output) {
     Artist artist(name);
     if (!graph_.is_vertex(artist)) {
         output << "\"" << name << "\" was not found in the collaboration graph." << endl;
         return false;
     }
     return true;
 }
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * @function: bfs
  * @purpose: find a collaboration path from source to destination using breadth-first search
  *
  * @preconditions: both source and destination artists must be in the collaboration graph
  * @postconditions: a path from source to destination is printed if one exists
  *
  * @parameters: 1) a const Artist reference, the source artist
  *             2) a const Artist reference, the destination artist
  *             3) a std::ostream reference, the output stream for the path
  * @returns: none
  */
 void SixDegrees::bfs(const Artist &source, const Artist &dest, ostream &output) {
     graph_.clear_metadata();
     
     queue<Artist> queue;
     queue.push(source);
     graph_.mark_vertex(source);
     
     while (!queue.empty()) {
         Artist current = queue.front();
         queue.pop();
         
         if (current == dest) {
             std::stack<Artist> path = graph_.report_path(source, dest);
             print_path(path, output);
             return;
         }
         
         vector<Artist> neighbors = graph_.get_vertex_neighbors(current);
         for (const Artist &neighbor : neighbors) {
             if (!graph_.is_marked(neighbor)) {
                 graph_.mark_vertex(neighbor);
                 graph_.set_predecessor(neighbor, current);
                 queue.push(neighbor);
             }
         }
     }
     
     output << "No path exists from \"" << source.get_name() 
         << "\" to \"" << dest.get_name() << "\"." << endl;
 }
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * @function: dfs
  * @purpose: find a collaboration path from source to destination using depth-first search
  *
  * @preconditions: both source and destination artists must be in the collaboration graph
  * @postconditions: a path from source to destination is printed if one exists
  *
  * @parameters: 1) a const Artist reference, the source artist
  *             2) a const Artist reference, the destination artist
  *             3) a std::ostream reference, the output stream for the path
  * @returns: none
  */
 void SixDegrees::dfs(const Artist &source, const Artist &dest, ostream &output) {
     graph_.clear_metadata();
     
     std::stack<Artist> artist_stack;
     artist_stack.push(source);
     graph_.mark_vertex(source);
     
     while (!artist_stack.empty()) {
         Artist current = artist_stack.top();
         artist_stack.pop();
         
         if (current == dest) {
             std::stack<Artist> path = graph_.report_path(source, dest);
             print_path(path, output);
             return;
         }
         
         vector<Artist> neighbors = graph_.get_vertex_neighbors(current);
         for (const Artist &neighbor : neighbors) {
             if (!graph_.is_marked(neighbor)) {
                 graph_.mark_vertex(neighbor);
                 graph_.set_predecessor(neighbor, current);
                 artist_stack.push(neighbor);
             }
         }
     }
     
     output << "No path exists from \"" << source.get_name() 
         << "\" to \"" << dest.get_name() << "\"." << endl;
 }
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * @function: not_search
  * @purpose: find a collaboration path from source to destination using BFS, avoiding excluded artists
  *
  * @preconditions: both source and destination artists must be in the collaboration graph
  * @postconditions: a path from source to destination, avoiding excluded artists, is printed if one exists
  *
  * @parameters: 1) a const Artist reference, the source artist
  *             2) a const Artist reference, the destination artist
  *             3) a std::vector<Artist> reference, the list of artists to exclude
  *             4) a std::ostream reference, the output stream for the path
  * @returns: none
  */
 void SixDegrees::not_search(const Artist &source, 
                         const Artist &dest, 
                         vector<Artist> &exclude, 
                         ostream &output) {
     graph_.clear_metadata();
     
     // Mark excluded artists as visited to skip them
     for (const Artist &artist : exclude) {
         if (graph_.is_vertex(artist)) {
             graph_.mark_vertex(artist);
         }
     }
     
     // Perform BFS
     queue<Artist> queue;
     queue.push(source);
     graph_.mark_vertex(source);
     
     while (!queue.empty()) {
         Artist current = queue.front();
         queue.pop();
         
         if (current == dest) {
             std::stack<Artist> path = graph_.report_path(source, dest);
             print_path(path, output);
             return;
         }
         
         vector<Artist> neighbors = graph_.get_vertex_neighbors(current);
         for (const Artist &neighbor : neighbors) {
             if (!graph_.is_marked(neighbor)) {
                 graph_.mark_vertex(neighbor);
                 graph_.set_predecessor(neighbor, current);
                 queue.push(neighbor);
             }
         }
     }
     
     output << "No path exists from \"" << source.get_name() 
         << "\" to \"" << dest.get_name() << "\"." << endl;
 }
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * @function: print_path
  * @purpose: output a collaboration path from destination to source
  *
  * @preconditions: the path stack should be valid, with predecessors set by a traversal
  * @postconditions: the path is printed to the output stream in destination-to-source order
  *
  * @parameters: 1) a std::stack<Artist> value, the path from source to destination
  *             2) a std::ostream reference, the output stream for the path
  * @returns: none
  */
 void SixDegrees::print_path(std::stack<Artist> path, ostream &output) {
     if (path.empty()) {
         output << "No path exists." << endl;
         return;
     }
     
     vector<Artist> artists;
     while (!path.empty()) {
         artists.push_back(path.top());
         path.pop();
     }
     
     if (artists.size() < 2) {
         output << "No path exists." << endl;
         return;
     }
     
     // Reverse to print from source to dest
     for (size_t i = artists.size() - 1; i > 0; --i) {
         string song = graph_.get_edge(artists[i], artists[i-1]);
         output << "\"" << artists[i].get_name() << "\" collaborated with "
             << "\"" << artists[i-1].get_name() << "\" in "
             << "\"" << song << "\"." << endl;
     }
 }