#pragma once

#include "../../nodes.h"

struct Vec2iComposeNode : Node {
    cv::Vec2i vec;
    
    static Node* Create() {
        Vec2iComposeNode* node = new Vec2iComposeNode();
        node->Title = "Vec2i Compose Node";
        node->InputSlots.push_back({"X", NodeSlotInt});
        node->InputSlots.push_back({"Y", NodeSlotInt});
        node->OutputSlots.push_back({"Output", NodeSlotVec2i});
        node->state = Resolved;
        return node;
    }

    virtual const TaggedData GetOutput(size_t slot) {
        switch(slot) {
            case 0: return vec;
        }
        return std::monostate{};
    }

    virtual NodeState Update() {
        int x = GetInputData<int>(0);
        int y = GetInputData<int>(1);
        vec = cv::Vec2i(x, y);
        return Resolved;
    }
};

REGISTER_NODE("Math>Vec2i Compose Node", Vec2iComposeNode);