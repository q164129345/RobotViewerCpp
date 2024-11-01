#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

// 宏定义用于提取文件名
#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
// 修改后的 LOG 宏
#define LOG qDebug() << FILENAME << ":" << __LINE__ << "@" << __FUNCTION__ << ":"

// 将4字节的大端格式uint8_t数组转换为uint32_t
constexpr uint32_t toUint32BE(const uint8_t* data) {
    return (static_cast<uint32_t>(data[0]) << 24) |
           (static_cast<uint32_t>(data[1]) << 16) |
           (static_cast<uint32_t>(data[2]) << 8) |
           static_cast<uint32_t>(data[3]);
}

// 将2字节的大端格式uint8_t数组转换为uint16_t
constexpr uint16_t toUint16BE(const uint8_t* data) {
    return (static_cast<uint16_t>(data[0]) << 8) |
           static_cast<uint16_t>(data[1]);
}

// 将4字节的小端格式uint8_t数组转换为uint32_t
constexpr uint32_t toUint32LE(const uint8_t* data) {
    return (static_cast<uint32_t>(data[3]) << 24) |
           (static_cast<uint32_t>(data[2]) << 16) |
           (static_cast<uint32_t>(data[1]) << 8) |
           static_cast<uint32_t>(data[0]);
}

// 将2字节的小端格式uint8_t数组转换为uint16_t
constexpr uint16_t toUint16LE(const uint8_t* data) {
    return (static_cast<uint16_t>(data[1]) << 8) |
           static_cast<uint16_t>(data[0]);
}

// 将4字节的大端格式uint8_t数组转换为int32_t
constexpr int32_t toInt32BE(const uint8_t* data) {
    return (static_cast<int32_t>(data[0]) << 24) |
           (static_cast<int32_t>(data[1]) << 16) |
           (static_cast<int32_t>(data[2]) << 8) |
           static_cast<int32_t>(data[3]);
}

// 将2字节的大端格式uint8_t数组转换为int16_t
constexpr int16_t toInt16BE(const uint8_t* data) {
    return (static_cast<int16_t>(data[0]) << 8) |
           static_cast<int16_t>(data[1]);
}

// 将4字节的小端格式uint8_t数组转换为int32_t
constexpr int32_t toInt32LE(const uint8_t* data) {
    return (static_cast<int32_t>(data[3]) << 24) |
           (static_cast<int32_t>(data[2]) << 16) |
           (static_cast<int32_t>(data[1]) << 8) |
           static_cast<int32_t>(data[0]);
}

// 将2字节的小端格式uint8_t数组转换为int16_t
constexpr int16_t toInt16LE(const uint8_t* data) {
    return (static_cast<int16_t>(data[1]) << 8) |
           static_cast<int16_t>(data[0]);
}


#endif // COMMON_H
