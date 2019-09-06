#pragma once

template<typename PolyType, typename ArrayType>
ArrayType polyval(const PolyType& p, const ArrayType& x) {

    using Scalar = typename ArrayType::Scalar;
    using ArrayXs = Array<Scalar, Dynamic, 1>;

    ArrayXs y(x.size());

    y.setConstant(p(0));

    for (unsigned i = 1; i < p.size() - 1; ++i) {
        y = y * x + Scalar(p(i));
    }

    return y;

}
