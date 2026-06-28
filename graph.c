#include <math.h>
#include "graph.h"
#include "create_Vector_List/comparators.h"
#include "create_Vector_List/list/generic.h"
#include "create_HashTable/hash_table/generic.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EPS 1e18

void read_csv_nodes(Graph* graph, char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Error: NULL file");          
        return;
    }

    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), file)) {
        printf("Empty file nodes");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char* token;

        token = strtok(buffer, ",");
        if (!token) {
            continue;
        }
        token[strcspn(token, "\r\n")] = '\0';
        
        long long id = atoll(token);

        token = strtok(NULL, ",");
        if (!token) {
            continue;
        }
        token[strcspn(token, "\r\n")] = '\0';
        double lat = atof(token);

        token = strtok(NULL, ",");
        if (!token) {
            continue;
        }
        token[strcspn(token, "\r\n")] = '\0';
        double lon = atof(token);

        addNode(graph, id, lat, lon);
    }
    
    fclose(file);
}

void read_csv_edges(Graph* graph, char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Error: NULL file");          
        return;
    }

    char buffer[1024];
    if (!fgets(buffer, sizeof(buffer), file)) {
        printf("Empty file edges");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char* token;

        token = strtok(buffer, ",");
        if (!token) {
            continue;
        }
        long long from = atoll(token);

        token = strtok(NULL, ",");
        if (!token) {
            continue;
        }
        long long to = atoll(token);

        token = strtok(NULL, ",");
        if (!token) {
            continue;
        }
        double len = atof(token);

        token = strtok(NULL, ",");
        if (!token) {
            continue;
        }

        int oneway = 0;
        if (strcmp(token, "False") == 0) {
            oneway = 1;
        }

        token = strtok(NULL, "\n");
        if (!token) {
            token = "";
        }

        addEdge(graph, from, to, len, oneway, token);
    }
    fclose(file);
}

Graph* createGraph() {
    Graph* g = malloc(sizeof(Graph));
    if (!g) {
        printf("Error: create graph");
        return NULL;
    }

    g->nodes = createHashTable(sizeof(long long), sizeof(GNode));
    if (!g->nodes) {
        printf("Error: create hash table (in createGraph)");
        free(g);
        return NULL;
    }
    return g;
}

void addNode(Graph* g, long long id, double lat, double lon){
    if (!g) {
        printf("Error: NULL graph in addNode");          
        return;
    }
    GNode* node = malloc(sizeof(GNode));
    node->lat = lat;
    node->lon = lon;
    node->node_id = id;
    node->edges = NULL;
    
    setItemHashTable(g->nodes, &id, node, HashLL, longLongEquals);    
}

GNode* getNode(Graph* g, long long id) {
    if (!g) {
        printf("Error: NULL graph in getNode");          
        return NULL;
    }
    return (GNode*)getItemHashTable(g->nodes, &id, HashLL, longLongEquals);
}

void addEdge(Graph* g, long long from, long long to, double len, int oneway, char* name) {
    if (!g) {
        printf("Error: NULL graph in addEdge");          
        return;
    }
    
    GNode* n1 = getNode(g, from);
    if (!n1) {
        printf("Error: create n1");
        return;
    }
    if (from == to) {
        return;
    }

    if (oneway == 1) {
        GEdge* e = malloc(sizeof(GEdge));
        if (!e) {
            printf("Error: create new edge in addEdge");          
            return;
        }
        e->to = to;
        e->from = from;
        e->lenght = len;
        e->next = n1->edges;
        n1->edges = e;
        strncpy(e->name, name, 255);
        e->name[255] = '\0';
    }

    else if (oneway == 0) {
        GEdge* e = malloc(sizeof(GEdge));
        if (!e) {
            printf("Error: create new edge in addEdge");
            return;
        }
        e->to = to;
        e->from = from;
        e->lenght = len;
        e->next = n1->edges;
        n1->edges = e;
        strncpy(e->name, name, 255);
        e->name[255] = '\0';

        GNode* n2 = getNode(g, to);
            if (!n2) {
                printf("Error: create second way");
                return;
            }

        GEdge* e2 = malloc(sizeof(GEdge));
        if (!e2) {
            printf("Error: create new edge2 (in addEdge)");          
            return;
        }
        e2->to = from;
        e2->from = to;
        e2->lenght = len;
        e2->next = n2->edges;
        n2->edges = e2;
        strncpy(e2->name, name, 255);
        e2->name[255] = '\0';
    }
}

void freeGraph(Graph* graph) {
    if (!graph) {
        return;
    }
    for (size_t i =0; i<graph->nodes->capacity; ++i) {
        Vector **bucket_ptr = get_bucket_ptr(graph->nodes, i);
        Vector *bucket = *bucket_ptr;
        if (!bucket) {
            continue;
        }

        for (size_t j = 0; j <bucket->size; ++j) {
            void* elem = getVectorItem(bucket, j);
            GNode* node = (GNode*)((char*)elem + sizeof(long long));

            GEdge* edge = node->edges;
            while (edge) {
                GEdge* tmp = edge;
                edge = edge->next;
                free(tmp);
            }
        }
    }
    freeHashTable(graph->nodes);
    free(graph);
}

long findMin(Vector* dnodes) {
    if (!dnodes) {
        printf("Error: NULL dnodes (in findMin)");          
        return -1;
    }
    double best = EPS;
    long idx = -1;

    for (size_t i = 0; i < dnodes->size; i++) {
        void* elem = getVectorItem(dnodes, i);

        DNode* d = (DNode*)elem;
        if (!d->visited && d->dist < best) {
            best = d->dist;
            idx = (long)i;
        }
    }
    return idx;
}

