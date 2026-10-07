#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14
#endif

/*
 * Convert rand()'s integer result to a decimal in [0, 1].
 * rand() can return RAND_MAX, so the value 1 is possible.
 */
double rand_double() {
    return (double)rand() / (double)RAND_MAX;
}

/*
 * Estimate how often a sampled point is closer to the center than to
 * the polygon's side. The polygon is centered at the origin and has a
 * side on the line x = 1 (so its apothem is 1).
 *
 * The polygon is symmetric, so this samples only the wedge around that
 * side's midpoint: angles from -pi/n to pi/n. For each angle, r_max is
 * where the ray meets the side, found from r*cos(theta) = 1.
 *
 * The radius is chosen with sqrt(u), which makes area uniform along each
 * individual ray. Since angles are chosen uniformly even though the rays
 * have different lengths, the points are not uniform over the polygon's
 * full area; the result estimates the probability for this sampling rule.
 */
double approx_probability(long num_sides, long num_trials) {
    long successful_trials = 0;
    double max_theta = M_PI / num_sides;

    for (long i = 0; i < num_trials; i++) {
        // Choose a direction uniformly within the symmetric wedge.
        double theta = -max_theta + (2.0 * max_theta * rand_double());

        // Find the edge of the polygon along this ray, then choose a radius.
        // sqrt(u) gives uniform area along this ray, from the center to edge.
        double u = rand_double();
        double r_max = 1.0 / cos(theta);
        double r = r_max * sqrt(u);

        // Convert the sampled point from polar coordinates to (x, y).
        double x = r * cos(theta);
        double y = r * sin(theta);

        // PC is the distance to the center (the origin).
        double PC = sqrt(x*x + y*y);
        // PQ is the perpendicular distance to the side x = 1.
        double PQ = 1.0 - x;

        // Count points closer to the center than to this side.
        if (PC < PQ) {
            successful_trials++;
        }
    }

    // The fraction of successes estimates the probability for this sampler.
    return (double)successful_trials / (double)num_trials;
}

int main() {
    // Use the current time as the seed so runs usually use different sequences.
    srand((unsigned int)time(NULL));
    
    // More trials generally make a Monte Carlo estimate less variable.
    long num_trials = 10000000;

    printf("Estimating Monte Carlo probabilities (Number of Trials: %ld):\n", num_trials);
    printf("--------------------------------------------------\n");
    printf("(a) For n = 4 (Square):         Probability = %.5f\n", approx_probability(4, num_trials));
    printf("(b) For n = 3 (Triangle):       Probability = %.5f\n", approx_probability(3, num_trials));
    printf("(c) For n = 1,000,000 (Circle): Probability = %.5f\n", approx_probability(1000000, num_trials));

    return 0;
}
