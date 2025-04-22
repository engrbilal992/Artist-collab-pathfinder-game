/**
 ** main.cpp
 **
 ** Project Two: Six Degrees of Collaboration
 **
 ** Purpose:
 **   Entry point for the Six Degrees program. Handles command-line arguments,
 **   initializes the SixDegrees class, populates the collaboration graph,
 **   and processes commands as specified.
 **/

 #include <iostream>
 #include <fstream>
 #include <string>
 
 #include "SixDegrees.h"
 
 using namespace std;
 
 int main(int argc, char *argv[]) {
     // Validate number of command-line arguments
     if (argc < 2 || argc > 4) {
         cerr << "Usage: ./SixDegrees dataFile [commandFile] [outputFile]" << endl;
         return 1;
     }
     
     // Open data file
     ifstream data_file(argv[1]);
     if (!data_file.is_open()) {
         cerr << argv[1] << " cannot be opened." << endl;
         return 1;
     }
     
     SixDegrees six_degrees;
     
     // Populate graph
     try {
         six_degrees.populate_graph(data_file);
     } catch (const runtime_error &e) {
         throw; // Do not catch as per instructions
     }
     data_file.close();
     
     // Determine input source
     ifstream command_file;
     istream *input = &cin;
     
     if (argc >= 3) {
         command_file.open(argv[2]);
         if (!command_file.is_open()) {
             cerr << argv[2] << " cannot be opened." << endl;
             return 1;
         }
         input = &command_file;
     }
     
     // Determine output destination
     ofstream output_file;
     ostream *output = &cout;
     
     if (argc == 4) {
         output_file.open(argv[3]);
         if (!output_file.is_open()) {
             cerr << argv[3] << " cannot be opened." << endl;
             if (command_file.is_open()) {
                 command_file.close();
             }
             return 1;
         }
         output = &output_file;
     }
     
     // Run command loop
     six_degrees.run(*input, *output);
     
     // Clean up
     if (command_file.is_open()) {
         command_file.close();
     }
     if (output_file.is_open()) {
         output_file.close();
     }
     
     return 0;
 }