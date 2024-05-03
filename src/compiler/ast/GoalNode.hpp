#pragma once

#include "Node.hpp"

struct GoalNode : public Node
{
    bool m_IsGoal = true;
    bool m_IsArg = false;
};