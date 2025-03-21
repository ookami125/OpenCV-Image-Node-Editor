#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <variant>

#include <opencv2/opencv.hpp>

struct Image {
    cv::Mat data;

    Image() : data(cv::Mat()) {}

    Image(cv::Mat data) :
        data(data)
    {}

    Image(uint8_t* data, size_t width, size_t height, int cv_type) :
        data(cv::Mat(height, width, cv_type, data))
    {}

    Image(size_t width, size_t height, size_t channels) :
        data(cv::Mat4b(height, width, 0))
    {}

    Image(const Image& other) : data(other.data.clone()) {}

    Image(Image&& rhs) = delete;
    Image& operator=(Image&& rhs) = delete;

    ~Image() {}

    const Image& operator=(const Image& other) {
        data = other.data.clone();
        return *this;
    }

    size_t width() const { return data.size().width; }
    size_t height() const { return data.size().height; }
    size_t channels() const { return data.channels(); }
};

struct CVFormat {
    int _inner;
    CVFormat() : _inner(0) {};
    CVFormat(int _inner) : _inner(_inner) {};
    bool operator==(const CVFormat& rhs) { return _inner == rhs._inner; }
    bool operator!=(const CVFormat& rhs) { return _inner != rhs._inner; }
    operator int() const { return _inner; }
};
template <>
struct std::hash<CVFormat>
{
  std::size_t operator()(const CVFormat& k) const
  {
    return std::hash<int>()(k._inner);
  }
};

std::variant<
    std::monostate,
    float,
    int,
    Image,
    CVFormat,
    cv::Vec2i
> typedef TaggedData;

//class TaggedData
// {
//     enum uint8_t {
//         Nil,
//         Image,
//         CVFormat,
//     } type;
//     union {
//         ::Image image;
//         ::CVFormat format;
//     };

// public:
//     TaggedData() :
//         type(Nil)
//     {};

//     TaggedData(::Image& image) :
//         type(Image),
//         image(image)
//     {};

//     TaggedData(::CVFormat& format) :
//         type(Image),
//         format(format)
//     {};

//     static void shallowCopy(TaggedData* dest, const TaggedData* src) {
//         memcpy(dest, src, sizeof(TaggedData));
//     }

//     TaggedData(const TaggedData& other) :
//         type(other.type)
//     {
//         switch(type) {
//             case Image: this->image = other.image; break;
//             default: {

//             }
//         }
//     }
// };