#ifndef RANDOMGENERATOR_HPP
#define RANDOMGENERATOR_HPP

#include <random>
#include <ctime>

/* Random int, float, double generator from TIME seed */

class RandomGenerator
{ 
private:

    std::default_random_engine randEngine;
	
public:

    RandomGenerator()
    {
        randEngine.seed(time(0));
    }

    ~RandomGenerator()
    {
    }

    int getRandInt(int min, int max)
	/* PRE: true */
	/* POST: returns a random int between 'min' and 'max' */
    {
        std::uniform_int_distribution<int> intBetween(min, max);

        return intBetween(randEngine);
    }

    float getRandFloat(float min, float max)
	/* PRE: true */
	/* POST: returns a random float between 'min' and 'max' */
    {
        std::uniform_real_distribution<float> floatBetween(min, max);

        return floatBetween(randEngine);
    }

    double getRandDouble(double min, double max)
	/* PRE: true */
	/* POST: returns a random double between 'min' and 'max' */
    {
        std::uniform_real_distribution<double> doubleBetween(min, max);

        return doubleBetween(randEngine);
    }
};

#endif