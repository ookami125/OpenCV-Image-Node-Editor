#pragma once

#include <vector>

#include <imgui.h>
#include <ImNodes.h>
#include <ImNodesEz.h>

struct Node;

struct Connection
{
    Node* InputNode = nullptr;
    const char* InputSlot = nullptr;
    Node* OutputNode = nullptr;
    const char* OutputSlot = nullptr;

    bool operator==(const Connection& other) const
    {
        return InputNode == other.InputNode &&
               InputSlot == other.InputSlot &&
               OutputNode == other.OutputNode &&
               OutputSlot == other.OutputSlot;
    }

    bool operator!=(const Connection& other) const
    {
        return !operator ==(other);
    }
};

enum NodeSlotTypes
{
    NodeSlotAny = 1,
    NodeSlotInt,
    NodeSlotFloat,
    NodeSlotVec2i,
    NodeSlotVec2f,
    NodeSlotVec3f,
    NodeSlotVec4f,
    NodeSlotMatrix3x3f,
    NodeSlotMatrix4x4f,
    
    NodeSlotImage,
    NodeSlotFormat,
};

enum NodeState : uint8_t
{
    Error,
    Unresolvable,
    Resolvable,
    Resolved,
};

struct Node
{
    const char* Title = nullptr;
    bool Selected = false;
    ImVec2 Pos{};
    NodeState state = Resolvable;
    
    std::vector<Connection> Connections{};
    std::vector<ImNodes::Ez::SlotInfo> InputSlots{};
    std::vector<ImNodes::Ez::SlotInfo> OutputSlots{};

    virtual ~Node() {};

    void SetDirty()
    {
        if(state == Resolved || state == Unresolvable) {
            state = Resolvable;
        }
    }
    
    bool GetDirty() { return state == Resolvable; }

    virtual bool CheckResolvable()
    {
        if(state == Unresolvable) state = Resolvable;
        return true;
    }
    
    size_t GetOutputSlotID(const char* slotName)
    {
        for(size_t i=0; i<OutputSlots.size(); ++i)
        {
            const ImNodes::Ez::SlotInfo slot = OutputSlots[i];
            if(strcmp(slot.title, slotName) == 0) return i;
        }
        return -1;
    }

    virtual const TaggedData GetOutput(size_t slot) { return TaggedData(); }

    std::pair<Node*, size_t> GetInputConnection(int id) {
        for(Connection connection : Connections) {
            if(connection.InputNode == this && strcmp(connection.InputSlot, InputSlots[id].title) == 0) {
                return { connection.OutputNode, connection.OutputNode->GetOutputSlotID(connection.OutputSlot) };
            }
        }
        return {nullptr, -1};
    }

    TaggedData GetInputData(int id) {
        std::pair<Node*, size_t> inputConnection = GetInputConnection(id);
        if(inputConnection.first == nullptr) {
            throw Unresolvable;
        }
        if(inputConnection.first != nullptr && inputConnection.second == -1) {
            throw Error;
        }
        return inputConnection.first->GetOutput(inputConnection.second);
    }

    template<typename T>
    T GetInputData(int id) {
        TaggedData data = GetInputData(id);
        T* data_ref = std::get_if<T>(&data);
        if(data_ref == nullptr) {
            throw Error;
        }
        return *data_ref;
    }

    virtual NodeState Update() { return Resolved; }

    virtual void BodyRender() {}

    virtual void Render()
    {
        auto node = this;
        bool borderColorSet = true;
        switch(state) {
            case Error: ImNodes::Ez::PushStyleColor(ImNodesStyleCol_NodeBorder, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); break;
            case Unresolvable: ImNodes::Ez::PushStyleColor(ImNodesStyleCol_NodeBorder, ImVec4(0.5f, 0.0f, 0.0f, 1.0f)); break;
            case Resolvable: ImNodes::Ez::PushStyleColor(ImNodesStyleCol_NodeBorder, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); break;
            default: borderColorSet = false; break;
        }
        if (ImNodes::Ez::BeginNode(this, Title, &Pos, &Selected))
        {
            ImNodes::Ez::InputSlots(InputSlots.data(), InputSlots.size());
            BodyRender();//ImGui::Text("Content of %s", Title);
            ImNodes::Ez::OutputSlots(OutputSlots.data(), OutputSlots.size());
            Connection new_connection;
            if (ImNodes::GetNewConnection((void**)&new_connection.InputNode, &new_connection.InputSlot,
            (void**)&new_connection.OutputNode, &new_connection.OutputSlot))
            {
                new_connection.InputNode->AddConnection(new_connection);
                new_connection.OutputNode->AddConnection(new_connection);
                new_connection.InputNode->SetDirty();
            }
            
            for (const Connection& connection : Connections)
            {
                if (connection.OutputNode != node)
                continue;
                
                if (!ImNodes::Connection(connection.InputNode, connection.InputSlot, connection.OutputNode, connection.OutputSlot))
                {
                    connection.InputNode->DeleteConnection(connection);
                    connection.OutputNode->DeleteConnection(connection);
                }
            }
            
            ImNodes::Ez::EndNode();
        }
        if(borderColorSet) ImNodes::Ez::PopStyleColor();
    }

    void AddConnection(const Connection& connection) {
        Connections.push_back(connection);
        if(state == Resolved || state == Unresolvable) {
            state = Resolvable;
        }
    }

    void DeleteConnection(const Connection& connection)
    {
        for (auto it = Connections.begin(); it != Connections.end(); ++it)
        {
            if (connection == *it)
            {
                Connections.erase(it);
                break;
            }
        }
        state = Resolvable;
    }
};

void ImguiScalableImage(float* scale, const GLTextureData& texture) {
    float zoom = ImNodes::GetCurrentCanvas()->Zoom;
    ImVec2 buttonSize = ImVec2( 16*zoom, 16*zoom );
    ImVec2 textboxSize = ImVec2( 64*zoom, 16*zoom );
    if(ImGui::Button("-", buttonSize)) {
        *scale -= 10.f;
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(textboxSize.x);
    ImGui::InputFloat("##size", scale);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if(ImGui::Button("+", buttonSize)) {
        *scale += 10.f;
    }
    int tempScale = *scale * ImNodes::GetCurrentCanvas()->Zoom; 
    int newWidth = tempScale;
    int newHeight = newWidth * ((float)texture.image.height() / (float)texture.image.width());
    ImGui::Image((ImTextureID)texture.id, ImVec2(newWidth, newHeight));
}

static std::map<std::string, Node*(*)()> available_nodes = {};
#define REGISTER_NODE(name, func) bool registered_##func = []{ available_nodes.insert({name, func::Create}); return 1; }()

//typedef std::pair<Node*, size_t> OutputRef;

//#define AssertConnection(output_ref) ({ \
//    auto& ref = (output_ref);\
//    ref.first->GetOutput(ref.second); \
//})

#include "nodes/all_nodes.h"