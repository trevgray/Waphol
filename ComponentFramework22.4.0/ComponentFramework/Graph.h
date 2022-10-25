#pragma once

#include <vector>
#include <map>
#include <queue>
#include "Node.h"

class Graph {
private:
	//a list of the nodes
	//i could use STL vector, but map matches some textbooks
	std::map<int, Node> node; //node[i]

	//a 2D matrix of the connection costs
	std::vector<std::vector<float>> cost;

public:
	Graph();
	~Graph();
	bool OnCreate(std::vector<Node> nodes);
	size_t NumNodes();
	Node GetNode(int label) { return node[label]; }
	void AddWeightConnection(int fromNode, int toNode, float weight);
	void AddConnectionVector(MATH::Vec3 fromNodePos, MATH::Vec3 toNodePos);
	std::vector<int> Neighbours(int fromNode);
	std::vector<int> AStar(int startNode, int goalNode);
	float Heuristic(int startNode, int goalNode);
	std::vector<int> Dijkstra(int startNode, int goalNode);

	std::map<int, Node> GetNodes() { return node; }
};