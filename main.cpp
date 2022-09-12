﻿// Chapter6.cpp : Defines the entry point for the application.

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>

// BFS
#include<queue>

template<typename T> class Graph;

template<typename T>
struct Edge
{
	size_t src;
	size_t dest;
	T weight;

	// To compare edges, only compare their weights,
	// and not the source/destination vertices
	inline bool operator< (const Edge<T>& e) const
	{
		return this->weight < e.weight;
	}

	inline bool operator> (const Edge<T>& e) const
	{
		return this->weight > e.weight;
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Graph<T>& G)
{
	for (size_t i = 1; i <= G.vertices(); i++)
	{
		os << i << ":\t";

		auto edges = G.outgoing_edges(i);
		for (auto& e : edges)
			os << "{" << e.dest << ": " << e.weight << "}, ";

		os << std::endl;
	}

	return os;
}

template<typename T>
class Graph
{
public:
	// Initialize the graph with N vertices
	Graph(size_t num_vertices) : m_num_vertices{ num_vertices }
	{}

	// Return number of vertices in the graph
	auto vertices() const
	{
		return m_num_vertices;
	}

	// Return all edges in the graph
	auto& edges() const
	{
		return m_edge_list;
	}

	void add_edge(Edge<T>&& e)
	{
		// Check if the source and destination vertices are within range
		if (e.src >= 1 && e.src <= m_num_vertices &&
			e.dest >= 1 && e.dest <= m_num_vertices)
			m_edge_list.emplace_back(e);
		else
			std::cerr << "Vertex out of bounds" << std::endl;
	}

	// Returns all outgoing edges from vertex v
	auto outgoing_edges(size_t num_vertices) const
	{
		std::vector<Edge<T>> edges_from_v;
		for (auto& e : m_edge_list)
		{
			if (e.src == num_vertices)
				edges_from_v.emplace_back(e);
		}
		return edges_from_v;
	}

	// Overloads the << operator so a graph be written directly to a stream
	// Can be used as std::cout << obj << std::endl;
	friend std::ostream& operator<< <T>(std::ostream& os, const Graph<T>& G);

private:
	size_t m_num_vertices;		// Stores number of vertices in graph
	std::vector<Edge<T>> m_edge_list;
};

template <typename T>
auto create_reference_graph()
{
	Graph<T> G{ 8 };

	std::map<unsigned, std::vector<std::pair<size_t, T>>> edges;
	edges[1] = { {2, 2}, {5, 3} };
	edges[2] = { {1, 2}, {5, 5}, {4, 1} };
	edges[3] = { {4, 2}, {7, 3} };
	edges[4] = { {2, 1}, {3, 2}, {5, 2}, {6, 4}, {8, 5} };
	edges[5] = { {1, 3}, {2, 5}, {4, 2}, {8, 3} };
	edges[6] = { {4, 4}, {7, 4}, {8, 1} };
	edges[7] = { {3, 3}, {6, 4} };
	edges[8] = { {4, 5}, {5, 3}, {6, 1} };

	for (auto& i : edges)
		for (auto& j : i.second)
			G.add_edge(Edge<T>{ i.first, j.first, j.second });

	return G;
}


template <typename T>
auto breadth_first_search(const Graph<T>& G, size_t start)
{
	std::queue<size_t> queue;
	std::vector<std::pair<size_t, int>> visit_order;
	std::set<size_t> visited;
	queue.push(start); // Assume that BFS always starts from vertex ID 1

	size_t count = 1;
	bool is_border = false;
	int dist = 0;
	while (queue.empty() == false)
	{

		auto current_vertex = queue.front();
		queue.pop();

		if (--count == 0)
		{
			is_border = true;
		}

		// If the current vertex hasn't been visited in the past
		if (visited.find(current_vertex) == visited.end())
		{
			visited.insert(current_vertex);
			visit_order.emplace_back(current_vertex, dist);



			for (const auto& e : G.outgoing_edges(current_vertex))
			{
				queue.push(e.dest);
			}

			if (is_border == true)
			{
				count = queue.size();
				++dist;
				is_border = false;
			}
		}
	}

	return visit_order;
}

template <typename T>
void test_BFS()
{
	// Create an instance of and print the graph
	auto G = create_reference_graph<unsigned>();
	std::cout << G << std::endl;

	// Run BFS starting from vertex ID 1 and print the order
	// in which vertices are visited.
	std::cout << "BFS Order of vertices: " << std::endl;
	auto bfs_visit_order = breadth_first_search(G, 1);
	for (auto v : bfs_visit_order)
		std::cout << v.first << ", " << v.second << std::endl;
}

int main()
{
	using T = unsigned;
	test_BFS<T>();

	return 0;
}
