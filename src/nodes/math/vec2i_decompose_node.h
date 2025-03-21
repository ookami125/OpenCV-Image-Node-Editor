#pragma once

#include "../../nodes.h"

struct Vec2iDecomposeNode : Node {
    int x;
    int y;
    
    static Node* Create() {
        Vec2iDecomposeNode* node = new Vec2iDecomposeNode();
        node->Title = "Vec2i Decompose Node";
        node->InputSlots.push_back({"Input", NodeSlotVec2i});
        node->OutputSlots.push_back({"X", NodeSlotInt});
        node->OutputSlots.push_back({"Y", NodeSlotInt});
        node->state = Resolved;
        return node;
    }

    virtual const TaggedData GetOutput(size_t slot) {
        switch(slot) {
            case 0: return x;
            case 1: return y;
        }
        return std::monostate{};
    }

    virtual NodeState Update() {
        cv::Vec2i vec2 = GetInputData<cv::Vec2i>(0);
        x = vec2[0];
        y = vec2[1];
        return Resolved;
    }
};

REGISTER_NODE("Math>Vec2i Decompose Node", Vec2iDecomposeNode);