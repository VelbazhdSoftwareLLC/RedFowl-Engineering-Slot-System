#ifndef PERSISTENCE_DOORTYPE_H_
#define PERSISTENCE_DOORTYPE_H_

namespace persistence {

enum DoorType {
	MAIN = 1,
	LOGIC_BOARD = 2,
	DROP = 4,
	BILL_STACKER_BOX = 8,
	BALLEY = 16,
	TOP_BOX = 32,
	BILL_VALIDATOR = 64,
	EXT = 128
};

}

#endif
