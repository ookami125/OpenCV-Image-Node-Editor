#pragma once

#include "../../nodes.h"
#include "datatypes.h"
#include "ImImage.h"

#include <opencv2/opencv.hpp>

static std::vector<ComboMap<cv::InterpolationFlags>::Entry> interpolationEntries = {
    {cv::INTER_NEAREST, "INTER_NEAREST"},
    {cv::INTER_LINEAR, "INTER_LINEAR"},
    {cv::INTER_CUBIC, "INTER_CUBIC"},
    {cv::INTER_AREA, "INTER_AREA"},
    {cv::INTER_LANCZOS4, "INTER_LANCZOS4"},
    {cv::INTER_LINEAR_EXACT, "INTER_LINEAR_EXACT"},
    {cv::INTER_NEAREST_EXACT, "INTER_NEAREST_EXACT"},
    {cv::INTER_MAX, "INTER_MAX"},
    {cv::WARP_FILL_OUTLIERS, "WARP_FILL_OUTLIERS"},
};
ComboMap<cv::InterpolationFlags> interpolationComboMap(interpolationEntries);


struct ImageResizeNode : Node {

    cv::InterpolationFlags intpolationFlag = cv::INTER_LINEAR;
    GLTextureData texture;
    float scale = 100.0f;

    static Node* Create() {
        ImageResizeNode* node = new ImageResizeNode();
        node->Title = "Image Resize Node";
        node->InputSlots.push_back({"Input", NodeSlotImage});
        node->InputSlots.push_back({"Resolution", NodeSlotVec2i});
        node->OutputSlots.push_back({"Output", NodeSlotImage});

        //texture
        LoadNullTexture(&node->texture);

        return node;
    }

    virtual ~ImageResizeNode() {
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
        cv::Size resolution = cv::Size(GetInputData<cv::Vec2i>(1));

        if(resolution.width <= 0) return Unresolvable;
        if(resolution.height <= 0) return Unresolvable;

        cv::Mat resizedImage;
        cv::resize(image.data, resizedImage, resolution, 0, 0, intpolationFlag);
        
        RemoveTexture(&texture);
        LoadTextureFromData(Image{resizedImage}, &texture);

        return Resolved;
    }

    virtual void BodyRender() {
        int currentIndex = interpolationComboMap.toIndex(intpolationFlag);
        ImGui::PushItemWidth(100);
        ImGui::Combo("##interp", &currentIndex, ComboMap<cv::InterpolationFlags>::itemGetter, (void*)&interpolationComboMap, interpolationComboMap.count());
        ImGui::PopItemWidth();
        cv::InterpolationFlags newValue = interpolationComboMap.toValue(currentIndex);
        if(newValue != intpolationFlag) {
            intpolationFlag = newValue;
            SetDirty();
        }

        ImguiScalableImage(&scale, texture);
    }
};

REGISTER_NODE("Image>Image Resize Node", ImageResizeNode);