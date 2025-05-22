#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#include<limits.h>
#include<float.h>

// Edge structure for Graph(adjacency list)
typedef struct Edge {
    int dest;
    float weight;
    struct Edge* next;
} Edge;

// Graph structure(adjacency list)
typedef struct {
    int no_of_verts;
    int *vert_labels;
    int *ver_indices;
    int max_vert_label;
    Edge **adj_list;
} Graph;

// MinHeapNode structure for Dijkstra's algorithm
typedef struct {
    int vertex;
    // int distance;
    float distance;
} MinHeapNode;


typedef struct {
    int capacity;
    int size;
    int *pos;
    MinHeapNode **array;
} MinHeap;

// Neew edge creation
Edge* create_edge(int dest, float weight) {
    Edge *new_edge = (Edge*)malloc(sizeof(Edge));
    new_edge->dest = dest;
    new_edge->weight = weight;
    new_edge->next = NULL;
    return new_edge;
}

// New graph based on no of vertices
Graph* create_graph(int no_of_verts) {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->no_of_verts = 0;
    graph->max_vert_label = no_of_verts;

    graph->vert_labels = (int*)malloc(no_of_verts * sizeof(int));
    graph->ver_indices = (int*)malloc(no_of_verts * sizeof(int));


    graph->adj_list = (Edge**)malloc(no_of_verts * sizeof(Edge*));
    for (int i = 0; i < no_of_verts; i++) {
        graph->adj_list[i] = NULL;
        graph->vert_labels[i] = -1;
        graph->ver_indices[i] = -1;
    }
    return graph;
}

// Add vertex to graph
int add_vert(Graph *graph, int vert_label) {

    if (graph->ver_indices[vert_label] != -1) {
        return graph->ver_indices[vert_label];
    }

    if (graph->no_of_verts >= graph->max_vert_label) {
        return -1; 
    }

    int index = graph->no_of_verts;
    graph->vert_labels[index] = vert_label;
    graph->ver_indices[vert_label] = index;
    graph->no_of_verts++;
    return index;
}

// From teh file adds the edge with vertices and weight
void add_edge(Graph* graph, int src, int dest, float weight) {
    int src_index = add_vert(graph, src);
    int dest_index = add_vert(graph, dest);

    if (src_index == -1 || dest_index == -1) {
        return; 
    }

    Edge* new_edge = create_edge(dest_index, weight);
    new_edge->next = graph->adj_list[src_index];
    graph->adj_list[src_index] = new_edge;
}


MinHeapNode* create_min_heap_node(int vertex, float distance) {
    MinHeapNode *min_heap_node = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    min_heap_node->vertex = vertex;

    min_heap_node->distance = distance;
    return min_heap_node;
}

MinHeap* create_min_heap(int capacity) {
    MinHeap *min_heap = (MinHeap*)malloc(sizeof(MinHeap));
    min_heap->pos = (int*)malloc(capacity * sizeof(int));
    // Initialize pos array
    for (int i = 0; i < capacity; i++) {
        min_heap->pos[i] = -1;  
    }
    min_heap->size = 0;

    min_heap->capacity = capacity;
    min_heap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    return min_heap;
}

void swap_min_heap_node(MinHeapNode **a, MinHeapNode **b) {
    MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void min_heapify(MinHeap *min_heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < min_heap->size && min_heap->array[left]->distance < min_heap->array[smallest]->distance) {
        smallest = left;
    }
    if (right < min_heap->size && min_heap->array[right]->distance < min_heap->array[smallest]->distance) {
        smallest = right;
    }
    if (smallest != index) {
        MinHeapNode *smallest_node = min_heap->array[smallest];
        MinHeapNode *index_node = min_heap->array[index];

        min_heap->pos[smallest_node->vertex] = index;
        min_heap->pos[index_node->vertex] = smallest;

        swap_min_heap_node(&min_heap->array[smallest], &min_heap->array[index]);
        min_heapify(min_heap, smallest);
    }
}

int is_heap_empty(MinHeap *min_heap) {
    return min_heap->size == 0;
}

// EXTRACTMIN for DIjKSTRA
MinHeapNode* extract_min(MinHeap *min_heap) {
    if (is_heap_empty(min_heap)) {
        return NULL;
    }

    MinHeapNode *root = min_heap->array[0];

    MinHeapNode *last_node = min_heap->array[min_heap->size - 1];
    min_heap->array[0] = last_node;

    min_heap->pos[root->vertex] = min_heap->size - 1;
    min_heap->pos[last_node->vertex] = 0;

    --min_heap->size;
    min_heapify(min_heap, 0);

    return root;
}

