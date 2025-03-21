#pragma once

#include "../../nodes.h"

struct TestNode : Node {
    static Node* Create() {
        TestNode* node = new TestNode();
        node->Title = "Test Node";
        node->InputSlots.push_back({"NodeSlotAny", NodeSlotAny});
        node->InputSlots.push_back({"NodeSlotInt", NodeSlotInt});
        node->InputSlots.push_back({"NodeSlotFloat", NodeSlotFloat});
        node->InputSlots.push_back({"NodeSlotVec2", NodeSlotVec2i});
        node->InputSlots.push_back({"NodeSlotVec3", NodeSlotVec3f});
        node->InputSlots.push_back({"NodeSlotVec4", NodeSlotVec4f});
        node->InputSlots.push_back({"NodeSlotMatrix3x3", NodeSlotMatrix3x3f});
        node->InputSlots.push_back({"NodeSlotMatrix4x4", NodeSlotMatrix4x4f});
        node->InputSlots.push_back({"NodeSlotImage", NodeSlotImage});
        node->OutputSlots.push_back({"NodeSlotAny", NodeSlotAny});
        node->OutputSlots.push_back({"NodeSlotInt", NodeSlotInt});
        node->OutputSlots.push_back({"NodeSlotFloat", NodeSlotFloat});
        node->OutputSlots.push_back({"NodeSlotVec2", NodeSlotVec2i});
        node->OutputSlots.push_back({"NodeSlotVec3", NodeSlotVec3f});
        node->OutputSlots.push_back({"NodeSlotVec4", NodeSlotVec4f});
        node->OutputSlots.push_back({"NodeSlotMatrix3x3", NodeSlotMatrix3x3f});
        node->OutputSlots.push_back({"NodeSlotMatrix4x4", NodeSlotMatrix4x4f});
        node->OutputSlots.push_back({"NodeSlotImage", NodeSlotImage});
        return node;
    }
};

REGISTER_NODE("Debug>Test Node", TestNode);