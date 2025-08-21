#pragma once
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>
#include <string>

class UUIDGenerator {
public:
    static std::string generate() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

        uint32_t data[4];
        for (int i = 0; i < 4; ++i) {
            data[i] = dis(gen);
        }

        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        // Format: 8-4-4-4-12
        ss << std::setw(8) << data[0] << "-";
        ss << std::setw(4) << ((data[1] >> 16) & 0xFFFF) << "-";
        ss << std::setw(4) << ((data[1]) & 0xFFFF) << "-";
        ss << std::setw(4) << ((data[2] >> 16) & 0xFFFF) << "-";
        ss << std::setw(4) << ((data[2]) & 0xFFFF);
        ss << std::setw(8) << data[3];

        return ss.str();
    }
};