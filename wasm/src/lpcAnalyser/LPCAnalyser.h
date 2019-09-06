#ifndef LPC_ANALYSER_H
#define LPC_ANALYSER_H

#include <emscripten/val.h>
#include <Eigen/Core>
#include <array>
#include <cmath>
#include <mutex>
#include <string>
#include <thread>

constexpr unsigned kRenderQuantumFrames = 128;
constexpr unsigned kBytesPerChannel = kRenderQuantumFrames * sizeof(float);
constexpr float sampleRate = 48000;

// How many blocks do we need for ~20ms of data.
const unsigned blockCount = std::ceil((20.0 / 1000.0) / (kRenderQuantumFrames / sampleRate));

class LPCAnalyser {
public:
    LPCAnalyser() noexcept;
    virtual ~LPCAnalyser() = default;

    emscripten::val getData(emscripten::val frequencies);

    void process(uintptr_t inputPtr, unsigned channelCount);

private:
    void calculate();

    std::mutex mutexInput;
    std::mutex mutexCalc;

    std::thread calcThread;
    unsigned lpOrder;

    Eigen::ArrayXd input;
    Eigen::ArrayXd B, A;
    double pitch;
};

bool isType(emscripten::val value, const std::string& type);

#endif // VT_FILTER_H
