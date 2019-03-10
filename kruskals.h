//Alex Llamas, Victor Cabunoc
//CS350
//Extra credit question
// Implementation of Kruskal's algorithm for minimum spanning tree of weighted, undirected city-pairs graph data

#ifndef UNTITLED2_KRUSKALS_H
#define UNTITLED2_KRUSKALS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
#include <algorithm>
using namespace std;


// a structure to represent a weighted edge in graph
struct edgeConnection
{
    int cityPairSource;
    int cityPairDest;
    int weight;
};


//main graph structure to form weighted, undirected city-pairs graph
struct cityGraph
{
    int vertices;                             //number of verticies in graph
    int edges;                                //number of edges in graph
    struct edgeConnection * connection;       //edge struct to formulate array of edges within graph
};


// A structure to represent a subset for union-find
struct subset
{
    int parent;
    int rank;
};


//global int parameters for cityGraph
int graphEdges;
int graphVerticies;


//function declarations
void kruskalsAlg(struct cityGraph * graph, string * array);
struct cityGraph * allocateGraph(int vertices, int edges);
void getParameters();
void  populate(struct cityGraph * graph, string * array);
bool edgeExists(struct cityGraph * graph, string city, string dest, string * array);
int map(string city, string * array);
void display (edgeConnection result[], int resultIndex, string * array);



//HELPER FUNCTIONS


//allocate the graph with given parameters and return it
struct cityGraph * allocateGraph(int vertices, int edges)
{
    struct cityGraph * graph = new cityGraph;                   //allocate a new graph
    graph -> vertices = vertices;                               //fill with # verticies needed
    graph -> edges = edges;                                     //fill with #edges needed
    graph -> connection = new edgeConnection[edges];            //allocate array of edge connections

    return graph;
}

//comparator func variable for qsort to compare two edges weights and sort them accordingly
int comparatorFunc(const void * a, const void * b)
{
    struct edgeConnection * edgeA = (struct edgeConnection *)a;
    struct edgeConnection * edgeB = (struct edgeConnection *)b;

//    cout << "\nA val " << edgeA -> weight << endl;
//    cout << "B val " << edgeB -> weight << endl;

    if (edgeA -> weight < edgeB -> weight)
        return -1;

    if (edgeA -> weight == edgeB -> weight)
        return 0;

    if (edgeA -> weight > edgeB -> weight)
        return 1;

}


bool edgeExists(struct cityGraph * graph, string city, string dest, string * array)
{

    for (int i = 0; i < graphEdges ; i++)
    {
        if (graph -> connection[i].cityPairSource == map(dest, array) && graph -> connection[i].cityPairDest == map(city, array)) {
            return true;
        }
    }

    return false;

}


//map anarray index to a city so we can use ints in kruskals algorithm
int map(string city, string array[])
{
    for (int f = 0; f <= graphVerticies + 1; f++) {
        if (array[f] == city)
            return f;
    }

}


//recursive function that traverses throguh until we find the location index fo the given city
int find(struct subset subsets[], int i)
{
    // find root and make root as parent of i
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);       //if not found, recurse again

    return subsets[i].parent;                                       //if foud retrun
}


void display (edgeConnection result[], int resultIndex, string * array)
{
    int totalWeight = 0;                            //int to hold total sum of weight

    cout << "\nThese are the edges of the minimum spanning tree:\n" << endl;

    for (int i = 0; i < resultIndex; ++i)                               //print for final results of min spanning tree
    {
        cout << array[result[i].cityPairSource] << " ---> " << array[result[i].cityPairDest] << "; "<< result[i].weight << " weight" << endl;
        totalWeight = totalWeight + result[i].weight;
    }

    cout << "\nTotal Weight: " << totalWeight << endl;
}


#endif //UNTITLED2_KRUSKALS_H
