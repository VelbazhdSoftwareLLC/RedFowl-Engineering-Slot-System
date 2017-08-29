#ifndef GAME_CORE_FONTIMAGES_H_
#define GAME_CORE_FONTIMAGES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace core {

#define FONT_SYMBOLS_LENGTH 100

/**
 *
 */
class FontImages {
public:
	/**
	 * Map ASCII symbols positions.
	 */
	static const char TEXT_SYMBOLS[10][10];

	/**
	 * Width of font symbol.
	 */
	static const int WIDTH = 32;

	/**
	 * Height of font symbol.
	 */
	static const int HEIGHT = 32;

	/**
	 * Number of pixels to overlap during text rendering.
	 */
	static const int OVERLAP = 15;

	/**
	 * Digit 0 image.
	 */
	static SDL_Surface *DIGIT_0;

	/**
	 * Digit 1 image.
	 */
	static SDL_Surface *DIGIT_1;

	/**
	 * Digit 2 image.
	 */
	static SDL_Surface *DIGIT_2;

	/**
	 * Digit 3 image.
	 */
	static SDL_Surface *DIGIT_3;

	/**
	 * Digit 4 image.
	 */
	static SDL_Surface *DIGIT_4;

	/**
	 * Digit 5 image.
	 */
	static SDL_Surface *DIGIT_5;

	/**
	 * Digit 6 image.
	 */
	static SDL_Surface *DIGIT_6;

	/**
	 * Digit 7 image.
	 */
	static SDL_Surface *DIGIT_7;

	/**
	 * Digit 8 image.
	 */
	static SDL_Surface *DIGIT_8;

	/**
	 * Digit 9 image.
	 */
	static SDL_Surface *DIGIT_9;

	/**
	 * Total number of used digits.
	 */
	static const int NUMBER_OF_DIGITS = 10;

	/**
	 * All digits images in a signle array.
	 */
	static SDL_Surface* DIGITS[NUMBER_OF_DIGITS];

	/**
	 * Letter A image.
	 */
	static SDL_Surface *LETTER_A;

	/**
	 * Letter B image.
	 */
	static SDL_Surface *LETTER_B;

	/**
	 * Letter C image.
	 */
	static SDL_Surface *LETTER_C;

	/**
	 * Letter D image.
	 */
	static SDL_Surface *LETTER_D;

	/**
	 * Letter E image.
	 */
	static SDL_Surface *LETTER_E;

	/**
	 * Letter F image.
	 */
	static SDL_Surface *LETTER_F;

	/**
	 * Letter G image.
	 */
	static SDL_Surface *LETTER_G;

	/**
	 * Letter H image.
	 */
	static SDL_Surface *LETTER_H;

	/**
	 * Letter I image.
	 */
	static SDL_Surface *LETTER_I;

	/**
	 * Letter J image.
	 */
	static SDL_Surface *LETTER_J;

	/**
	 * Letter K image.
	 */
	static SDL_Surface *LETTER_K;

	/**
	 * Letter L image.
	 */
	static SDL_Surface *LETTER_L;

	/**
	 * Letter M image.
	 */
	static SDL_Surface *LETTER_M;

	/**
	 * Letter N image.
	 */
	static SDL_Surface *LETTER_N;

	/**
	 * Letter O image.
	 */
	static SDL_Surface *LETTER_O;

	/**
	 * Letter P image.
	 */
	static SDL_Surface *LETTER_P;

	/**
	 * Letter Q image.
	 */
	static SDL_Surface *LETTER_Q;

	/**
	 * Letter R image.
	 */
	static SDL_Surface *LETTER_R;

	/**
	 * Letter S image.
	 */
	static SDL_Surface *LETTER_S;

	/**
	 * Letter T image.
	 */
	static SDL_Surface *LETTER_T;

	/**
	 * Letter U image.
	 */
	static SDL_Surface *LETTER_U;

	/**
	 * Letter V image.
	 */
	static SDL_Surface *LETTER_V;

	/**
	 * Letter W image.
	 */
	static SDL_Surface *LETTER_W;

	/**
	 * Letter X image.
	 */
	static SDL_Surface *LETTER_X;

	/**
	 * Letter Y image.
	 */
	static SDL_Surface *LETTER_Y;

	/**
	 * Letter Z image.
	 */
	static SDL_Surface *LETTER_Z;

	/**
	 * Total number of ASCII symbols.
	 */
	static const int NUMBER_OF_ASCII_SYMBOLS = 256;

	/**
	 * All available font symbols images at ASCII positions.
	 */
	static SDL_Surface* SYMBOLS[NUMBER_OF_ASCII_SYMBOLS];

public:

	static SDL_Surface* extractImage(SDL_Surface *fontImage,
			SDL_Surface *buffer, int left, int top, int width, int height);

	static void load(const char *file);
};

}

#endif
