#include <iostream>

#include "graph.h"

int main(int argc, char **argv) {
    Graph g;
    //         >>>rel.add_edge(("user","good","buy"), ([1, 1, 2, 2, 2, 3], [11, 12, 11, 13, 14, 15]), edge_weight=None)
    //         >>>rel.add_edge(("good","category","belongTo"), ([11, 12, 13, 14, 15], [21, 21, 22, 22, 23]), edge_weight=None)
    //         >>>path = rel.metapath_bfs_sampling(
    //             ids=[1, 2],
    //             metapath=["buy", "belongTo"],
    //             neigh_nums=[10, 20],
    //             strategy="random",
    //             num_works=16
    //             )
    {
        int srcId[] = {1, 1, 2, 2, 2, 3};
        int destId[] = {11, 12, 11, 13, 14, 15};
        std::string edgeType = "buy";
        int weight = 0;
        for (int i = 0; i < sizeof(srcId) / sizeof(int); ++i) {
            g.add_edge(edgeType, srcId[i], destId[i], weight);
        }
    }
    {
        int srcId[] = {11, 12, 13, 14, 15};
        int destId[] = {21, 21, 22, 22, 23};
        std::string edgeType = "belongTo";
        int weight = 0;
        for (int i = 0; i < sizeof(srcId) / sizeof(int); ++i) {
            g.add_edge(edgeType, srcId[i], destId[i], weight);
        }
    }
    std::vector<int> ids = {1, 2};
    std::vector<std::string> metapath = {"buy", "belongTo"};
    std::vector<int> neigh_num = {1, 1};
    auto strategy = SampleStrategy::random;
    auto res = g.metapath_bfs_sampling(ids, metapath, neigh_num, strategy);

    std::cout << res.size() << std::endl;
    std::cout << res[0].dest_list.size() << "   " << res[0].edges.size() << std::endl;
    for (auto r : res) {
        std::cout << "*************" << std::endl;
        auto edges = r.edges;
        auto dest_list = r.dest_list;
        for (unsigned i = 0; i < edges.size(); ++i) {
            std::cout << edges[i].type << ": " << edges[i].src << " ==> " << edges[i].dest << std::endl;
        }
    }

    return 0;
}
