#include <gtest/gtest.h>
#include "graph.hpp"

#include <limits>

class GraphTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Complex graph with multiple paths
        g.add_edge("A", "B", 4);
        g.add_edge("A", "C", 2);
        g.add_edge("B", "C", 5);
        g.add_edge("B", "D", 10);
        g.add_edge("C", "D", 3);
        g.add_edge("D", "E", 7);
        g.add_edge("E", "F", 2);
        g.add_edge("F", "A", 9);
    }

    template_library::Graph g;
};

TEST_F(GraphTest, BasicPathFinding) {
    EXPECT_EQ(g.find_shortest_path("A", "D"),
              (std::vector<std::string>{"A", "C", "D"})); // Cost 2+3=5
}

TEST_F(GraphTest, NoPathExists) {
    template_library::Graph local_g;
    local_g.add_edge("X", "Y", 10);
    EXPECT_TRUE(local_g.find_shortest_path("X", "Z").empty());
}

TEST_F(GraphTest, DisconnectedGraph) {
    g.add_edge("Z", "W", 3);
    EXPECT_TRUE(g.find_shortest_path("A", "Z").empty());
}

TEST_F(GraphTest, CyclicGraph) {
    auto path = g.find_shortest_path("A", "F");
    EXPECT_EQ(path, (std::vector<std::string>{"A", "C", "D", "E", "F"})); // 2+3+7+2=14
}

TEST_F(GraphTest, EqualPathWeights) {
    g.add_edge("A", "D", 5);
    EXPECT_EQ(g.find_shortest_path("A", "D"),
              (std::vector<std::string>{"A", "C", "D"})); // Still cheaper (5 vs 2+3)
}

TEST_F(GraphTest, NodeToItself) {
    EXPECT_EQ(g.find_shortest_path("A", "A"),
              (std::vector<std::string>{"A"}));
}

TEST_F(GraphTest, LargeValueStress) {
    const int max_int = std::numeric_limits<int>::max();
    g.add_edge("A", "X", max_int - 100);
    g.add_edge("X", "Y", 50);
    auto path = g.find_shortest_path("A", "Y");
    EXPECT_EQ(path, (std::vector<std::string>{"A", "X", "Y"}));
}

TEST_F(GraphTest, InvalidNodes) {
    EXPECT_THROW(g.find_shortest_path("Ghost", "City"), std::out_of_range);
    EXPECT_THROW(g.find_shortest_path("A", "Phantom"), std::out_of_range);
}

// Parameterized test for multiple scenarios
struct PathTestCase {
    std::string start;
    std::string end;
    std::vector<std::string> expected;
};

class GraphParamTest : public GraphTest,
                       public ::testing::WithParamInterface<PathTestCase> {};

TEST_P(GraphParamTest, MultiplePaths) {
    auto params = GetParam();
    EXPECT_EQ(g.find_shortest_path(params.start, params.end), params.expected);
}

INSTANTIATE_TEST_SUITE_P(
        Default,
        GraphParamTest,
        ::testing::Values(
                PathTestCase{"A", "B", {"A", "B"}},       // Direct 4 vs A-C-B (2+5=7)
                PathTestCase{"B", "E", {"B", "D", "E"}},  // 10+7=17 vs B-C-D-E (5+3+7=15)
                PathTestCase{"C", "F", {"C", "D", "E", "F"}}
        )
);


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
