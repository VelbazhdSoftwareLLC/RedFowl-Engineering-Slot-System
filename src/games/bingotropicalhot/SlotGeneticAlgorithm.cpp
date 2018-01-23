#include <cmath>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "../core/CommonState.h"

#include "SlotState.h"
#include "SlotDefinition.h"
#include "SlotStatistics.h"

#include "SlotGeneticAlgorithm.h"

namespace bingotropicalhot {

/**
 * Sum of all values into paytable.
 */
int paytableSum;

/**
 * Optimal target percent.
 */
double optimalPercent = 0.0;

/**
 * Sum of the base game symbols wins. It is used for probability distribution.
 */
int baseSymbolsWinSum[SYMBOLS_LENGTH];

/**
 * Sum of the free games symbols wins. It is used for probability distribution.
 */
int freeSymbolsWinSum[SYMBOLS_LENGTH];

/**
 * Single chromosome size.
 * Base game reals + free games reels + free games multiplier distribution +
 * base game scatters distribution + free games scatters distribution.
 */
const int CHROMOSOME_LENGHT =
	REELS_LENGTH
	* REEL_LENGTH /*+ REELS_LENGHT * REEL_LENGHT + FREE_MULTIPLIER_DISTRIBUTION_LENGHT + NUMBER_OF_SCATTERS * BASE_SCATTER_DISTRIBUTION_LENGHT + NUMBER_OF_SCATTERS * FREE_SCATTER_DISTRIBUTION_LENGHT*/;

/**
 * Genetic algorithm population size.
 */
double fitness[ GA_POPULATION_SIZE];

/**
 * Genetic algorithm population of chromosomes.
 */
int population[ GA_POPULATION_SIZE][CHROMOSOME_LENGHT];

/**
 * Population index to first parent chromosome.
 */
int firstParentIndex;

/**
 * Population index to second parent chromosome.
 */
int secondParentIndex;

/**
 * Population index to child chromosome.
 */
int childIndex;

/**
 * Population index to best fintess chromosome.
 */
int bestFitnessIndex;

/**
 * Population index to worst fintess chromosome.
 */
int worstFitnessIndex;

/**
 * Do symbol balance in order to destroy groups of equal symbols.
 *
 * @param array Reels.
 */
void symbolsBalance(int chromozome[]) {
	/*
	 * Problems are possible for example if there is only one symbol in the reel.
	 */
	static const int TRYS_LIMIT = REEL_LENGTH * REEL_LENGTH;

	static int *array[REELS_LENGTH];

	/*
	 * Map chromozome as reels array.
	 */
	for (int i = 0; i < REELS_LENGTH; i++) {
		array[i] = chromozome + (REEL_LENGTH * i);
	}

	int swap;
	for (int i = 0, a, b, c, t; i < REELS_LENGTH; i++) {
		bool done = true;

		for (int j = 0, k = 1, l = 2; j < REEL_LENGTH;
				j++, k = (k + 1) % REEL_LENGTH, l = (l + 1) % REEL_LENGTH) {
			if ((array[i][j] == array[i][k] || array[i][j] == array[i][l])) {
				t = 0;
				done = false;
				do {
					if (t >= TRYS_LIMIT) {
						break;
					}

					a = rand() % REEL_LENGTH;
					b = (a + 1) % REEL_LENGTH;
					c = (a + 2) % REEL_LENGTH;
					t++;
				} while (array[i][j] == array[i][a]
						 || array[i][j] == array[i][b]
						 || array[i][j] == array[i][c]);

				swap = array[i][j];
				array[i][j] = array[i][a];
				array[i][a] = swap;
			}
		}

		if (done == false) {
			i--;
		}
	}
}

/**
 * Store current used values in signle chromosome.
 *
 * @param chromosome Chromosome pointer.
 */
void storeCurrentValues(int chromosome[]) {
	static int *current = NULL;
	current = chromosome;

	/*
	 * Base game reels values.
	 */
	for (int i = 0; i < REELS_LENGTH; i++) {
		for (int j = 0; j < REEL_LENGTH; j++) {
			core::CommonState::baseReels[i][j] = current[j];
		}
		current += REEL_LENGTH;
	}

	//TODO It is done in order to optimize only base game reels.
	return;
}

/**
 * Load current used values in signle chromosome.
 *
 * @param chromosome Chromosome pointer.
 */
void loadCurrentValues(int chromosome[]) {
	static int *current = NULL;
	current = chromosome;

	/*
	 * Base game reels values.
	 */
	for (int i = 0; i < REELS_LENGTH; i++) {
		for (int j = 0; j < REEL_LENGTH; j++) {
			current[j] = core::CommonState::baseReels[i][j];
		}
		current += REEL_LENGTH;
	}

	//TODO Handle the rest of the chromosome.
	return;
}

/**
 * Check for valid chromosome.
 *
 * @param chromosome Chromosome pointer.
 *
 * @return True if it is valid, false otherwise.
 */
bool isValidChromosome(int chromosome[]) {
	static int *current = NULL;
	current = chromosome;

	//TODO It is done in order to optimize only base game reels.
	return (true);
}

/**
 * Create random chromosome according game design rules.
 *
 * @param chromosome Chromosome pointer.
 */
void createRandomChromosome(int chromosome[]) {
	static const int BASE_SYMBOLS_DISTRIBUTION[] = { 1, 3, 4, 4, 4, 4, 4, 4, 4,
													 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
													 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
													 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10,
													 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11,
													 11, 11, 11, 11, 11, 11, 11, 15, 15, 15, 15, 16, 16, 16, 16
												   };

#ifdef RARE_RANDOM_CHROMOSOME
	if( rand()%(POPULATION_SIZE*NUMBER_OF_EPOCHS) ) {
		return;
	}
#endif

	do {
		/*
		 * Base game and free games reels values.
		 */
		static int *current = NULL;
		current = chromosome;
		for (int i = 0; i < REELS_LENGTH; i++) {
			for (int j = 0; j < REEL_LENGTH; j++) {
				(*current) = BASE_SYMBOLS_DISTRIBUTION[rand() % 109];
				current++;
			}
		}

		//TODO Generate the rest of the chromosome.

	} while (isValidChromosome(chromosome) == false);
}

/**
 * Shuffle chromosome values.
 */
void shffleChromosome(int chromosome[]) {
	static int *current = NULL;
	current = chromosome;

	/*
	 * Base game reels values.
	 */
	for (int i = 0; i < REELS_LENGTH; i++) {
		for (int last = REEL_LENGTH - 1, r = -1, swap = -1; last > 0; last--) {
			r = rand() % (last + 1);
			swap = current[last];
			current[last] = current[r];
			current[r] = swap;
		}

		current += REEL_LENGTH;
	}

	//TODO It is done in order to optimize only base game reels.
	return;

	/*
	 * Free games reels values.
	 */
	for (int i = 0; i < REELS_LENGTH; i++) {
		for (int last = REEL_LENGTH - 1, r = -1, swap = -1; last > 0; last--) {
			r = rand() % (last + 1);
			swap = current[last];
			current[last] = current[r];
			current[r] = swap;
		}

		current += REEL_LENGTH;
	}

	//TODO Handle the rest of the chromosome.
}

/**
 * Create random population.
 */
void createRandomPopulation() {
	for (int p = 0; p < GA_POPULATION_SIZE; p++) {
		loadCurrentValues(population[p]);
#ifdef USE_CHROMOSOME_SHUFFLING
		shffleChromosome(population[p]);
#endif
#ifdef USE_RANDOM_CHROMOSOME_GENERATION
		createRandomChromosome( population[p] );
#endif
		fitness[p] = rand();
	}
}

/**
 * Loop over population and evaluate fitness values.
 *
 * @param chromosome Chromosome pointer.
 */
double evaluate(int chromosome[]) {
	/*
	 * Clear invalid chromosome.
	 */
	while (isValidChromosome(chromosome) == false) {
		loadCurrentValues(chromosome);
#ifdef USE_CHROMOSOME_SHUFFLING
		shffleChromosome(chromosome);
#endif
#ifdef USE_RANDOM_CHROMOSOME_GENERATION
		createRandomChromosome( chromosome );
#endif
	}

	storeCurrentValues(chromosome);

	double average = 0.0;
	for (unsigned long e = 0; e < NUMBER_OF_EXPERIMENTS; e++) {
		experiment->wonMoney = 0L;
		experiment->lostMoney = 0L;

		runSimulation();

		average += (double) experiment->wonMoney
				   / (double) experiment->lostMoney * 100;
	}
	average /= NUMBER_OF_EXPERIMENTS;

	return (average);
}

/**
 * Loop over population and renew chromosomes with equal fitness.
 */
void renewPopulation() {
	for (int p = 0; p < GA_POPULATION_SIZE; p++) {
		for (int q = p + 1; q < GA_POPULATION_SIZE; q++) {
			/*
			 * One way to select equal chromosomes is to compare fitness values.
			 */
			bool doIt = fitness[p] == fitness[q];

#ifdef CHECK_CHROMOSOMES_PERFECT_MATCH
			/*
			 * Another way to select equal chromosomes is to check for perfect match.
			 */
			doIt = true;
			for (int i = 0; i < CHROMOSOME_LENGHT; i++) {
				if (population[p][i] != population[q][i]) {
					doIt = false;
					break;
				}
			}
#endif

			if (doIt == true) {
				loadCurrentValues(population[q]);
#ifdef USE_CHROMOSOME_SHUFFLING
				shffleChromosome(population[q]);
#endif
#ifdef USE_RANDOM_CHROMOSOME_GENERATION
				createRandomChromosome(population[q]);
#endif
			}
		}
	}
}

/**
 * Select indexes to take part of crossover.
 */
void selectIndexes() {
	static const int CROSSOVER_RESULT_INTO_BEST_PERCENT = 1;
	static const int CROSSOVER_RESULT_INTO_MIDDLE_PERCENT = 9;
	static const int CROSSOVER_RESULT_INTO_WORST_PERCENT = 90;

	static int percent = -1;
	percent = rand()
			  % (CROSSOVER_RESULT_INTO_WORST_PERCENT
				 + CROSSOVER_RESULT_INTO_MIDDLE_PERCENT
				 + CROSSOVER_RESULT_INTO_BEST_PERCENT);

	if (percent < CROSSOVER_RESULT_INTO_WORST_PERCENT) {
		do {
			childIndex = rand() % GA_POPULATION_SIZE;
			firstParentIndex = rand() % GA_POPULATION_SIZE;
			secondParentIndex = rand() % GA_POPULATION_SIZE;
		} while (fitness[childIndex] < fitness[firstParentIndex]
				 || fitness[childIndex] < fitness[secondParentIndex]);
	} else if (percent
			   < (CROSSOVER_RESULT_INTO_WORST_PERCENT
				  + CROSSOVER_RESULT_INTO_MIDDLE_PERCENT)) {
		if (fitness[secondParentIndex] < fitness[firstParentIndex]) {
			int index = secondParentIndex;
			secondParentIndex = firstParentIndex;
			firstParentIndex = index;
		}
		do {
			childIndex = rand() % GA_POPULATION_SIZE;
			firstParentIndex = rand() % GA_POPULATION_SIZE;
			secondParentIndex = rand() % GA_POPULATION_SIZE;
		} while (fitness[childIndex] < fitness[firstParentIndex]
				 || fitness[childIndex] > fitness[secondParentIndex]);
	} else if (percent
			   < (CROSSOVER_RESULT_INTO_WORST_PERCENT
				  + CROSSOVER_RESULT_INTO_MIDDLE_PERCENT
				  + CROSSOVER_RESULT_INTO_BEST_PERCENT)) {
		do {
			childIndex = rand() % GA_POPULATION_SIZE;
			firstParentIndex = rand() % GA_POPULATION_SIZE;
			secondParentIndex = rand() % GA_POPULATION_SIZE;
		} while (fitness[childIndex] > fitness[firstParentIndex]
				 || fitness[childIndex] > fitness[secondParentIndex]);
	}

#ifdef KEEP_ELITE
	if (childIndex == bestFitnessIndex) {
		childIndex = worstFitnessIndex;
	}
#endif
}

/**
 * Corssover single chromosome.
 *
 * @param child Result chromosome pointer.
 *
 * @param first First parent chromosome pointer.
 *
 * @param second Second parent chromosome pointer.
 */
void crossover(int child[], int first[], int second[]) {
	static int cut = -1;

	cut = rand() % (CHROMOSOME_LENGHT + 1);

	memcpy(child, first, sizeof(int) * cut);
	memcpy(child + cut, second, sizeof(int) * (CHROMOSOME_LENGHT - cut));
}

/**
 * Mutate single chromosome.
 *
 * @param chromosome Chromosome pointer.
 */
void mutate(int chromosome[]) {
	static int position = -1;
	static int takeFromIndex = -1;

	position = rand() % (CHROMOSOME_LENGHT + 1);

	takeFromIndex = rand() % GA_POPULATION_SIZE;

	chromosome[position] = population[takeFromIndex][position];
}

/**
 * Keep indexes of best and worst chromosomes.
 *
 * @param index Index of the chromosome to be cheked on.
 */
void keepTrackOfBestAndWorst(int index) {
	if (fitness[index] < fitness[bestFitnessIndex]) {
		bestFitnessIndex = index;
	}
	if (fitness[index] > fitness[worstFitnessIndex]) {
		worstFitnessIndex = index;
	}
}

/**
 * Run single optimization cycle.
 */
void runOptimization() {
	core::CommonState::baseReels.resize(REELS_LENGTH);
	for (int i = 0; i < REELS_LENGTH; i++) {
		core::CommonState::baseReels[i].resize(REEL_LENGTH);
		for (int j = 0; j < REEL_LENGTH; j++) {
			core::CommonState::baseReels[i][j] = baseGameReels[core::CommonState::rtp - 90][i][j];
		}
	}

	createRandomPopulation();

	for (long e = 0; e < GA_NUMBER_OF_EPOCHS; e++) {
		static int lastPrintTime = clock() - 11;
#ifdef USE_POPULATION_RENEW
		renewPopulation();
#endif

		for (int p = 0; p < GA_POPULATION_SIZE; p++) {
			if (clock() - lastPrintTime > 10) {
				cerr
						<< (int) ((double) (e * GA_POPULATION_SIZE + p) * 10000
								  / (double) (GA_NUMBER_OF_EPOCHS
											  * GA_POPULATION_SIZE)) / 100.0 << "%"
						<< endl;
				lastPrintTime = clock();
			}

			selectIndexes();
#ifdef EVALUATE_IN_EACH_GENERATION
			while (childIndex == firstParentIndex
					|| childIndex == secondParentIndex) {
				selectIndexes();
			}
#endif

			crossover(population[childIndex], population[firstParentIndex],
					  population[secondParentIndex]);

			mutate(population[childIndex]);

#ifdef CHROMOSOME_SYMBOLS_BALANCE
			symbolsBalance(population[childIndex]);
#endif

			fitness[childIndex] = abs(
									  optimalPercent - evaluate(population[childIndex]));
			keepTrackOfBestAndWorst(childIndex);
#ifdef EVALUATE_IN_EACH_GENERATION
			fitness[firstParentIndex] = abs(
											optimalPercent - evaluate(population[firstParentIndex]));
			keepTrackOfBestAndWorst(firstParentIndex);
			fitness[secondParentIndex] = abs(
											 optimalPercent - evaluate(population[secondParentIndex]));
			keepTrackOfBestAndWorst(secondParentIndex);
#endif
		}

		cerr << "Epoch: " << e << endl;
		printOptimalResults(cerr);
	}
}

/**
 * Print best found solution.
 *
 * @param out Output stream.
 */
void printOptimalResults(ostream &out) {
	int *current = population[bestFitnessIndex];

	/*
	 * Base game reels values.
	 */
	out << "\t";
	out << "/*Base game \tTarget percent: " << optimalPercent
		<< "%\tFitness value: " << fitness[bestFitnessIndex] << "%*/{";
	out << endl;
	for (int i = 0; i < REELS_LENGTH; i++) {
		out << "\t\t";
		out << "{";
		for (int j = 0; j < REEL_LENGTH; j++) {
			out << (*current) << ", ";
			current++;
		}
		out << "}," << endl;
	}
	out << "\t";
	out << "},";
	out << endl;

	//TODO It is done in order to optimize only base game reels.
	return;
}

}
