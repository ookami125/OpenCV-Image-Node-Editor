#pragma once

#include "../../nodes.h"
#include "datatypes.h"
#include "ImImage.h"

#include <opencv2/opencv.hpp>

struct ImageSiftNode : Node {

    GLTextureData texture;
    cv::Mat4f matrix;
    float scale = 100.0f;

    static Node* Create() {
        ImageSiftNode* node = new ImageSiftNode();
        node->Title = "Image Feature Warping Node";
        node->InputSlots.push_back({"Input 1", NodeSlotImage});
        node->InputSlots.push_back({"Input 2", NodeSlotImage});
        node->OutputSlots.push_back({"Output", NodeSlotImage});

        LoadNullTexture(&node->texture);

        return node;
    }

    virtual ~ImageSiftNode() {
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

        cv::Mat descriptor_1, descriptor_2;

        std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

        cv::Ptr<cv::ORB> detector = cv::ORB::create();
        std::vector<cv::DMatch> matches;
        cv::BFMatcher matcher;

        detector->detectAndCompute(image0.data, cv::noArray(), keypoints_1, descriptor_1);
        detector->detectAndCompute(image1.data, cv::noArray(), keypoints_2, descriptor_2);

        matcher.match(descriptor_1, descriptor_2, matches);

        std::vector< cv::DMatch > good_matches;
        float average_distance = 0;
        for( int i = 0; i < descriptor_1.rows; i++ )
        {
            average_distance += matches[i].distance;
        }
        average_distance /= descriptor_1.rows;
        average_distance *= 0.75;
        for( int i = 0; i < descriptor_1.rows; i++ )
        {
            auto distance = matches[i].distance;
            if( distance <= average_distance )
            {
                good_matches.push_back( matches[i]);
            }
        }

        std::vector<cv::Point2f> points1, points2;
        float totalErrorDistance = 0.0f;
        for( size_t i = 0; i < good_matches.size(); i++ )
        {
            auto p1 = keypoints_1[ good_matches[i].queryIdx ].pt;
            auto p2 = keypoints_2[ good_matches[i].trainIdx ].pt;
            points1.push_back( p1 );
            points2.push_back( p2 );
            
            totalErrorDistance += cv::norm(p1-p2);
        }
        printf("Total Error Distance: %f\n", totalErrorDistance);
        printf("Average Error Distance: %f\n", totalErrorDistance / good_matches.size());
        
        cv::Mat homographyMatrix = cv::findHomography(points1, points2, cv::RANSAC);
        
        
        std::cout << homographyMatrix << std::endl;
        
        cv::Mat output;
        cv::warpPerspective(image0.data, output, homographyMatrix, image1.data.size());
        
        RemoveTexture(&texture);
        LoadTextureFromData(Image{output}, &texture);

        return Resolved;
    }

    virtual void BodyRender() {
        ImguiScalableImage(&scale, texture);
    }
};

REGISTER_NODE("Image>Image Feature Warping Node", ImageSiftNode);