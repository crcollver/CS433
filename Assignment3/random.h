/**
 Following Random number generation functions are from Numerical Recipe book
 */
#ifndef _CS433_RANDOM_H
#define _CS433_RANDOM_H

#include <math.h>

class RandomNumGen {
public:
    /**
     * Constructor
     * @param _seed a negative integer to initialize the random number generator.
     */
    RandomNumGen(long _seed = -10) {
        if(_seed < 0) {
            seed = _seed;
        } else {
            seed = -_seed;
        }
    }
    /**
    * Return a random integer as the length of the next CPU burst
    * @param mean is the average CPU burst length, e.g. mean = 10
    */
    int CPUBurstRandom(int mean);

    /**
     *	Minimal random number generator of Park and Miller with Bays-Durham shuffle and added
    *	safeguards.
    *	@Return a uniform random deviate between 0.0 and 1.0 (exclusive of the endpoint values).
    *  Call with idum a negative integer to initialize; thereafter, do not alter idum between
    *  successive deviates in a sequence. RNMX should approximate the largest floating value that is
    *  less than 1.
    */
    float ran1();

    /**
    * Return a random integer uniformly from the range of [min, max]
     */
    int ranInt(int min, int max);

private:
    /**
     * Returns as a floating-point number an integer value that is a random deviate drawn from a
     * Poisson distribution of mean xm, using ran1(idum) as a source of uniform random deviates.
     */
    float poidev(float xm, long *idum);

    /**
     * Compute the gamma function
     */
    float gammln(float xx);

    /**
     * seed: a negative integer to initialize the random number generator;
     */
    long seed;
};


#endif

