#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP
#include <QtMath>
#include <vector>

class MathUtil
{
public:
    template<class T> static inline void divideLineIntoSegmentsAndGaps(T lineSize, unsigned int segmentAmount, T segmentGapRatio, T &resultingSegmentSize, T &resultingGapSize);
    template<class T> static inline T *hanningMultipliers(unsigned int dataSize);
    template<class T> static inline T *hanningMultipliersMatlab(int N, short itype = 0);
    template<class T> static inline T calculateMagnitude(T real, T imaginary);
    template<class T> static inline T calculateMagnitudeDb(T real, T imaginary);
    template<class T> static inline T calculateMagnitudeDb(T magnitude);
    template<class T> static inline T clamp(T x, T a, T b);
};

template <class T> inline T MathUtil::clamp(T x, T a, T b){    return x < a ? a : (x > b ? b : x);}

template <class T> inline T MathUtil::calculateMagnitudeDb(T magnitude)
{
    return log10(magnitude)*20;
}


template <> inline double MathUtil::calculateMagnitudeDb(double real, double imaginary)
{
    return 10*log10(real*real + imaginary*imaginary);
}

template <> inline float MathUtil::calculateMagnitudeDb(float real, float imaginary)
{
    return 10*log10f(real*real + imaginary*imaginary);
}

template <> inline long double MathUtil::calculateMagnitudeDb(long double real, long double imaginary)
{
    return 10*log10l(real*real + imaginary*imaginary);
}


template <class T> inline T MathUtil::calculateMagnitude(T real, T imaginary)
{
    return std::sqrt(real*real + imaginary*imaginary);
}

template <class T> inline void MathUtil::divideLineIntoSegmentsAndGaps(T lineSize, unsigned int segmentAmount, T segmentGapRatio, T &resultingSegmentSize, T &resultingGapSize){
    T totalPartsSize = lineSize * segmentGapRatio;
    T totalGapsSize = lineSize - totalPartsSize;

    resultingSegmentSize = totalPartsSize / segmentAmount;
    resultingGapSize = totalGapsSize / (segmentAmount - 1);
}

template <class T> inline T * MathUtil::hanningMultipliers(unsigned int dataSize){
    T *multipliers;

    multipliers = new T[dataSize];
    for (unsigned int i = 0; i < dataSize; i++) {
        T multiplier = 0.5 * (T(1) - cos(T(2)*M_PI*i/(dataSize-1)));
        multipliers[i] = multiplier;
    }
    return multipliers;
}

/*  function w = hanning(varargin)
%   HANNING   Hanning window.
%   HANNING(N) returns the N-point symmetric Hanning window in a column
%   vector.  Note that the first and last zero-weighted window samples
%   are not included.
%
%   HANNING(N,'symmetric') returns the same result as HANNING(N).
%
%   HANNING(N,'periodic') returns the N-point periodic Hanning window,
%   and includes the first zero-weighted window sample.
%
%   NOTE: Use the HANN function to get a Hanning window which has the
%          first and last zero-weighted samples.ep
    itype = 1 --> periodic
    itype = 0 --> symmetric
    default itype=0 (symmetric)

    Copyright 1988-2004 The MathWorks, Inc.
%   $Revision: 1.11.4.3 $  $Date: 2007/12/14 15:05:04 $
*/
template <class T> inline T * MathUtil::hanningMultipliersMatlab(int N, short itype){
    int half, i, idx, n;
    T *w;

    w = new T[N];
    std::fill(w, w+N, T(0));

    if(itype==1)    //periodic function
        n = N-1;
    else
        n = N;

    if(n%2==0)
    {
        half = n/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (T(1) - cos(T(2)*M_PI*(i+1) / (n+1)));

        idx = half-1;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }
    else
    {
        half = (n+1)/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (T(1) - cos(T(2)*M_PI*(i+1) / (n+1)));

        idx = half-2;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }

    if(itype==1)    //periodic function
    {
        for(i=N-1; i>=1; i--)
            w[i] = w[i-1];
        w[0] = 0.0;
    }
    return w;
}

#endif // MATHUTIL_HPP
