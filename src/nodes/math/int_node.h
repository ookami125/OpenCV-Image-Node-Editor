#pragma once

#include "../../nodes.h"

struct IntNode : Node {
    int value;
    
    static Node* Create() {
        IntNode* node = new IntNode();
        node->Title = "Int Node";
        node->OutputSlots.push_back({"Output", NodeSlotInt});
        node->state = Resolved;
        return node;
    }

    virtual ~IntNode() {}

    virtual const TaggedData GetOutput(size_t slot) {
        switch(slot) {
            case 0: return value;
        }
        return std::monostate{};
    }

    virtual void BodyRender() {
        int newValue = value;
        ImGui::PushItemWidth(100 * ImNodes::GetCurrentCanvas()->Zoom);
        ImGui::InputInt("", &newValue);
        ImGui::PopItemWidth();
        if(newValue != value) {
            value = newValue;
            SetDirty();
        }
    }
};

REGISTER_NODE("Math>Int Node", IntNode);