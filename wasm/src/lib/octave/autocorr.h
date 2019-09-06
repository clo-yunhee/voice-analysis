#pragma once

ArrayXd lpc_autocorr(const ArrayXd& x, double *pitch) {
    double norm, temp;
    int n, i, j, k;
    int len = x.size();

    ArrayXd y(len+1);

    // refer to pp. 89 for variable name consistency
    for ( n = 0; n < len; n++ )
    {
        temp = 0.0;
        for ( i = 0; i < len - n - 1; i++ )
            temp += x(i) * x(i+n);
        y(n) = temp;
    }

    // set temp to the first element of y
    temp = y(0);
    // why?
    j = (unsigned int)(len * 0.02);
    // loop to the point y stops descreasing
    while( y(j) < temp && j < len )
    {
        temp = y(j);
        j++;
    }

    // yes
    temp = 0.0;
    // find the max between j and the end
    for( i = j; i < len/2; i++ )
    {
        if( y(i) > temp)
        {
            j = i;
            temp = y(i);
        }
    }

    // why are we doing this?
    norm = 1.0f / len;
    k = len;

    // normalize, we think
    for( i = 0; i < len; i++ )
        y(i) *= (k-i) * norm;

    if( (y(j) / y(0)) < 0.4 ) j = 0;
    if( j > len / 4 ) j = 0;

    // we return the pitch information
    *pitch = double(j);

    return y;
}