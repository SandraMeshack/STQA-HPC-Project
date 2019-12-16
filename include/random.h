#pragma once

#include <random>

/**
 * This class implements methods for simplifying the generation of random numbers following different distributions
 */
class Random {
public:
/**
 * Generate a random number using a exponential distribution with a average number corresponding to mean parameter
 * @param mean
 * @return
 */
    static double exponential(double mean) {
        std::random_device rd;
        std::mt19937 rnd_gen(rd());
        std::exponential_distribution<> rng(1 / mean);
        return rng(rnd_gen);
    }
/**
 * Generate a random number using a uniform law to generate a number between min and max parameters
 * @param min
 * @param max
 * @return
 */
    static double uniformDouble(double min, double max) {
        std::random_device rd;
        std::mt19937 rnd(rd());
        std::uniform_real_distribution<double> uni(min, max);
        return uni(rnd);
    }

/**
 * Generate a random number following a normal distribution.
 * 99.7% of the numbers should be in the interval [mean - 3 stddev; mean + 3stddev]
 * @param mean
 * @param stddev
 * @return
 */
    static double normalDouble(double mean, double stddev) {
        std::random_device rd;
        std::mt19937 rnd(rd());
        std::normal_distribution<double> norm(mean, stddev);
        return norm(rnd);
    }
/**
 * Generates a random integer following a binomial law of parmeters n and p
 * @param n
 * @param p
 * @return
 */
    static int binomialInt(int n, double p) {
        std::random_device rd;
        std::mt19937 rnd(rd());
        std::binomial_distribution<int> bin(n, 0.5);
        return bin(rnd);
    }
/**
 * Generate a random integer using a uniform law to generate a number between min and max parameters
 * @param min
 * @param max
 * @return
 */
    static double uniformInt(double min, double max) {
        std::random_device rd;
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased //
        return uni(rng);
    }

};