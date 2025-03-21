#pragma once

#include "../../nodes.h"

template<typename T>
class ComboMap {
public:
    // An entry holds a sparse value and its associated label.
    struct Entry {
        T value;
        std::string label;
    };

    // Construct the ComboMap from a vector of entries.
    ComboMap(const std::vector<Entry>& entries) : entries_(entries) {
        for (size_t i = 0; i < entries_.size(); ++i) {
            value_to_index_[entries_[i].value] = static_cast<int>(i);
        }
    }

    // Convert a sparse value to its linear index.
    int toIndex(const T& value) const {
        auto it = value_to_index_.find(value);
        return (it != value_to_index_.end()) ? it->second : 0;
    }

    // Convert a linear index back to the sparse value.
    T toValue(int index) const {
        return (index >= 0 && index < static_cast<int>(entries_.size()))
            ? entries_[index].value : T();
    }

    // Return the number of entries.
    int count() const { return static_cast<int>(entries_.size()); }

    // Retrieve the label for the given index.
    const char* getLabel(int index) const {
        return (index >= 0 && index < static_cast<int>(entries_.size()))
            ? entries_[index].label.c_str() : "";
    }

    static const char* itemGetter(void* data, int idx) {
        ComboMap<T>* map = static_cast<ComboMap<T>*>(data);
        if (idx < 0 || idx >= map->count())
            return nullptr;
        return map->getLabel(idx);
    };

private:
    std::vector<Entry> entries_;
    std::unordered_map<T, int> value_to_index_;
};

static std::vector<ComboMap<CVFormat>::Entry> entries = {
    {CV_8UC3, "CV_8UC3"},
    {CV_8UC4, "CV_8UC4"},
    {CV_32FC3, "CV_32FC3"},
    {CV_32FC4, "CV_32FC4"},
};
ComboMap<CVFormat> items(entries);

struct FormatNode : Node {
    CVFormat value;

    static Node* Create() {
        FormatNode* node = new FormatNode();
        node->Title = "Format Node";
        node->OutputSlots.push_back({"Output", NodeSlotFormat});
        node->state = Resolved;
        return node;
    }

    virtual ~FormatNode() {}

    virtual const TaggedData GetOutput(size_t slot) {
        switch(slot) {
            case 0: return value;
        }
        return std::monostate{};
    }

    virtual void BodyRender() {
        int currentIndex = items.toIndex(value);
        ImGui::PushItemWidth(100);
        ImGui::Combo("", &currentIndex, ComboMap<CVFormat>::itemGetter, (void*)&items, items.count());
        ImGui::PopItemWidth();
        CVFormat newValue = items.toValue(currentIndex);
        if(newValue != value) {
            printf("FormatNode::value : %d\n", (int)newValue);
            value = newValue;
            SetDirty();
        }
    }
};

REGISTER_NODE("Image>Format Node", FormatNode);