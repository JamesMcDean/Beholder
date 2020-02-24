//
// Created by James McRoberts on 2/8/20.
//

#include "visiontools.hpp"

namespace Vision {
    auto compressFrame(const cv::Mat& rawFrame, int jpegQuality) -> struct compressedFrame {
        // Filter quality argument
        int filteredQuality;
        if (jpegQuality < 0) filteredQuality = 0;
        else if (jpegQuality > 100) filteredQuality = 100;
        else filteredQuality = jpegQuality;

        // Compress frame with JPG
        auto buffer = new std::vector<uint8_t>();
        auto parameters = std::vector<int>(2);
        parameters[0] = cv::IMWRITE_JPEG_QUALITY;
        parameters[1] = filteredQuality;
        cv::imencode(".jpg", rawFrame, *buffer, parameters);

        return {
            .quality = filteredQuality,
            .data = buffer,
            .disposed = false
        };
    }

    auto disposeFrame(struct compressedFrame* frame) -> void {
        delete[] frame->data;
        frame->disposed = true;
    }
}