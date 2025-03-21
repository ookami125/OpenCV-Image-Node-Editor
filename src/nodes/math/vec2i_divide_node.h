#pragma once

#include "../../nodes.h"

struct Vec2iDivideNode : Node {
    cv::Vec2i value;
    
    static Node* Create() {
        Vec2iDivideNode* node = new Vec2iDivideNode();
        node->Title = "Vec2i Divide Node";
        node->InputSlots.push_back({"Input", NodeSlotVec2i});
        node->InputSlots.push_back({"Divisor", NodeSlotInt});
        node->OutputSlots.push_back({"Output", NodeSlotVec2i});
        node->state = Resolved;
        return node;
    }

    virtual const TaggedData GetOutput(size_t slot) {
        switch(slot) {
            case 0: return value;
        }
        return std::monostate{};
    }

    virtual NodeState Update() {
        cv::Vec2i vec2 = GetInputData<cv::Vec2i>(0);
        int32_t divisor = GetInputData<int32_t>(1);
        value = vec2 / divisor;
        return Resolved;
    }
};

REGISTER_NODE("Math>Vec2i Divide Node", Vec2iDivideNode);