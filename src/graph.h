#ifndef GRAPH_H_
#define GRAPH_H_
#include <map>
#include <string>
#include <vector>

class Node {
   public:
    std::string type;
    int id;

   public:
    Node() = default;
    ~Node() {}
};

class Edge {
   public:
    std::string type;
    int src;
    int dest;
    int weight;

   public:
    Edge(std::string type, int src, int dest, int weight = 0) : type(type), src(src), dest(dest), weight(weight) {}
    ~Edge(){};
};

enum class SampleStrategy {
    random,
    topk,
    all
};

class Graph {
   private:
    /* data */
    std::map<int, std::multimap<std::string, Edge>> relations;

   public:
    Graph() = default;
    ~Graph() {}

    class SampleStepResult {
       public:
        std::vector<Edge> edges;
        std::vector<int> dest_list;

       public:
        SampleStepResult() = default;
        ~SampleStepResult() {}
        void add_edge(Edge &e) {
            this->dest_list.push_back(e.dest);
            this->edges.push_back(e);
        }
    };

    bool add_edge(std::string edge_type, int src, int dest, int weight);
    std::vector<SampleStepResult> metapath_bfs_sampling(std::vector<int> &ids, std::vector<std::string> &metapath, std::vector<int> &neigh_nums, SampleStrategy strategy);

   private:
    SampleStepResult sampling_single_step(std::vector<int> &ids, std::string label, int num, SampleStrategy strategy);
};

#endif