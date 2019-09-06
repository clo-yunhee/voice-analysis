#pragma once

PITCH_DETECTOR(amdf) {

    constexpr double minFrequency = 70;
    constexpr double maxFrequency = 600;
    constexpr double sensitivity = 0.1;
    constexpr double ratio = 5;

    const unsigned maxPeriod = std::ceil(sampleRate / minFrequency);
    const unsigned minPeriod = std::floor(sampleRate / maxFrequency);

    const unsigned maxShift = x.size();

    ArrayXd amd = ArrayXd::Zero(maxShift);

    // Find the average magnitude difference for each possible period offset.
    for (unsigned i = 0; i < maxShift; ++i) {
        ArrayXd frame1 = x.segment(0, maxShift - i);
        ArrayXd frame2 = x.segment(i, maxShift - i);

        amd(i) = (frame1 - frame2).abs().sum();
    }

    double minVal = amd.segment(minPeriod, maxPeriod - minPeriod).minCoeff();
    double maxVal = amd.segment(minPeriod, maxPeriod - minPeriod).maxCoeff();

    const double cutoff = std::round((sensitivity * (maxVal - minVal)) + minVal);

    unsigned j;
    for (j = minPeriod; j <= maxPeriod && amd(j) > cutoff; ++j);

    const unsigned searchLength = minPeriod / 2;

    ArrayXd searchSegment = amd.segment(j - 1, std::max(searchLength, maxShift - j));

    minVal = amd(j);
    unsigned minPos = j;
    unsigned i = j;

    while ((i < j + searchLength) && (i <= maxPeriod)) {
        i++;

        double cur = amd(i);

        if (cur < minVal) {
            minVal = cur;
            minPos = i;
        }
    }

    if (std::round(amd(minPos) * ratio) < maxVal) {
        *frequency = sampleRate / double(minPos);
        return true;
    }

    return false;
}