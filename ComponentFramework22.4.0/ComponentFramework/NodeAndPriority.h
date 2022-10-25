#pragma once
#include "Node.h"

class NodeAndPriority {
public:

    // member variables
    Node* node;
    float priority;

    // constructors
    NodeAndPriority(Node* node_, float priority);

    // useful functions
    void print();
};