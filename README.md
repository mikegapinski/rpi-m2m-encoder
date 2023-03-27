### Raspberry Pi Video Encoder Library (rpi-m2m-encoder)

This library provides a C++ interface for encoding video on the Raspberry Pi using the Video4Linux (V4L2) API.

## Usage

The main file main.cpp shows an example of how to use the library to encode a single frame of video to JPEG format. The example sets up a dummy RGB32 frame, converts it to RGB24 format, compresses it using the JPEG encoder, and writes the output to a file.

To use the library in your own project, include the m2m.h header file and link against the shared object library (libm2m.so).

## Dependencies

The library has the following dependencies:
- Video4Linux2 (V4L2) API
- ARM NEON instructions (optional, for faster RGB32 to RGB24 conversion)

## License

This library is released under the GPL 3 license. See LICENSE file for more information.

## Authors

- Michał Gapiński (mike@gapinski.eu)
- Maxim Devaev (mdevaev@gmail.com)
