#pragma once

ArrayXd lpc(const ArrayXd& x, unsigned m) {

    double aut[m+1];
    ArrayXd lpc(m);
    double error;
    double epsilon;
    int n = x.size();
    int i,j;

    /* autocorrelation, p+1 lag coefficients */
    j=m+1;
    while(j--){
        double d=0; /* double needed for accumulator depth */
        for(i=j;i<n;i++)d+=x[i]*x[i-j];
        aut[j]=d;
    }

    /* Generate lpc coefficients from autocorr values */

    /* set our noise floor to about -100dB */
    error=aut[0] * (1. + 1e-10);
    epsilon=1e-9*aut[0]+1e-10;

    for(i=0;i<m;i++){
        double r= -aut[i+1];

        if(error<epsilon){
            for(j=i;j<m;j++)lpc[j]=0;
            goto done;
        }

        /* Sum up this iteration's reflection coefficient; note that in
           Vorbis we don't save it.  If anyone wants to recycle this code
           and needs reflection coefficients, save the results of 'r' from
           each iteration. */

        for(j=0;j<i;j++)r-=lpc[j]*aut[i-j];
        r/=error;

        /* Update LPC coefficients and total error */

        lpc[i]=r;
        for(j=0;j<i/2;j++){
            double tmp=lpc[j];

            lpc[j]+=r*lpc[i-1-j];
            lpc[i-1-j]+=r*tmp;
        }
        if(i&1)lpc[j]+=lpc[j]*r;

        error*=1.-r*r;

    }

    done:

    /* slightly damp the filter */
    {
        double g = .99;
        double damp = g;
        for(j=0;j<m;j++){
            lpc[j]*=damp;
            damp*=g;
        }
    }

    return lpc;


/*

    // Find the autocorrelation of the signal, with pitch.
    ArrayXd corr = lpc_autocorr(x, pitch);

    if (*pitch == 0.0) {
        return ArrayXd::Ones(1);
    }

    // Construct the R matrix.
    MatrixXd R(order, order);

    for (int i = 1; i <= order; ++i) {
        for (int j = 1; j <= order; ++j) {
            R(i - 1, j - 1) = corr(std::abs(i - j));
        }
    }

    // Invert R.
    R = R.inverse().eval();

    // Find the coefficients A = P * R^(-1)
    ArrayXd coeffs = ArrayXd::Zero(order);

    for (unsigned i = 0; i < order; ++i) {
        for (unsigned j = 0; j < order; ++j) {
            coeffs(i) += R(i,j) * corr(j+1);
        }
    }

    return coeffs;
*/

}