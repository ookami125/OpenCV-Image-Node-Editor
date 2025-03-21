#pragma once

#include "../../nodes.h"
#include "datatypes.h"
#include "ImImage.h"

#include <opencv2/opencv.hpp>

struct ImageMapNode : Node {

    GLTextureData texture;
    float scale = 100.0f;

    static Node* Create() {
        ImageMapNode* node = new ImageMapNode();
        node->Title = "Image Map Node";
        node->InputSlots.push_back({"Input", NodeSlotImage});
        node->InputSlots.push_back({"M", NodeSlotFloat});
        node->InputSlots.push_back({"B", NodeSlotFloat});
        node->OutputSlots.push_back({"Output", NodeSlotImage});

        //texture
        LoadNullTexture(&node->texture);

        return node;
    }

    virtual ~ImageMapNode() {
        RemoveTexture(&texture);
    }

    virtual const TaggedData GetOutput(size_t slot) {
        switch(slot) {
            case 0: return texture.image;
        }
        return std::monostate{};
    }

    virtual NodeState Update() {
        Image image = GetInputData<Image>(0);
        float M = GetInputData<float>(1);
        float B = GetInputData<float>(2);
        
        Image img = image;
        printf("format: %d\n", img.data.type());
        CVFormat type = img.data.type();
        img.data = img.data * M + B;
        
        RemoveTexture(&texture);
        LoadTextureFromData(img, &texture);

        return Resolved;
    }

    virtual void BodyRender() {
        ImguiScalableImage(&scale, texture);
    }
};

REGISTER_NODE("Image>Image Map Node", ImageMapNode);