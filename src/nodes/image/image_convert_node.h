#pragma once

#include "../../nodes.h"
#include "datatypes.h"
#include "ImImage.h"

#include <opencv2/opencv.hpp>

struct ImageConvertNode : Node {

    GLTextureData texture;
    float scale = 100.0f;

    static Node* Create() {
        ImageConvertNode* node = new ImageConvertNode();
        node->Title = "Image Convert Node";
        node->InputSlots.push_back({"Input", NodeSlotImage});
        node->InputSlots.push_back({"Format", NodeSlotFormat});
        node->OutputSlots.push_back({"Output", NodeSlotImage});

        //texture
        LoadNullTexture(&node->texture);

        return node;
    }

    virtual ~ImageConvertNode() {
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
        CVFormat format = GetInputData<CVFormat>(1);
        
        Image img = image;
        cv::Mat convertdImage;
        printf("convertToChannels: %zu\n", img.channels());
        img.data.convertTo(convertdImage, format, 1.0f/255.0f);
        img.data = convertdImage; 
        
        printf("convertToChannels(2): %zu\n", img.channels());

        RemoveTexture(&texture);
        LoadTextureFromData(img, &texture);

        return Resolved;
    }

    virtual void BodyRender() {
        ImguiScalableImage(&scale, texture);
    }
};

REGISTER_NODE("Image>Image Convert Node", ImageConvertNode);