#ifndef GAME_CORE_COMMON_H_
#define GAME_CORE_COMMON_H_

namespace core {

/*
 * Switch simulation and optimization mode on/off.
 * Determine which RTP target percent to be optimized.
 */
// #define SIMULATION true
// #define OPTIMIZATION true
#define PERCENT_TO_OPTIMIZE 90

/*
 * Mark symbols with labels and numbers.
 */
// #define WRITE_ON_SYMBOLS_INFO true
// #define USE_TTF true
/*
 * Number of experiment simulations to be done.
 */
#define NUMBER_OF_EXPERIMENTS 10

/*
 * Total number of games to be played during single simulation.
 */
#define NUMBER_OF_SIMULATIONS 1000000L

/*
 * Control over genetic algorithm optimization.
 * Flag for pure random chromosome generation.
 * In rare random chromosomes process is more stable.
 * Keep the best chromosome in order to keep best found solution.
 * Population renew can be very rare and it is not always needed.
 * Check for perfect match between chromosomes for better chromosome renew strategy.
 * Chromosome symbol balance is uset to have nice distribution of symbols on the reels.
 * Evaluation of chromosomes in each generation gives better fitness for each chromosome, but more simulations are done on identical reels.
 * Genetic algorithm population size.
 * How many times to run genetic algorithm.
 */
// #define USE_RANDOM_CHROMOSOME_GENERATION true
// #define RARE_RANDOM_CHROMOSOME true
#define KEEP_ELITE true
#define USE_POPULATION_RENEW true
#define CHECK_CHROMOSOMES_PERFECT_MATCH true
#define USE_CHROMOSOME_SHUFFLING true
#define CHROMOSOME_SYMBOLS_BALANCE true
#define EVALUATE_IN_EACH_GENERATION true
#define GA_POPULATION_SIZE 17
#define GA_NUMBER_OF_EPOCHS 213

}

#endif
