#include "graph.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>
#include <utility>

using std::string;
using std::vector;

bool Graph::add_edge(string edge_type, int src, int dest, int weight) {
    auto edge = new Edge(edge_type, src, dest, weight);
    if (this->relations.find(src) == this->relations.end()) {
        this->relations[src] = {};
    }
    this->relations[src].insert({edge_type, Edge(edge_type, src, dest, weight)});
    return true;
}

Graph::SampleStepResult Graph::sampling_single_step(std::vector<int> &ids, std::string label, int num, SampleStrategy strategy) {
    Graph::SampleStepResult res;

    for (auto id : ids) {
        auto it = this->relations.find(id);
        if (it == this->relations.end()) {
            continue;
        }

        auto edgeMap = it->second;
        auto pos = edgeMap.equal_range(label);
        // check if all the elements is less than num
        bool is_less = false;
        if (std::distance(pos.first, pos.second) <= num) {
            is_less = true;
        }
        switch (strategy) {
            case SampleStrategy::all: {
                for (auto num_need = num; pos.first != pos.second && num_need != 0; ++pos.first, --num_need) {
                    auto &edge = pos.first->second;
                    res.add_edge(edge);
                }
                break;
            }
            case SampleStrategy::random: {
                vector<Edge> all_edges;
                for (auto pos = edgeMap.equal_range(label); pos.first != pos.second; ++pos.first) {
                    auto &edge = pos.first->second;
                    if (is_less) {
                        res.add_edge(edge);
                    } else {
                        all_edges.push_back(edge);
                    }
                }
                if (!is_less) {
                    size_t left = std::distance(all_edges.begin(), all_edges.end());
                    auto begin = all_edges.begin();
                    int num_random = num;
                    while (num_random--) {
                        auto r = begin;
                        std::advance(r, rand() % left);
                        std::swap(*begin, *r);
                        ++begin;
                        --left;
                    }
                    for (unsigned i = 0; i < num; ++i) {
                        res.add_edge(all_edges[i]);
                    }
                }
                break;
            }
            case SampleStrategy::topk: {
                vector<Edge> all_edges;
                for (; pos.first != pos.second; ++pos.first) {
                    auto &edge = pos.first->second;
                    if (is_less) {
                        res.add_edge(edge);
                    } else {
                        all_edges.push_back(edge);
                    }
                }
                if (!is_less) {
                    std::partial_sort(all_edges.begin(), all_edges.begin() + num, all_edges.end(), [](const Edge &lhs, const Edge &rhs) {
                        return lhs.weight > rhs.weight;
                    });
                    for (unsigned i = 0; i < num; ++i) {
                        res.add_edge(all_edges[i]);
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    return res;
}

vector<Graph::SampleStepResult> Graph::metapath_bfs_sampling(std::vector<int> &ids, std::vector<string> &metapath, std::vector<int> &neigh_nums, SampleStrategy strategy) {
    if (metapath.size() != neigh_nums.size()) {
        throw std::invalid_argument("the size of metapath and neigh_num are not same");
    }
    vector<SampleStepResult> results;

    for (unsigned i = 0; i < metapath.size(); ++i) {
        string label = metapath[i];
        int num = neigh_nums[i];
        auto res = this->sampling_single_step(ids, label, num, strategy);
        results.push_back(res);
        ids = res.dest_list;
    }
    return results;
}