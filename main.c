#include <math.h>
#include "graph.h"
#include "create_Vector_List/comparators.h"
#include "create_Vector_List/list/generic.h"
#include "create_HashTable/hash_table/generic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    if (argc != 4) {
        return -1;
    }

    char path_nodes[1024];
    snprintf(path_nodes, sizeof(path_nodes), "%s/nodes.csv", argv[1]);

    char path_edges[1024];
    snprintf(path_edges, sizeof(path_edges), "%s/edges.csv", argv[1]);

    Graph* graph = createGraph();
    double lat1;
    double lon1;
    double lat2;
    double lon2;

    read_csv_nodes(graph, path_nodes);
    read_csv_edges(graph, path_edges);

    if (readFile(argv[2], &lat1, &lon1, &lat2, &lon2) != 0) {
        freeGraph(graph);
        return 1;
    }

    long long start = findNearest(graph, lat1, lon1);
    long long end   = findNearest(graph, lat2, lon2);

    if (start == -1 || end == -1) {
        printf("Error: could not find nearest nodes\n");
        return 1;
    }

    Vector* dnodes = dijkstra(graph, start, end);
    Vector* path = buildPath(dnodes, start, end);

    outputFile(argv[3], path, graph);

    vectorFree(dnodes);
    vectorFree(path);
    freeGraph(graph);
    return 0;
}