#pragma once

#include "math.h"

#define PITCH_DETECTOR(name) bool pitch_##name (const ArrayXd& x, double *frequency)

#include "pitch/amdf.h"