Vector* buildPath(Vector* dnodes, long long start, long long end) {
    if (!dnodes) {
        printf("Error: NULL dnodes in buildPath");          
        return NULL;
    }
    Vector* path = createVector(sizeof(long long));
    if (!path) {
        return NULL;
    }

    long long cur = end;

    while (cur != -1) {
        appendVectorItem(path, &cur);

        int found = 0;
        for (size_t i = 0; i < dnodes->size; i++) {
            void* elem = getVectorItem(dnodes,i);

            DNode* d = (DNode*)elem;
            if (d->id == cur){
                cur = d->prev;
                found = 1;
                break;
            }
        }
        if (!found) {
            break;
        }
    }
    long long last = *(long long*)getVectorItem(path, path->size - 1);
    if (last != start) {
        vectorFree(path);
        return NULL;
    }   

    return path;
}


Vector* dijkstra(Graph* graph, long long start, long long end) {
    if (!graph) {
        printf("Error: NULL graph (in Dijkstra)");          
        return NULL;
    }
    Vector* dnodes = createVector(sizeof(DNode));
    if (!dnodes) {
        printf("Error: create dnodes (in Dijkstra)");          
        return NULL;
    }

    for (size_t i = 0; i < graph->nodes->capacity; ++i) {
        Vector **bucket_ptr = get_bucket_ptr(graph->nodes, i);
        Vector *bucket = *bucket_ptr;
        if (!bucket) {
            continue;
        }

        for (size_t j = 0; j < bucket->size; ++j) {
            void* elem = getVectorItem(bucket, j);
            GNode* node = (GNode*)((char*)elem + sizeof(long long));

            DNode d;
            d.id = node->node_id;
            d.dist = EPS;
            d.prev = -1;
            d.visited = 0;

            if (node->node_id == start) {
                d.dist = 0;
            }

            appendVectorItem(dnodes, &d);
        }
    }

    while (1) {
        long idx = findMin(dnodes);
        if (idx < 0) {
            break;
        }

        void* elem = getVectorItem(dnodes, idx);
        DNode* cur = (DNode*)elem;
        cur->visited = 1;

        GNode* gnode = getNode(graph, cur->id);
        if (!gnode) {
            continue;
        }
        GEdge* e = gnode->edges;
        while (e) {
            for (size_t i = 0; i < dnodes->size; i++) {
                void* elem = getVectorItem(dnodes, i);
                DNode* neigh = (DNode*)elem;

                // && !neigh->visited
                if (neigh->id == e->to && !neigh->visited) {
                    double nd = cur->dist + e->lenght;
                    if (nd < neigh->dist) {
                        neigh->dist = nd;
                        neigh->prev = cur->id;
                    }
                }
            }
            e = e->next;
        }
    }
    return dnodes;
}
long long* Dijkstra(Graph* graph, long long start, long long end, int* out_len) {
    if (!graph || !out_len) {
        return NULL;
    }
    *out_len = 0;

    Vector* dnodes = dijkstra(graph, start, end);
    if (!dnodes) {
        return NULL;
    }
    int found = 0;
    double end_dist = EPS;

    for (size_t i = 0; i < dnodes->size; i++) {
        void* elem = getVectorItem(dnodes, i);
        DNode* d = (DNode*)elem;

        if (d->id == end) {
            found = 1;
            end_dist = d->dist;
            break;
        }
    }

    if (!found || end_dist == EPS) {
        vectorFree(dnodes);
        printf("boo\n");
        return NULL;
    }

    Vector* path_vec = buildPath(dnodes, start, end);
    if (!path_vec || path_vec->size == 0) {
        vectorFree(dnodes);
        if (path_vec) vectorFree(path_vec);
        return NULL;
    }

    int n = (int)path_vec->size;

    long long* path = malloc(sizeof(long long) * n);
    if (!path) {
        vectorFree(dnodes);
        vectorFree(path_vec);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        path[i] = *(long long*)getVectorItem(path_vec, n - 1 - i);
    }

    *out_len = n;

    vectorFree(dnodes);
    vectorFree(path_vec);

    return path;
}


double dist2(double lat1, double lon1, double lat2, double lon2) {
    double dx = lat1 - lat2;
    double dy = lon1 - lon2;
    return dx*dx + dy*dy;
}

long long findNearest(Graph* graph, double lat, double lon) {
    double best = EPS;
    long long best_id = -1;

    for (size_t i = 0; i < graph->nodes->capacity; ++i) {
        Vector **bucket_ptr = get_bucket_ptr(graph->nodes, i);
        Vector *bucket = *bucket_ptr;
        if (!bucket) {
            continue;
        }
        for (size_t j = 0; j < bucket->size; ++j) {
            void* elem = getVectorItem(bucket, j);
            GNode* node = (GNode*)((char*)elem + sizeof(long long));

            double d = dist2(lat, lon, node->lat, node->lon);
            if (d < best) {
                best = d;
                best_id = node->node_id;
            }
        }
    }

    return best_id;
}   

int readFile(char* path, double* lat1, double* lon1, double* lat2, double* lon2){
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Error: NULL file");
        return -1;
    }

    if (fscanf(file, "%lf %lf", lat1, lon1) != 2) {
        fclose(file);
        return -1;
    }

    if (fscanf(file, "%lf %lf", lat2, lon2) != 2) {
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

void outputFile(char* path, Vector* path_ids, Graph* graph) {    
    FILE* file = fopen(path, "wb");
    if (!file) {
        printf("Error: NULL file");
        return;
    }

    for (long i = (long)path_ids->size - 1; i >= 0; --i) {
        long long id = *(long long*)getVectorItem(path_ids, (size_t)i);
        GNode* node = getNode(graph, id);
        if (!node) {
            continue;
        }
        fprintf(file, "%.6lf %.6lf\n", node->lat, node->lon);
    }

    fclose(file);
}