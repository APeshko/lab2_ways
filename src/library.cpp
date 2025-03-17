#include "graph.hpp"
#include <queue>
#include <unordered_map>
#include <limits>
#include <stdexcept>

namespace template_library {

    void Graph::add_edge(const std::string &from, const std::string &to, uint16_t weight) {
        adj[from].push_back(Edge{to, weight});
        adj[to].push_back(Edge{from, weight}); // Делаем граф неориентированным
    }

    std::vector<std::string> Graph::find_shortest_path(const std::string &start, const std::string &end) {
        if (adj.find(start) == adj.end() || adj.find(end) == adj.end()) {
            throw std::out_of_range("One or both nodes are missing in the graph");
        }

        using Pair = std::pair<int, std::string>;
        std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq;
        std::unordered_map<std::string, int> dist;
        std::unordered_map<std::string, std::string> prev;

        for (const auto &node : adj) {
            dist[node.first] = std::numeric_limits<int>::max();
        }

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [current_dist, current] = pq.top();
            pq.pop();

            if (current == end) break;

            if (current_dist > dist[current]) continue;

            for (const auto &edge : adj[current]) {
                int new_dist = dist[current] + edge.weight;
                if (new_dist < dist[edge.to]) {
                    dist[edge.to] = new_dist;
                    prev[edge.to] = current;
                    pq.push({new_dist, edge.to});
                }
            }
        }

        if (dist[end] == std::numeric_limits<int>::max()) {
            return {}; // Нет пути
        }

        std::vector<std::string> path;
        for (std::string at = end; at != start; at = prev[at]) {
            path.push_back(at);
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());

        return path;
    }
}
