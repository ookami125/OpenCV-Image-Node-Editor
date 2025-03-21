#pragma once

#include "../../nodes.h"
#include "datatypes.h"
#include "ImImage.h"

struct ImageSubtractNode : Node {

    GLTextureData texture;
    float scale = 100.0f;

    static Node* Create() {
        ImageSubtractNode* node = new ImageSubtractNode();
        node->Title = "Image Subtract Node";
        node->InputSlots.push_back({"Input 1", NodeSlotImage});
        node->InputSlots.push_back({"Input 2", NodeSlotImage});
        node->OutputSlots.push_back({"Output", NodeSlotImage});

        //texture
        LoadNullTexture(&node->texture);

        return node;
    }

    virtual ~ImageSubtractNode() {
        RemoveTexture(&texture);
    }

    virtual const TaggedData GetOutput(size_t slot) {
        switch(slot) {
            case 0: return texture.image;
        }
        return std::monostate{};
    }

    virtual NodeState Update() {
        Image image0 = GetInputData<Image>(0);
        Image image1 = GetInputData<Image>(1);

        if(image0.width() != image1.width()) return Error;
        if(image0.height() != image1.height()) return Error;
        if(image0.channels() != image1.channels()) return Error;

        RemoveTexture(&texture);
        Image img0 = image0;
        const Image& img1 = image1;
        img0.data -= img1.data;
        LoadTextureFromData(img0, &texture);
    
        return Resolved;
    }

    virtual void BodyRender() {
        ImguiScalableImage(&scale, texture);
    }
};

REGISTER_NODE("Image>Image Subtract Node", ImageSubtractNode);