#include "NodeAndPriority.h"
#include <stdio.h>

NodeAndPriority::NodeAndPriority(Node* node_, float priority_)
{
    node = node_;
    priority = priority_;
}

void NodeAndPriority::print()
{
    printf("Node: %i, priority: %.2f \n", node->GetLabel(), priority);
}