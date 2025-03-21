#pragma once

#include "../../nodes.h"

struct ImageNode : Node {
    GLTextureData texture;
    cv::Vec2i resolution;
    float scale = 100.0f;

    static Node* Create() {
        ImageNode* node = new ImageNode();
        node->Title = "Image Node";
        node->OutputSlots.push_back({"Image", NodeSlotImage});
        node->OutputSlots.push_back({"Resolution", NodeSlotVec2i});
        node->state = Resolved;
        //texture
        LoadNullTexture(&node->texture);
        node->resolution = cv::Vec2i(2, 2);

        return node;
    }

    virtual ~ImageNode() {
        RemoveTexture(&texture);
    }

    virtual const TaggedData GetOutput(size_t slot) {
        switch(slot) {
            case 0: return texture.image;
            case 1: return resolution;
        }
        return std::monostate{};
    }

    //virtual bool Update() {}

    virtual void BodyRender() {
        ImguiScalableImage(&scale, texture);
        if(ImGui::Button("LI1")) {
            RemoveTexture(&texture);
            LoadTextureFromFile("../../examples/A1.png", &texture);
            resolution[0] = texture.image.width();
            resolution[1] = texture.image.height();
            SetDirty();
        }
        if(ImGui::Button("LI2")) {
            RemoveTexture(&texture);
            LoadTextureFromFile("../../examples/B1.png", &texture);
            resolution[0] = texture.image.width();
            resolution[1] = texture.image.height();
            SetDirty();
        }
    }
};

REGISTER_NODE("Image>Image Node", ImageNode);