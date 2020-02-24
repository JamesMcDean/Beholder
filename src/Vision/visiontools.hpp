//
// Created by James McRoberts on 2/8/20.
//

#ifndef BEHOLDER_VISIONTOOLS_HPP
#define BEHOLDER_VISIONTOOLS_HPP

#include <cstdint>
#include <opencv2/opencv.hpp>

namespace Vision {
    /**
     * Holds the frame data compressed from the compressFrame() method.
     */
    struct compressedFrame {
        int quality{};
        std::vector<uint8_t>* data{};
        bool disposed = false;
    };

    /**
     * Compresses a frame using the JPG format for sending.
     * @param rawFrame The raw opencv format matrix (cv::Mat) to compress.
     * @param jpegQuality The quality of the JPG in the range of [0, 100].
     * @return The compressed frame data.
     */
    auto compressFrame(const cv::Mat& rawFrame, int jpegQuality) -> struct compressedFrame;

    /**
     * Disposes of the frame provided.
     * @param frame The frame to dispose.
     */
    auto disposeFrame(struct compressedFrame* frame) -> void;
};

#endif //BEHOLDER_VISIONTOOLS_HPP
