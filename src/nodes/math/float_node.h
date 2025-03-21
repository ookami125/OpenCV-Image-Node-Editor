#pragma once

#include "../../nodes.h"

struct FloatNode : Node {
    float value;
    
    static Node* Create() {
        FloatNode* node = new FloatNode();
        node->Title = "Float Node";
        node->OutputSlots.push_back({"Output", NodeSlotFloat});
        node->state = Resolved;
        return node;
    }

    virtual ~FloatNode() {}

    virtual const TaggedData GetOutput(size_t slot) {
        switch(slot) {
            case 0: return value;
        }
        return std::monostate{};
    }

    virtual void BodyRender() {
        float newValue = value;
        ImGui::PushItemWidth(100 * ImNodes::GetCurrentCanvas()->Zoom);
        ImGui::InputFloat("##input", &newValue);
        ImGui::PopItemWidth();
        if(newValue != value) {
            value = newValue;
            SetDirty();
        }
    }
};

REGISTER_NODE("Math>Float Node", FloatNode);