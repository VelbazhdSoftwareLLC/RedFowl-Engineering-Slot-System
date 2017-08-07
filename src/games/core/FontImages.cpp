#include "FontImages.h"

namespace core {

SDL_Surface *FontImages::DIGIT_0;
SDL_Surface *FontImages::DIGIT_1;
SDL_Surface *FontImages::DIGIT_2;
SDL_Surface *FontImages::DIGIT_3;
SDL_Surface *FontImages::DIGIT_4;
SDL_Surface *FontImages::DIGIT_5;
SDL_Surface *FontImages::DIGIT_6;
SDL_Surface *FontImages::DIGIT_7;
SDL_Surface *FontImages::DIGIT_8;
SDL_Surface *FontImages::DIGIT_9;

SDL_Surface *FontImages::LETTER_A;
SDL_Surface *FontImages::LETTER_B;
SDL_Surface *FontImages::LETTER_C;
SDL_Surface *FontImages::LETTER_D;
SDL_Surface *FontImages::LETTER_E;
SDL_Surface *FontImages::LETTER_F;
SDL_Surface *FontImages::LETTER_G;
SDL_Surface *FontImages::LETTER_H;
SDL_Surface *FontImages::LETTER_I;
SDL_Surface *FontImages::LETTER_J;
SDL_Surface *FontImages::LETTER_K;
SDL_Surface *FontImages::LETTER_L;
SDL_Surface *FontImages::LETTER_M;
SDL_Surface *FontImages::LETTER_N;
SDL_Surface *FontImages::LETTER_O;
SDL_Surface *FontImages::LETTER_P;
SDL_Surface *FontImages::LETTER_Q;
SDL_Surface *FontImages::LETTER_R;
SDL_Surface *FontImages::LETTER_S;
SDL_Surface *FontImages::LETTER_T;
SDL_Surface *FontImages::LETTER_U;
SDL_Surface *FontImages::LETTER_V;
SDL_Surface *FontImages::LETTER_W;
SDL_Surface *FontImages::LETTER_X;
SDL_Surface *FontImages::LETTER_Y;
SDL_Surface *FontImages::LETTER_Z;

SDL_Surface* FontImages::DIGITS[FontImages::NUMBER_OF_DIGITS];
SDL_Surface* FontImages::SYMBOLS[FontImages::NUMBER_OF_ASCII_SYMBOLS];

const char FontImages::TEXT_SYMBOLS[10][10] = { { 0, '!', 0, 0, '$', 0, 0, 0,
		'(', ')' }, { '*', '+', ',', 0, '.', '/', '0', '1', '2', '3' }, { '4',
		'5', '6', '7', '8', '9', ':', ';', '<', '=' }, { '>', '?', 0, 'A', 'B',
		'C', 'D', 'E', 'F', 'G' }, { 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
		'P', 'Q' }, { 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[' }, {
		'\\', ']', 0, '_', 0, 'a', 'b', 'c', 'd', 'e' }, { 'f', 'g', 'h', 'i',
		'j', 'k', 'l', 'm', 'n', 'o' }, { 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y' }, { 'z', 0, '|', 0, 0, 0, 0, 0, 0, 0 }, };

static SDL_Surface *buffer;

SDL_Surface* FontImages::extractImage(SDL_Surface *image, SDL_Surface *buffer,
		int left, int top, int width, int height) {
	buffer = SDL_CreateRGBSurface(image->flags, width, height,
			image->format->BitsPerPixel, image->format->Rmask,
			image->format->Gmask, image->format->Bmask, image->format->Amask);

	if (image->format->BitsPerPixel == 32) {
		Uint32 value;
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				//TODO Optimize by memory block copy.
				value = *((Uint32*) (image->pixels) + left + i
						+ (image->w * (top + j)));
				*((Uint32*) (buffer->pixels) + i + width * j) = value;
			}
		}
	} else {
		//TODO Rise exception!
	}

	return (buffer);
}

void FontImages::load(const char *file) {
	SDL_Surface *font = IMG_Load(file);

	DIGIT_0 = extractImage(font, buffer, 6 * WIDTH, 1 * HEIGHT, WIDTH, HEIGHT);
	DIGIT_1 = extractImage(font, buffer, 7 * WIDTH, 1 * HEIGHT, WIDTH, HEIGHT);
	DIGIT_2 = extractImage(font, buffer, 8 * WIDTH, 1 * HEIGHT, WIDTH, HEIGHT);
	DIGIT_3 = extractImage(font, buffer, 9 * WIDTH, 1 * HEIGHT, WIDTH, HEIGHT);
	DIGIT_4 = extractImage(font, buffer, 0 * WIDTH, 2 * HEIGHT, WIDTH, HEIGHT);
	DIGIT_5 = extractImage(font, buffer, 1 * WIDTH, 2 * HEIGHT, WIDTH, HEIGHT);
	DIGIT_6 = extractImage(font, buffer, 2 * WIDTH, 2 * HEIGHT, WIDTH, HEIGHT);
	DIGIT_7 = extractImage(font, buffer, 3 * WIDTH, 2 * HEIGHT, WIDTH, HEIGHT);
	DIGIT_8 = extractImage(font, buffer, 4 * WIDTH, 2 * HEIGHT, WIDTH, HEIGHT);
	DIGIT_9 = extractImage(font, buffer, 5 * WIDTH, 2 * HEIGHT, WIDTH, HEIGHT);

	LETTER_A = extractImage(font, buffer, 3 * WIDTH, 3 * HEIGHT, WIDTH, HEIGHT);
	LETTER_B = extractImage(font, buffer, 4 * WIDTH, 3 * HEIGHT, WIDTH, HEIGHT);
	LETTER_C = extractImage(font, buffer, 5 * WIDTH, 3 * HEIGHT, WIDTH, HEIGHT);
	LETTER_D = extractImage(font, buffer, 6 * WIDTH, 3 * HEIGHT, WIDTH, HEIGHT);
	LETTER_E = extractImage(font, buffer, 7 * WIDTH, 3 * HEIGHT, WIDTH, HEIGHT);
	LETTER_F = extractImage(font, buffer, 8 * WIDTH, 3 * HEIGHT, WIDTH, HEIGHT);
	LETTER_G = extractImage(font, buffer, 9 * WIDTH, 3 * HEIGHT, WIDTH, HEIGHT);
	LETTER_H = extractImage(font, buffer, 0 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_I = extractImage(font, buffer, 1 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_J = extractImage(font, buffer, 2 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_K = extractImage(font, buffer, 3 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_L = extractImage(font, buffer, 4 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_M = extractImage(font, buffer, 5 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_N = extractImage(font, buffer, 6 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_O = extractImage(font, buffer, 7 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_P = extractImage(font, buffer, 8 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_Q = extractImage(font, buffer, 9 * WIDTH, 4 * HEIGHT, WIDTH, HEIGHT);
	LETTER_R = extractImage(font, buffer, 0 * WIDTH, 5 * HEIGHT, WIDTH, HEIGHT);
	LETTER_S = extractImage(font, buffer, 1 * WIDTH, 5 * HEIGHT, WIDTH, HEIGHT);
	LETTER_T = extractImage(font, buffer, 2 * WIDTH, 5 * HEIGHT, WIDTH, HEIGHT);
	LETTER_U = extractImage(font, buffer, 3 * WIDTH, 5 * HEIGHT, WIDTH, HEIGHT);
	LETTER_V = extractImage(font, buffer, 4 * WIDTH, 5 * HEIGHT, WIDTH, HEIGHT);
	LETTER_W = extractImage(font, buffer, 5 * WIDTH, 5 * HEIGHT, WIDTH, HEIGHT);
	LETTER_X = extractImage(font, buffer, 6 * WIDTH, 5 * HEIGHT, WIDTH, HEIGHT);
	LETTER_Y = extractImage(font, buffer, 7 * WIDTH, 5 * HEIGHT, WIDTH, HEIGHT);
	LETTER_Z = extractImage(font, buffer, 8 * WIDTH, 5 * HEIGHT, WIDTH, HEIGHT);

	DIGITS[0] = DIGIT_0;
	DIGITS[1] = DIGIT_1;
	DIGITS[2] = DIGIT_2;
	DIGITS[3] = DIGIT_3;
	DIGITS[4] = DIGIT_4;
	DIGITS[5] = DIGIT_5;
	DIGITS[6] = DIGIT_6;
	DIGITS[7] = DIGIT_7;
	DIGITS[8] = DIGIT_8;
	DIGITS[9] = DIGIT_9;

	SYMBOLS[0] = NULL;
	SYMBOLS[1] = NULL;
	SYMBOLS[2] = NULL;
	SYMBOLS[3] = NULL;
	SYMBOLS[4] = NULL;
	SYMBOLS[5] = NULL;
	SYMBOLS[6] = NULL;
	SYMBOLS[7] = NULL;
	SYMBOLS[8] = NULL;
	SYMBOLS[9] = NULL;
	SYMBOLS[10] = NULL;
	SYMBOLS[11] = NULL;
	SYMBOLS[12] = NULL;
	SYMBOLS[13] = NULL;
	SYMBOLS[14] = NULL;
	SYMBOLS[15] = NULL;
	SYMBOLS[16] = NULL;
	SYMBOLS[17] = NULL;
	SYMBOLS[18] = NULL;
	SYMBOLS[19] = NULL;
	SYMBOLS[20] = NULL;
	SYMBOLS[21] = NULL;
	SYMBOLS[22] = NULL;
	SYMBOLS[23] = NULL;
	SYMBOLS[24] = NULL;
	SYMBOLS[25] = NULL;
	SYMBOLS[26] = NULL;
	SYMBOLS[27] = NULL;
	SYMBOLS[28] = NULL;
	SYMBOLS[29] = NULL;
	SYMBOLS[30] = NULL;
	SYMBOLS[31] = NULL;
	SYMBOLS[32] = NULL;
	SYMBOLS[33] = NULL;
	SYMBOLS[34] = NULL;
	SYMBOLS[35] = NULL;
	SYMBOLS[36] = NULL;
	SYMBOLS[37] = NULL;
	SYMBOLS[38] = NULL;
	SYMBOLS[39] = NULL;
	SYMBOLS[40] = NULL;
	SYMBOLS[41] = NULL;
	SYMBOLS[42] = NULL;
	SYMBOLS[43] = NULL;
	SYMBOLS[44] = NULL;
	SYMBOLS[45] = NULL;
	SYMBOLS[46] = NULL;
	SYMBOLS[47] = NULL;
	SYMBOLS[48] = DIGIT_0;
	SYMBOLS[49] = DIGIT_1;
	SYMBOLS[50] = DIGIT_2;
	SYMBOLS[51] = DIGIT_3;
	SYMBOLS[52] = DIGIT_4;
	SYMBOLS[53] = DIGIT_5;
	SYMBOLS[54] = DIGIT_6;
	SYMBOLS[55] = DIGIT_7;
	SYMBOLS[56] = DIGIT_8;
	SYMBOLS[57] = DIGIT_9;
	SYMBOLS[58] = NULL;
	SYMBOLS[59] = NULL;
	SYMBOLS[60] = NULL;
	SYMBOLS[61] = NULL;
	SYMBOLS[62] = NULL;
	SYMBOLS[63] = NULL;
	SYMBOLS[64] = NULL;
	SYMBOLS[65] = LETTER_A;
	SYMBOLS[66] = LETTER_B;
	SYMBOLS[67] = LETTER_C;
	SYMBOLS[68] = LETTER_D;
	SYMBOLS[69] = LETTER_E;
	SYMBOLS[70] = LETTER_F;
	SYMBOLS[71] = LETTER_G;
	SYMBOLS[72] = LETTER_H;
	SYMBOLS[73] = LETTER_I;
	SYMBOLS[74] = LETTER_J;
	SYMBOLS[75] = LETTER_K;
	SYMBOLS[76] = LETTER_L;
	SYMBOLS[77] = LETTER_M;
	SYMBOLS[78] = LETTER_N;
	SYMBOLS[79] = LETTER_O;
	SYMBOLS[80] = LETTER_P;
	SYMBOLS[81] = LETTER_Q;
	SYMBOLS[82] = LETTER_R;
	SYMBOLS[83] = LETTER_S;
	SYMBOLS[84] = LETTER_T;
	SYMBOLS[85] = LETTER_U;
	SYMBOLS[86] = LETTER_V;
	SYMBOLS[87] = LETTER_W;
	SYMBOLS[88] = LETTER_X;
	SYMBOLS[89] = LETTER_Y;
	SYMBOLS[90] = LETTER_Z;
	SYMBOLS[91] = NULL;
	SYMBOLS[92] = NULL;
	SYMBOLS[93] = NULL;
	SYMBOLS[94] = NULL;
	SYMBOLS[95] = NULL;
	SYMBOLS[96] = NULL;
	SYMBOLS[97] = LETTER_A;
	SYMBOLS[98] = LETTER_B;
	SYMBOLS[99] = LETTER_C;
	SYMBOLS[100] = LETTER_D;
	SYMBOLS[101] = LETTER_E;
	SYMBOLS[102] = LETTER_F;
	SYMBOLS[103] = LETTER_G;
	SYMBOLS[104] = LETTER_H;
	SYMBOLS[105] = LETTER_I;
	SYMBOLS[106] = LETTER_J;
	SYMBOLS[107] = LETTER_K;
	SYMBOLS[108] = LETTER_L;
	SYMBOLS[109] = LETTER_M;
	SYMBOLS[110] = LETTER_N;
	SYMBOLS[111] = LETTER_O;
	SYMBOLS[112] = LETTER_P;
	SYMBOLS[113] = LETTER_Q;
	SYMBOLS[114] = LETTER_R;
	SYMBOLS[115] = LETTER_S;
	SYMBOLS[116] = LETTER_T;
	SYMBOLS[117] = LETTER_U;
	SYMBOLS[118] = LETTER_V;
	SYMBOLS[119] = LETTER_W;
	SYMBOLS[120] = LETTER_X;
	SYMBOLS[121] = LETTER_Y;
	SYMBOLS[122] = LETTER_Z;
	SYMBOLS[123] = NULL;
	SYMBOLS[124] = NULL;
	SYMBOLS[125] = NULL;
	SYMBOLS[126] = NULL;
	SYMBOLS[127] = NULL;
	SYMBOLS[128] = NULL;
	SYMBOLS[129] = NULL;
	SYMBOLS[130] = NULL;
	SYMBOLS[131] = NULL;
	SYMBOLS[132] = NULL;
	SYMBOLS[133] = NULL;
	SYMBOLS[134] = NULL;
	SYMBOLS[135] = NULL;
	SYMBOLS[136] = NULL;
	SYMBOLS[137] = NULL;
	SYMBOLS[138] = NULL;
	SYMBOLS[139] = NULL;
	SYMBOLS[140] = NULL;
	SYMBOLS[141] = NULL;
	SYMBOLS[142] = NULL;
	SYMBOLS[143] = NULL;
	SYMBOLS[144] = NULL;
	SYMBOLS[145] = NULL;
	SYMBOLS[146] = NULL;
	SYMBOLS[147] = NULL;
	SYMBOLS[148] = NULL;
	SYMBOLS[149] = NULL;
	SYMBOLS[150] = NULL;
	SYMBOLS[151] = NULL;
	SYMBOLS[152] = NULL;
	SYMBOLS[153] = NULL;
	SYMBOLS[154] = NULL;
	SYMBOLS[155] = NULL;
	SYMBOLS[156] = NULL;
	SYMBOLS[157] = NULL;
	SYMBOLS[158] = NULL;
	SYMBOLS[159] = NULL;
	SYMBOLS[160] = NULL;
	SYMBOLS[161] = NULL;
	SYMBOLS[162] = NULL;
	SYMBOLS[163] = NULL;
	SYMBOLS[164] = NULL;
	SYMBOLS[165] = NULL;
	SYMBOLS[166] = NULL;
	SYMBOLS[167] = NULL;
	SYMBOLS[168] = NULL;
	SYMBOLS[169] = NULL;
	SYMBOLS[170] = NULL;
	SYMBOLS[171] = NULL;
	SYMBOLS[172] = NULL;
	SYMBOLS[173] = NULL;
	SYMBOLS[174] = NULL;
	SYMBOLS[175] = NULL;
	SYMBOLS[176] = NULL;
	SYMBOLS[177] = NULL;
	SYMBOLS[178] = NULL;
	SYMBOLS[179] = NULL;
	SYMBOLS[180] = NULL;
	SYMBOLS[181] = NULL;
	SYMBOLS[182] = NULL;
	SYMBOLS[183] = NULL;
	SYMBOLS[184] = NULL;
	SYMBOLS[185] = NULL;
	SYMBOLS[186] = NULL;
	SYMBOLS[187] = NULL;
	SYMBOLS[188] = NULL;
	SYMBOLS[189] = NULL;
	SYMBOLS[190] = NULL;
	SYMBOLS[191] = NULL;
	SYMBOLS[192] = NULL;
	SYMBOLS[193] = NULL;
	SYMBOLS[194] = NULL;
	SYMBOLS[195] = NULL;
	SYMBOLS[196] = NULL;
	SYMBOLS[197] = NULL;
	SYMBOLS[198] = NULL;
	SYMBOLS[199] = NULL;
	SYMBOLS[200] = NULL;
	SYMBOLS[201] = NULL;
	SYMBOLS[202] = NULL;
	SYMBOLS[203] = NULL;
	SYMBOLS[204] = NULL;
	SYMBOLS[205] = NULL;
	SYMBOLS[206] = NULL;
	SYMBOLS[207] = NULL;
	SYMBOLS[208] = NULL;
	SYMBOLS[209] = NULL;
	SYMBOLS[210] = NULL;
	SYMBOLS[211] = NULL;
	SYMBOLS[212] = NULL;
	SYMBOLS[213] = NULL;
	SYMBOLS[214] = NULL;
	SYMBOLS[215] = NULL;
	SYMBOLS[216] = NULL;
	SYMBOLS[217] = NULL;
	SYMBOLS[218] = NULL;
	SYMBOLS[219] = NULL;
	SYMBOLS[220] = NULL;
	SYMBOLS[221] = NULL;
	SYMBOLS[222] = NULL;
	SYMBOLS[223] = NULL;
	SYMBOLS[224] = NULL;
	SYMBOLS[225] = NULL;
	SYMBOLS[226] = NULL;
	SYMBOLS[227] = NULL;
	SYMBOLS[228] = NULL;
	SYMBOLS[229] = NULL;
	SYMBOLS[230] = NULL;
	SYMBOLS[231] = NULL;
	SYMBOLS[232] = NULL;
	SYMBOLS[233] = NULL;
	SYMBOLS[234] = NULL;
	SYMBOLS[235] = NULL;
	SYMBOLS[236] = NULL;
	SYMBOLS[237] = NULL;
	SYMBOLS[238] = NULL;
	SYMBOLS[239] = NULL;
	SYMBOLS[240] = NULL;
	SYMBOLS[241] = NULL;
	SYMBOLS[242] = NULL;
	SYMBOLS[243] = NULL;
	SYMBOLS[244] = NULL;
	SYMBOLS[245] = NULL;
	SYMBOLS[246] = NULL;
	SYMBOLS[247] = NULL;
	SYMBOLS[248] = NULL;
	SYMBOLS[249] = NULL;
	SYMBOLS[250] = NULL;
	SYMBOLS[251] = NULL;
	SYMBOLS[252] = NULL;
	SYMBOLS[253] = NULL;
	SYMBOLS[254] = NULL;
	SYMBOLS[255] = NULL;
}

}
