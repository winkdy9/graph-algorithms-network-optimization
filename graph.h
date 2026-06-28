#ifndef GRAPH_H
#define GRAPH_H

#include "create_Vector_List/vector/generic.h"
#include "create_HashTable/hash_table/generic.h"

typedef struct GEdge { 
    long long from;
    long long to;
    double lenght;
    int oneway;
    char name[256];
    struct GEdge* next;
} GEdge;

typedef struct GNode {
    long long node_id;
    double lat;
    double lon;
    GEdge* edges; 
} GNode;

typedef struct Graph {
    HashTable* nodes; 
} Graph;

typedef struct DNode {
    long long id;
    double dist;
    long long prev;
    int visited;
} DNode;

Graph* createGraph();
void addNode(Graph* g, long long id, double lat, double lon);
void addEdge(Graph* g, long long from, long long to, double len, int oneway, char* name);
void freeGraph(Graph* graph);
GNode* getNode(Graph* g, long long id);

void read_csv_nodes(Graph* graph, char* path);
void read_csv_edges(Graph* graph, char* path);

long findMin(Vector* dnodes);

Vector* buildPath(Vector* dnodes, long long start, long long end);
Vector* dijkstra(Graph* graph, long long start, long long end);
long long* Dijkstra(Graph* graph, long long start, long long end, int* out_len);
double dist2(double lat1, double lon1, double lat2, double lon2);

long long findNearest(Graph* graph, double lat, double lon);

int readFile(char* path, double* lat1, double* lon1, double* lat2, double* lon2);
void outputFile(char* path, Vector* path_ids, Graph* graph);

#endif // GRAPH_H