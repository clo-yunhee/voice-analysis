#include <emscripten/bind.h>
#include <cmath>
#include <iostream>
#include "LPCAnalyser.h"
#include "octave.h"
#include "pitch.h"

using namespace emscripten;
using namespace Eigen;

LPCAnalyser::LPCAnalyser() noexcept {
    this->input = ArrayXd::Zero(blockCount * kRenderQuantumFrames);
    this->B = ArrayXd::Ones(1);
    this->A = ArrayXd::Ones(1);

    this->lpOrder = 18;
}

val LPCAnalyser::getData(val frequencies) {

    this->calculate();

    // Copy variables
    ArrayXd B = this->B;
    double pitch = this->pitch;

    // Calculate frequency response

    unsigned length = frequencies["length"].as<unsigned>();

    ArrayXd F(length);
    for (unsigned k = 0; k < length; ++k) {
        F(k) = frequencies[k].as<double>();
    }

    ArrayXd h = freqz(B, this->A, F, sampleRate);

    val response = val::array();

    for (unsigned k = 0; k < length; ++k) {
        val point = val::object();
        point.set("x", F(k));
        point.set("y", h(k));

        response.set(k, point);
    }

    // Construct final object.

    val r = val::object();
    r.set("response", response);
    r.set("pitch", pitch);

    return r;

}

void LPCAnalyser::process(uintptr_t inputPtr, unsigned channelCount) {

    float *input = reinterpret_cast<float*>(inputPtr);

    // Apply the filter.

    ArrayXd x(kRenderQuantumFrames);
    for (unsigned k = 0; k < kRenderQuantumFrames; ++k) {
        x(k) = input[k];
    }

    // Shift the input buffer.

    this->input.head(this->input.size() - kRenderQuantumFrames)
        = this->input.segment(kRenderQuantumFrames, this->input.size() - kRenderQuantumFrames);

    this->input.tail(kRenderQuantumFrames) = x;

}

void LPCAnalyser::calculate() {

    ArrayXd x(this->input);

    unsigned L = x.size();

    // Apply Hamming window to the input.
    ArrayXd win(L);
    for (unsigned k = 0; k < L; ++k) {
        win(k) = 0.54 - 0.46 * cos(2.0 * M_PI * double(k) / double(L - 1));
    }

    // Calculate LP coefficients.
    double pitch;
    ArrayXd coeffs = lpc(x * win, this->lpOrder);

    if (!pitch_amdf(x, &pitch)) {
        pitch = 0.0;
    }

    // Set all the calculations.
    this->B = coeffs;
    this->pitch = pitch;

}

bool isType(val value, const std::string& type) {
    return (value.typeof().as<std::string>() == type);
}

