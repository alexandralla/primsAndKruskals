//Alex Llamas, Victor Cabunoc
//CS350
//Extra credit question
// Implementation of Kruskal's algorithm for minimum spanning tree of weighted, undirected city-pairs graph data

#include "kruskals.h"
#include <algorithm>

//main sets graph parameters, creates cityGraph, fills city graph, sorts the data by weight, then calls krukals algorithem
int main() {
    bool success = false;                    //bool value to know when to stop reading
    ifstream fin;                           //file variable
    string city, dest;
    int weight;
    int index = 0;
    bool addIn = true;

    getParameters();
    struct cityGraph * graph = allocateGraph(graphVerticies, graphEdges);
    string array[graphVerticies];

    //open file and create a temp array of citys so that we can map the city name to an index number
    fin.open("city-pairs.txt");                 //open up file

    if (fin)                                    //only executed if file opened successfully
    {
        while (!success)                        //while there is still more to read, keep reading
        {
            if (!fin.eof())                     //if read was successful, put data in
            {

                fin >> city >> dest >> weight;

                for (int i = 0; i < graphVerticies + 1; i++)
                {
                    if (array[i] == city)
                    {
                        addIn = false;
                    }
                }

                if (addIn == true)
                {
                    array[index] = city;
                    index = index + 1;
                }

                addIn = true;

            }
            else					               //if read was not successful, exit loop
                success = true;
        }
    }
    else
        cout << "DID NOT OPEN FILE" << endl;

    fin.close();                                    //close file

    populate(graph, array);                         //populate graph with int values once we have paramteters and city mappings

//    for (int i = 0; i < graphEdges ; i++)         //to see whats inside cityGraph
//    {
//        cout << "Number: " << i + 1 << endl;
//        cout << graph -> connection[i].cityPairSource << endl;
//        cout << graph -> connection[i].cityPairDest << endl;
//        cout << graph -> connection[i].weight << endl;
//        cout << "________" << endl;
//    }

//sort data by weight to prepare kruscals alg???
//quick sort clibrary format - void qsort (void* base, size_t num, size_t size, int (*comparator)(const void*,const void*));
    qsort(graph -> connection, graph -> edges, sizeof(graph -> connection[0]), comparatorFunc);

    for (int h = 0; h < graphEdges; h++)          //to see sorted edges based on weight
    {
        cout << "sorted " << graph -> connection[h].weight << endl;
    }

    //call kruscals alg
    kruskalsAlg(graph, array);

    return 0;
}


//gets parameters of cityGraph (#verticies and edges) by reading through data file
void getParameters()
{
    bool success = false;					//bool value to know when to stop reading
    ifstream fin;                           //file variable
    int vertices = 0;
    int edges = 0;
    string previousCity = "";
    string city, dest;
    int weight;

    fin.open("city-pairs.txt");					//open up file

    if (fin) 						            //only executed if file opened successfully
    {
        while (!success) 			        	//while there is still more to read, keep reading
        {
            if (!fin.eof())				        //if read was successful, put data in
            {
                fin >> city >> dest >> weight;

                if (edges == 0)
                {
                    previousCity = city;
                }

                if (city != previousCity)       //if we are at a new citySource
                {
                    vertices = vertices + 1;
                    previousCity = city;
                }

                edges = edges + 1;
            }
            else					//if read was not successful, exit loop
                success = true;
        }

        fin.close();                //close file

        //set the size of the graph to globals
        graphEdges = (edges / 2) ;
        graphVerticies = vertices;
    } else {cout << "DID NOT OPEN FILE" << endl;}
}



//populate cityGraph with the city data
void  populate(struct cityGraph * graph, string * array) {
    bool success = false;                    //bool value to know when to stop reading
    ifstream fin;                           //file variable
    int i = 0;
    string city, dest;
    int weight;

    fin.open("city-pairs.txt");                    //open up file

    if (fin)                                       //only executed if file opened successfully
    {
        while (!success)                           //while there is still more to read, keep reading
        {
            if (!fin.eof())                        //if read was successful, put data in
            {
                fin >> city >> dest >> weight;

                if (!edgeExists(graph, city, dest, array) && city != "Albany")  //if edge does not already exist, add it in (undirected)
                {
                    graph -> connection[i].cityPairSource = map(city, array);
                    graph -> connection[i].cityPairDest = map(dest, array);
                    graph -> connection[i].weight = weight;

                    i = i + 1;
                }
            }
            else                    //if read was not successful, exit loop
                success = true;
        }
        fin.close();                //close file
    }
    else
        cout << "DID NOT OPEN FILE" << endl;
}



//main function to construct MST using Kruskal's algorithm
void kruskalsAlg(struct cityGraph * graph, string * array) {
    struct edgeConnection result[graphVerticies];   //store the final result in an array of edge structs so that it can be read later
    int sortedIndex = 0;                            //index variable for sorted edges
    int resultIndex = 0;                            //index variable to parse throguh result[]

    struct subset *vSubset;                        //allocate memory for creating the subsets for each verticy
    vSubset = new struct subset[graphVerticies];

    for (int i = 0; i < graphVerticies; ++i)        //need to initialize subsets in numerical order
    {
        vSubset[i].parent = i;
        vSubset[i].rank = 0;
    }

    while (resultIndex < graphVerticies - 1)        //loop through all verticies from 0 to graphVerticies
    {
        struct edgeConnection current = graph -> connection[sortedIndex];       //start at smallest edge with index i and incrememnt through sorted list
        int src = find(vSubset, current.cityPairSource);            //get int value of verticy in order to do a compare if in same tree
        int dest = find(vSubset, current.cityPairDest);

        if (src != dest)                                            //if adding x and y are not already part of the same tree, add into result array and increment index (if not dont do anythign)
        {
            result[resultIndex++] = current;                        //add current edge to result edge list

            //UNION FIND!!!! Attach smaller rank under root of higher rank tree
            //3 cases to look for
            int srcRoot = find(vSubset, src);
            int destRoot = find(vSubset, dest);

            //1. if src rank is smaller, make dest root its parent
            if (vSubset[srcRoot].rank < vSubset[destRoot].rank)
                vSubset[srcRoot].parent = destRoot;

            //2. if dest rank is smaller, make src root its parent
            else if (vSubset[srcRoot].rank > vSubset[destRoot].rank)
                vSubset[destRoot].parent = srcRoot;

            //3. if they are equal, one is assigned root and the other gets incremented by one
            else
            {
                vSubset[destRoot].parent = srcRoot;
                vSubset[srcRoot].rank++;
            }
        }

        sortedIndex += 1;     //incrementt index
    }

    display(result, resultIndex, array);
}
