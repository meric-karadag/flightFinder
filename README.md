# Flight Finder Application

This project implements a flight finder application similar to popular services like flightsfinder.com and skyscanner.net. The application utilizes C++ and focuses on implementing a MultiGraph data structure for representing flight connections and a Hash Table for efficient caching of frequently queried flight paths.

## MultiGraph Implementation 

The MultiGraph data structure is used to represent a directional, non-negative weighted multigraph with parallel edges and a string identifier for each edge representing the airline name. The implementation follows the adjacency list style with some modifications.

Creating a MultiGraph

```cpp
// Creating an empty graph
MultiGraph flightGraph;

// Creating a graph from a file
MultiGraph flightGraphFromFile("flightMap.map");
```

#### 1. GraphVertex Struct 
GraphVertex holds information about a vertex, including a list of edges and a unique name.
#### 2. GraphEdge Struct
GraphEdge holds information about an edge, including the edge name, two weights, and the index of the end vertex. 
#### 3. MultiGraph Class
+ **MultiGraph** implements a directional, non-negative weighted multigraph.
+ Constructor: Creates an empty graph.
+ Constructor with file input: Reads a ".map" file and constructs the graph.
+ **InsertVertex:** Inserts a vertex.
```cpp
// Inserting a vertex
flightGraph.InsertVertex("AirportA");
```
+ **RemoveVertex:** Removes a vertex.
+ **AddEdge:** Adds an edge between two vertices.
```cpp
// Adding an edge between two vertices
flightGraph.AddEdge("Flight1", "AirportA", "AirportB", 100.0, 2.5);
```
+ **RemoveEdge:** Removes an edge between two vertices.
+ **HeuristicShortestPath:** Finds the shortest path with a heuristic weight.
```cpp
// Finding the shortest path with heuristic weight
std::vector<int> pathIndices;
bool pathExists = flightGraph.HeuristicShortestPath(pathIndices, "AirportA", "AirportB", 0.3);
```
+ **FilteredShortestPath:** Finds the shortest path with filtered edges.
+ **BiDirectionalEdgeCount:** Returns the count of bidirectional edges.
+ **MaxDepthViaEdgeName:** Returns the maximum depth using a specific edge.
+ **PrintPath:** Prints the path between vertices.
+ **Lerp:** Combines weights for shortest path calculations.
#### 4 Exceptions
Exception classes provided for handling graph-related errors.

## Hash Table
The project also includes a compile-time sized **HashTable** class for efficient caching of frequently queried flight paths. The hash table uses quadratic probing for collision resolution and includes LRU counters for eviction.

Initializing a hash table
```cpp
// Creating a hash table with a maximum size of 97
HashTable<97> pathCache;
```
#### 1. HashTable Class
+ **HashTable** stores key-value pairs with a maximum size defined at compile-time.
+ Constructor: Initializes the hash table.
+ **Hash:** Hash function for key calculation.
+ **Insert:** Inserts an entry into the hash table.
```cpp
// Inserting an entry into the hash table
pathCache.Insert({1, 2, 3}, true);
```
+ **Find:** Finds an entry in the hash table.
 ```cpp 
 // Inserting an entry into the hash table
pathCache.Insert({1, 2, 3}, true);
```
+ **Remove:** Removes an entry from the hash table.
+ ```cpp```
+ **RemoveLRU:** Removes the least recently used entries.
+ **InvalidateTable:** Clears the entire hash table.
+ **GetMostInserted:** Returns data of the most inserted entry.
+ **PrintSortedLRUEntries:** Prints entries with non-zero LRU counters.
+ **PrintTable:** Prints the entire hash table.
## Flight Finder 
The FlightFinder class combines the MultiGraph and HashTable classes to provide functionalities for finding flights, halting/resuming flights, and caching results.

Initializing flightFinder

```cpp
// Initializing CENGFlight with a flight map file
flightFinder flightSystem("flightMap.map");
```
#### 1. FlightFinder Class
Constructor: Initializes the flight map and hash table.
+ **HaltFlight:** Temporarily removes a flight edge.
+ **ContinueFlight:** Re-enables a previously halted flight.
 ```cpp
 // Halting a flight temporarily
flightSystem.HaltFlight("AirportA", "AirportB", "AirlineX");

// Continuing a halted flight
flightSystem.ContinueFlight("AirportA", "AirportB", "AirlineX");
```
+ **FindFlight:** Finds the shortest flight with given parameters and caches results.
 ```cpp
 // Finding and caching the shortest flight
flightSystem.FindFlight("AirportA", "AirportB", 0.8);
 ```
+ **FindSpecificFlight:** Finds a specific flight with filtering and prints results.
```cpp
// Finding a specific flight with filtering
flightSystem.FindSpecificFlight("AirportA", "AirportB", 0.8, {"AirlineX", "AirlineY"});
```
+ **FurthestTransferViaAirline:** Returns the maximum transfer amount using a specific airline.
```cpp
// Finding the maximum transfer using a single airport and airline
int maxTransfer = flightSystem.FurthestTransferViaAirline("AirportA", "AirlineX");
```
+ **PrintMap:** Prints the entire flight graph for debugging.
+ **PrintCache:** Prints the cached flight paths.

Note: Print functions are provided for consistency and error handling is implemented internally.

#### Caching Rules: 
+ Paths with alpha equal to zero or one are cached.
+ Blended paths (0 < alpha < 1) are not cached. Because they are specific
