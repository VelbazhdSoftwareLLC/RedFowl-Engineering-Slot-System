#ifndef GAMES_CORE_CALCULATIONS_H_
#define GAMES_CORE_CALCULATIONS_H_

#include <vector>

namespace core {

extern void generatDecorative(const std::vector<std::vector<int> > &reels);
extern void spin(std::vector<std::vector<int> > &view,
		const std::vector<std::vector<int> > &reels,
		const std::vector<int> &reelsMinOffset,
		const std::vector<int> &reelsMaxOffset);

}

#endif
