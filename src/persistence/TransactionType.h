#ifndef PERSISTENCE_TRANSACTIONTYPE_H_
#define PERSISTENCE_TRANSACTIONTYPE_H_

namespace persistence {

/**
 * Debit is for money in to the machine. Credit is for money out of the machine.
 */
enum TransactionType {
	ATTENDANT_DEBIT = 1, ATTENDANT_CREDIT = 2,
};

}

#endif
