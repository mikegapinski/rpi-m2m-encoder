#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
#include "m2m.h"
#include <arm_neon.h>

void rgb32_to_rgb24(uint8_t *rgb32, uint8_t *rgb24, size_t width, size_t height) {
    size_t num_pixels = width * height;
    size_t num_iterations = num_pixels / 8;

    uint8x8x4_t input;
    uint8x8x3_t output;

    for (size_t i = 0; i < num_iterations; ++i) {
        // Load 8 pixels (32 bytes) from the RGB32 buffer
        input = vld4_u8(rgb32);

        // Store the RGB channels to the RGB24 buffer
        output.val[0] = input.val[0];
        output.val[1] = input.val[1];
        output.val[2] = input.val[2];
        vst3_u8(rgb24, output);

        // Move to the next set of pixels
        rgb32 += 32;
        rgb24 += 24;
    }

    // Handle any remaining pixels that don't fit in a multiple of 8
    size_t remaining_pixels = num_pixels % 8;
    for (size_t i = 0; i < remaining_pixels; ++i) {
        rgb24[0] = rgb32[0];
        rgb24[1] = rgb32[1];
        rgb24[2] = rgb32[2];

        rgb32 += 4;
        rgb24 += 3;
    }
}

int main(int argc, char *argv[]) {
    // Initialize the JPEG encoder
    us_m2m_encoder_s *encoder = us_m2m_jpeg_encoder_init("encoder", "/dev/video31", 90);

    // Dummy frame dimensions and format
    unsigned width = 1920;
    unsigned height = 1080;
    unsigned bytes_per_pixel = 4; // RGB32

    // Create a dummy RGB32 frame
    us_frame_s input_frame;
    input_frame.width = width;
    input_frame.height = height;
    input_frame.format = V4L2_PIX_FMT_RGB32;
    input_frame.stride = width * bytes_per_pixel;
    input_frame.used = static_cast<size_t>(width) * static_cast<size_t>(height) * bytes_per_pixel;
    input_frame.data = static_cast<uint8_t*>(malloc(input_frame.used));

    // Fill the dummy frame with a solid color (e.g., red)
    for (size_t i = 0; i < input_frame.used; i += bytes_per_pixel) {
        input_frame.data[i] = 0;         // B
        input_frame.data[i + 1] = 0;     // G
        input_frame.data[i + 2] = 255;   // R
        input_frame.data[i + 3] = 255;   // A (Alpha)
    }

    // Create a buffer for the RGB24 frame
    size_t rgb24_frame_size = static_cast<size_t>(width) * static_cast<size_t>(height) * 3;
    uint8_t *rgb24_frame_data = static_cast<uint8_t*>(malloc(rgb24_frame_size));

    // Convert RGB32 frame to RGB24
    rgb32_to_rgb24(input_frame.data, rgb24_frame_data, width, height);

    // Update the input_frame to use the RGB24 data
    input_frame.format = V4L2_PIX_FMT_RGB24;
    input_frame.stride = width * 3;
    input_frame.used = rgb24_frame_size;
    input_frame.data = rgb24_frame_data;

    // Initialize the output frame
    us_frame_s output_frame;
    output_frame.width = width;
    output_frame.height = height;
    output_frame.format = V4L2_PIX_FMT_JPEG;
    output_frame.stride = 0;
    output_frame.used = 0;
    output_frame.data = static_cast<uint8_t*>(malloc(width * height * bytes_per_pixel)); // Just a rough estimation, will be resized later if needed

    // Compress the frame using the encoder
    bool force_key = false;
    int result = us_m2m_encoder_compress(encoder, &input_frame, &output_frame, force_key);

    if (result != 0) {
        printf("Error compressing frame: %d\n", result);
        return 1;
    }

    // Write the output frame data to the output file
    FILE *output_file = fopen("output.jpg", "wb");
    if (output_file == NULL) {
        perror("Error opening output file");
        return 1;
    }
    fwrite(output_frame.data, 1, output_frame.used, output_file);
    fclose(output_file);

    // Destroy the encoder and clean up
    us_m2m_encoder_destroy(encoder);
    free(input_frame.data);
    free(output_frame.data);

    return 0;
}