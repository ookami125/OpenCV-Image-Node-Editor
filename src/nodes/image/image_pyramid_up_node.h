#pragma once

#include "../../nodes.h"
#include "datatypes.h"
#include "ImImage.h"

#include <opencv2/opencv.hpp>

struct ImagePyramidUpNode : Node {

    GLTextureData texture;
    float scale = 100.0f;

    static Node* Create() {
        ImagePyramidUpNode* node = new ImagePyramidUpNode();
        node->Title = "Image Pyramid Up Node";
        node->InputSlots.push_back({"Input", NodeSlotImage});
        node->OutputSlots.push_back({"Output", NodeSlotImage});

        //texture
        LoadNullTexture(&node->texture);

        return node;
    }

    virtual ~ImagePyramidUpNode() {
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
        
        Image img;
        cv::pyrUp(image.data, img.data);

        RemoveTexture(&texture);
        LoadTextureFromData(img, &texture);

        return Resolved;
    }

    virtual void BodyRender() {
        ImguiScalableImage(&scale, texture);
    }
};

REGISTER_NODE("Image>Image Pyramid Up Node", ImagePyramidUpNode);