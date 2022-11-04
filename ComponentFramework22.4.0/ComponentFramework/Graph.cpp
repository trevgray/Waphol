#include "Graph.h"
#include "NodeAndPriority.h"
#include <iostream>

struct ComparePriority {
	bool operator()(NodeAndPriority const& lhs, NodeAndPriority const& rhs) {
		// make it a min queue
		return lhs.priority > rhs.priority;
	}
};

Graph::Graph() {}

Graph::~Graph() {}

bool Graph::OnCreate(std::vector<Node> nodes_) {
	//ASSUMPTION: node_ vector has the node with label "i" in the "i" position of the vector

	//given a list of nodes
	//initialize a matrix of costs
	//with 0.0 weights
	int numNodes = nodes_.size();
	cost.resize(numNodes);

	for (int i = 0; i < numNodes; i++) {
		//add the nodes to internal list
		if (i != nodes_[i].GetLabel()) {
			return false;
		}

		node[i] = nodes_[i];

		//set up the connections
		cost[i].resize(numNodes);
		for (int j = 0; j < numNodes; j++) {
			//initialize connections to other nodes
			cost[i][j] = 0.0f;
		}
	}
	return true;
}

size_t Graph::NumNodes() {
	return node.size();
}

void Graph::AddWeightConnection(int fromNode, int toNode, float weight) {
	cost[fromNode][toNode] = weight;
}

void Graph::AddConnectionVector(MATH::Vec3 fromNodePos, MATH::Vec3 toNodePos) {

	Node fromNode;
	Node toNode;
	for (auto nodeIterator : node) {
		//if (fromNodePos == nodeIterator.second.GetPos()) { - we break it down in the x and y components
		//if (fromNodePos.x == nodeIterator.second.GetPos().x && fromNodePos.y == nodeIterator.second.GetPos().y)
		if (fromNodePos.x == nodeIterator.second.GetPos().x && fromNodePos.y == nodeIterator.second.GetPos().y) {
			fromNode = nodeIterator.second;
		}
		//toNodePos
		if (toNodePos.x == nodeIterator.second.GetPos().x && toNodePos.y == nodeIterator.second.GetPos().y) {
			toNode = nodeIterator.second;
		}
	}
	if (fromNode.GetLabel() == -1 || toNode.GetLabel() == -1) {
		std::cout << "ERROR: Node not found" << std::endl;
	}
	else {
		AddWeightConnection(fromNode.GetLabel(), toNode.GetLabel(), 1.0f);
	}
}

std::vector<int> Graph::Neighbours(int fromNode) {
	std::vector<int> result = {};
	for (int x = 0; x < NumNodes()/*cost[fromNode] == node.size()*/; x++) {
		if (cost[fromNode][x] > 0.0f) {
			result.push_back(x);
		}
	}
	return result;
}

std::vector<int> Graph::AStar(int startNode, int goalNode) {
	//declare helper variables
	float newCost;
	int current;

	//declare current NodeAndPriority
	//set up priority queue for frontier of nodes
	std::priority_queue<NodeAndPriority, std::deque<NodeAndPriority>, ComparePriority> frontier;
	frontier.push(NodeAndPriority(&node[startNode], 0.0f));
	//track solution path
	std::vector<int> cameFrom;
	cameFrom.resize(NumNodes());
	//store cost so far to reach a node
	std::map<int, float> costSoFar;
	costSoFar[startNode] = 0.0f;

	//start looping through the frontier
	while (frontier.size() > 0) {
		//get the node from the top of the frontier, put it in "current"
		current = frontier.top().node->GetLabel();
		//pop it off
		frontier.pop();
		//if its the goal, then break of the loop
		if (current == goalNode) {
			break;
		}
		//for the neighbors of current
		for (int nextNode : Neighbours(current)) {
			//calculate newCost
			newCost = costSoFar[current] + cost[current][nextNode];
			//if neighbor is not in costSoFar or newCost is lower
			if (costSoFar[nextNode] == 0.0f || newCost < costSoFar[nextNode]) {
				//found a better path so update structure (look at pseudo code algorithm)
				costSoFar[nextNode] = newCost + Heuristic(nextNode, goalNode); //+ Heuristic
				frontier.push(NodeAndPriority(&node[nextNode], newCost));
				cameFrom[nextNode] = current;
			}
		}
	}
	return cameFrom;
}

float Graph::Heuristic(int startNode, int goalNode) {
	float dx = abs(node[startNode].GetPos().x - node[goalNode].GetPos().x);
	float dy = abs(node[startNode].GetPos().y - node[goalNode].GetPos().y);

	float D = 1;

	//return D * sqrt(dx * dx + dy * dy); //Euclidean distance - http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
	return D * (dx * dx + dy * dy); //avoid the expensive square root in the Euclidean distance by using distance-squared
}

std::vector<int> Graph::Dijkstra(int startNode, int goalNode) {
	//declare helper variables
	float newCost;
	int current;

	//declare current NodeAndPriority
	NodeAndPriority* currentNodeAndPriority; //missing node and priority
	currentNodeAndPriority = new NodeAndPriority(&node[startNode], 0.0f);
	//set up priority queue for frontier of nodes
	std::priority_queue<NodeAndPriority, std::deque<NodeAndPriority> , ComparePriority> frontier;
	frontier.push(*currentNodeAndPriority);
	//track solution path
	std::vector<int> cameFrom;
	cameFrom.resize(NumNodes());
	//store cost so far to reach a node
	std::map<int, float> costSoFar;
	costSoFar[startNode] = 0.0f;

	//start looping through the frontier
	while (frontier.size() > 0) {
		//get the node from the top of the frontier, put it in "current"
		current = frontier.top().node->GetLabel();
		//pop it off
		frontier.pop();
		//if its the goal, then break of the loop
		if (current == goalNode) {
			break;
		}
		//for the neighbors of current
		for (int nextNode : Neighbours(current)) {
			//calculate newCost
			newCost = costSoFar[current] + cost[current][nextNode];
			//if neighbor is not in costSoFar or newCost is lower
			if (costSoFar[nextNode] == 0.0f || newCost < costSoFar[nextNode]) {
				//found a better path so update structure (look at pseudo code algorithm)
				costSoFar[nextNode] = newCost;
				frontier.push(NodeAndPriority(&node[nextNode], newCost));
				cameFrom[nextNode] = current;
			}
		}
	}
	return cameFrom;
}
