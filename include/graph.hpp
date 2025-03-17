#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <cstdint>

namespace template_library {
    /// Sample Graph structure for implementing
    class Graph {
        struct Edge { std::string to; uint16_t weight; };
        std::unordered_map<std::string, std::vector<Edge>> adj;

    public:
        virtual void add_edge(const std::string& from, const std::string& to, uint16_t weight);
        virtual std::vector<std::string> find_shortest_path(const std::string& start, const std::string& end);
    };
}
