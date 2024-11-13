#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef enum { SOURCE, SINK, DICE, WORD } Node_Type;

class Edge {
    public:
        class Node *to;
        class Node *from;
        Edge *reverse;
        int original = 0;
        int residual = 1;
        bool remove = 0;
};

class Node {
    public:
        Node* parent;
        Node_Type type;
        int number;  // dice number or character number for words
        string word;
        bool visited;
        vector <Edge*> adj;
        Edge *backedge;
        bool match; // if node has matched a letter
};

class Graph {
    public:
        Node *source;
        Node *sink;
        vector<Node*> nodes;
        vector<Edge*> backedges;

        int BFS();
        int CanISpell();
        void DeleteHalfGraph();
};

int main(int argc, char** argv) {
    ifstream dice_file(argv[1]);
    ifstream word_file(argv[2]);

    string cur_word;
    int itemInGraph = 0; // to help resize nodes vector for DeleteHalfGraph()

    // make graph
    Graph* graph = new Graph;

    // populate source and sink
    graph->source = new Node;
    graph->source->type = SOURCE;
    graph->source->word = "source";
    graph->source->backedge = NULL;
    graph->nodes.push_back(graph->source);
    itemInGraph++;

    graph->sink = new Node;
    graph->sink->type = SINK;
    graph->sink->word = "sink";
    graph->sink->backedge = NULL;
    graph->nodes.push_back(graph->sink);
    itemInGraph++;

    while (getline(dice_file, cur_word)) {
        // populate node info
        Node* n = new Node;
        n->parent = graph->source;
        n->backedge = NULL;
        n->type = DICE;
        n->number = itemInGraph;
        n->word = cur_word; // this is the letters of the die
        n->visited = false;
        
        itemInGraph++;

        // make edges and reverse
        Edge* edge = new Edge; // edge from source to die
        Edge* edgeR = new Edge; // reverse: edge from die to source

        edge->to = n;
        edgeR->to = graph->source;
        edge->from = graph->source;
        edgeR->from = n;
        edge->reverse = edgeR;
        edgeR->reverse = edge;
        edgeR->original = 0; edgeR->residual = 1;

        // connect elements
        graph->source->adj.push_back(edge);
        graph->nodes.push_back(n);
        n->adj.push_back(edgeR);
    }

    while (getline(word_file, cur_word)) {

        for (int i = 0; i < cur_word.size(); i++) {
            Node* n = new Node;
            n->type = WORD;
            n->backedge = NULL;
            n->number = i;
            n->word = cur_word[i];

            //Edge to sink from letter
            Edge *edge = new Edge;
            Edge *edgeR = new Edge;
            edge->to = graph->sink;
            edgeR->to = n;
            edge->from = n;
            edgeR->from = graph->sink;
            edge->remove = 1;
            edgeR->remove = 1;
            edge->residual = 1;
            edgeR->original = 0;
            edge->reverse = edgeR;
            edgeR->reverse = edge;

            n->adj.push_back(edge);
            graph->sink->adj.push_back(edgeR);
            graph->nodes.push_back(n);

            for (int i = 0; i < graph->source->adj.size(); i++) {
                string temp = graph->source->adj[i]->to->word;
                string temp2 = n->word;

                for (int j = 0; j < temp.size(); j++){
                    if (temp[j] == temp2[0]){
                        // add edge to and from dice
                        Edge *edge2 = new Edge;
                        Edge *edge2R = new Edge;

                        edge2->to = n;
                        edge2R->to = graph->source->adj[i]->to;
                        edge2->from = graph->source->adj[i]->to;
                        edge2R->from = n;
                        edge2R->residual = 1;
                        edge2R->original = 0;
                        edge2->remove = 1;
                        edge2R->remove = 1;
                        edge2->reverse = edge2R;
                        edge2R->reverse = edge2;

                        graph->source->adj[i]->to->adj.push_back(edge2);
                        n->adj.push_back(edge2R);
                    }
                }
            }
        }

        // ********* TO_DO ************//
        // do pathfinding with CanISpell(), which calls BFS over and over again until no path is found.
        // Crumpton video number three is very useful
        
        graph->DeleteHalfGraph();

    }

    return 0;
}

void Graph::DeleteHalfGraph() {

}
