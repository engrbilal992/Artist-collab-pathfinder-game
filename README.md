# Artist Collab Pathfinder

A C++ program that models musical artist collaborations as an undirected graph and finds connection paths between artists based on shared songs. Supports search operations such as breadth-first search, depth-first search, and exclusion-based queries.

---

## Overview

**artist-collab-pathfinder** builds a collaboration graph where:
- **Vertices** represent musical artists.
- **Edges** represent songs they’ve collaborated on.

The program can determine paths between artists using different search strategies and outputs these paths in a clear, readable format.

---

## Features

- **Graph Construction**  
  Reads artist collaboration data from an input file and constructs an undirected graph.
  
- **Pathfinding Algorithms**  
  Supports:
  - Breadth-First Search (BFS) for shortest paths.
  - Depth-First Search (DFS) for exploratory paths.
  - Exclusion-based search to omit specific artists from results.

- **Flexible Input/Output**  
  Accepts command-line arguments for data input, command input, and output redirection.

---

## Build Instructions

```bash
make
```

This will produce an executable named:

```
SixDegrees
```

---

## Usage

```bash
./SixDegrees data.txt [commands.txt] [output.txt]
```

- `data.txt`: Required — contains artist collaboration data.
- `commands.txt`: Optional — file with commands to execute (if not provided, uses `stdin`).
- `output.txt`: Optional — output file for results (if not provided, outputs to `stdout`).

---

## Commands

- `bfs`  
  Find the shortest path between two artists.
- `dfs`  
  Find any valid path between two artists.
- `not`  
  Find the shortest path excluding a list of specified artists.
- `quit`  
  Terminate the program.

---

## Data Format

Input collaboration data should follow this structure:

```
Artist Name
Song Title
Song Title
*
```

Each artist's entry ends with an asterisk (`*`). Songs list collaborations that link this artist to others who also list the same song.

---

## Algorithms

- **BFS:** Queue-based, finds shortest path.
- **DFS:** Stack-based, finds any path.
- **Exclusion Search:** Modified BFS that ignores user-specified artists during traversal.

All implementations use adjacency lists for efficient lookup and traversal.

---

## Testing

Unit tests verify:
- Graph population and validation.
- Path discovery correctness.
- Error handling for unknown artists and invalid commands.
- Exclusion logic during searches.

Test files include: (To test units involved)
- `test_collabgraph.cpp`  
- `test_sixdegrees.cpp`  
- Example command and data files (`basic_commands.txt`, `edge_cases.txt`).

---

## Dependencies

- C++11 or later.
- Standard Template Library (STL).

---

## Notice

- This was a freelance project intended for educational purposes only.

---