void decrease_key(MinHeap *min_heap, int vertex, float distance) {
    int i = min_heap->pos[vertex];

    min_heap->array[i]->distance = distance;

    while (i && min_heap->array[i]->distance < min_heap->array[(i - 1) / 2]->distance) {
        min_heap->pos[min_heap->array[i]->vertex] = (i - 1) / 2;
        min_heap->pos[min_heap->array[(i - 1) / 2]->vertex] = i;
        swap_min_heap_node(&min_heap->array[i], &min_heap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

int is_in_min_heap(MinHeap *min_heap, int vertex) {
    return min_heap->pos[vertex] < min_heap->size;
}

void insert_min_heap(MinHeap *min_heap, int vertex, float distance) {
    if (min_heap->size == min_heap->capacity) {
        return;
    }

    int i = min_heap->size;
    min_heap->pos[vertex] = i;
    min_heap->array[i] = create_min_heap_node(vertex, distance);
    min_heap->size++;

    while (i && min_heap->array[i]->distance < min_heap->array[(i - 1) / 2]->distance) {
        min_heap->pos[min_heap->array[i]->vertex] = (i - 1) / 2;
        min_heap->pos[min_heap->array[(i - 1) / 2]->vertex] = i;
        swap_min_heap_node(&min_heap->array[i], &min_heap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

// Function to print the result i.e the shortest path and the sum of weughts
// from start to end
void print_result_path(int *parent, float *distance, Graph *graph,int start, int end) {
    int path[10000];
    int length_path = 0;

    int curr = end;
    while (curr != -1) {
        path[length_path++] = graph->vert_labels[curr];
        curr = parent[curr];
    }

    int int_dist = (int)distance[end];
        
    if (int_dist < 0) {
        printf("No path found\n"); 
    }
    else if (distance[end] >= INT_MAX-1) {
        printf("No path found\n");
    } else {
        printf("Shortest path from %d to %d: ", graph->vert_labels[start], graph->vert_labels[end]);
        for (int i = length_path - 1; i > 0; i--) {
            printf("%d -> ", path[i]);
        }
        printf("%d\n", path[0]);

        printf("Distance: %.2f\n", distance[end]);
    }

}

void dijkstra(Graph *graph, int start, int end) {
    // Arrays for distances and pareent pointers
    float *distance = (float*)malloc(graph->no_of_verts * sizeof(float));
    int *parent = (int*)malloc(graph->no_of_verts * sizeof(int));
    MinHeap *min_heap = create_min_heap(graph->no_of_verts);

    // intially all dists intialized to inf and parents to -1
    for (int i = 0; i < graph->no_of_verts; i++) {
        distance[i] = FLT_MAX;
        parent[i] = -1;
        insert_min_heap(min_heap, i, distance[i]);
    }

    // for default , distance of start is 0
    distance[start] = 0.0f;
    decrease_key(min_heap, start, distance[start]);

    // Process continues until heap is empty
    while (!is_heap_empty(min_heap)) {
        // Taking the vertex with min distance
        MinHeapNode *min_node = extract_min(min_heap);
        int u = min_node->vertex;

        // all adjacent vertices are checked
        Edge *edge = graph->adj_list[u];
        while (edge != NULL) {
            int v = edge->dest;
            if (is_in_min_heap(min_heap, v) && distance[u] != INT_MAX && edge->weight + distance[u] < distance[v]) {
                distance[v] = distance[u] + edge->weight;
                parent[v] = u;
                decrease_key(min_heap, v, distance[v]);
            }
            edge = edge->next;
        }
    }

    // result printed
    print_result_path(parent, distance, graph, start, end);

    free(distance);
    free(parent);
    free(min_heap->pos);
    free(min_heap->array);
    free(min_heap);
}

int max_vert_label(Graph *graph) {
    int max = -1;
    
    for (int i = 0; i < graph->max_vert_label; i++) {
    
        if (graph->vert_labels[i] != -1 && graph->vert_labels[i] > max) {
            max = graph->vert_labels[i];
        }
    }
    return max;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void free_graph(Graph *graph) {
    Edge *temp;

    for (int i = 0; i < graph->no_of_verts; i++) {
        while (graph->adj_list[i] != NULL) {
            temp = graph->adj_list[i];
            graph->adj_list[i] = graph->adj_list[i]->next;
            free(temp);
        }
    }
    free(graph->adj_list);
    free(graph->vert_labels);
    free(graph->ver_indices);
    free(graph);
}

int main() {
    // Gettin the file name from user
    char filename[256];
    printf("Enter input file name: ");
    int ret = scanf("%s", filename);
    if (ret != 1) {
        printf("Error reading filename.\n");
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    // printf("Successfully opened file: %s\n", filename);

    // THis to find the max vertex label
    int src, dest;
    float weight;
    int max_vert_label = 0;
    while (fscanf(file, "%d,%d,%f", &src, &dest, &weight) == 3) {
        max_vert_label = max(max_vert_label, src);
        max_vert_label = max(max_vert_label, dest);
    }

    rewind(file); 

    // Create the graph with max vertex label
    Graph *graph = create_graph(max_vert_label + 1); 

    while (fscanf(file, "%d,%d,%f", &src, &dest, &weight) == 3) {
        add_edge(graph, src, dest, weight);
    }


    fclose(file);

    // printf("Vertices loaded into graph (%d):\n", graph->no_of_verts);
    // for (int i = 0; i < graph->no_of_verts; i++) {
    //     printf("%d ", graph->vert_labels[i]);
    // }
    // printf("\n");

    // Getting the start and end vertices from user
    int start_label, end_label;
    printf("Enter start and end vertices: ");
    if (scanf("%d %d", &start_label, &end_label) != 2) {
        printf("Invalid input.\n");
        return 1;
    }

    // Checks if the start and end vertices are in the graph
    if (start_label < 0 || start_label >= graph->max_vert_label || graph->ver_indices[start_label] == -1 ||
        end_label < 0 || end_label >= graph->max_vert_label || graph->ver_indices[end_label] == -1) {
        printf("Error: Start or End vertex not found in graph.\n");
        return 1;
    }

    int start = graph->ver_indices[start_label];
    int end = graph->ver_indices[end_label];

    dijkstra(graph, start, end);

    // Freeing memory allocated to the graph
    for (int i = 0; i < graph->no_of_verts; i++) {
        Edge *temp;
        while (graph->adj_list[i] != NULL) {
            temp = graph->adj_list[i];
            graph->adj_list[i] = graph->adj_list[i]->next;
            free(temp);
        }
    }
    free(graph->adj_list);
    free(graph->vert_labels);
    free(graph->ver_indices);
    free(graph);

    return 0;
}