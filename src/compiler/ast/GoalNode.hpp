#pragma once

#include "Node.hpp"

struct GoalNode : public Node
{
    bool m_IsGoal = true;
};