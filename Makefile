# Makefile to replace pictures.sh, format.sh, odb.sh, and clean.sh

# Directories
SRC_DIR := ./src/persistence
RES_DIR := ./res
OUT_DIR := ./Default

# Files for odb
ODB_HEADERS := Game.h Bet.h Win.h GameConfiguration.h PlayHistory.h Session.h \
               MachineConfiguration.h Account.h DoorState.h BillStackerAccess.h \
               PowerState.h Event.h GameHistoryInfo.h

# Default target
all: clean format odb pictures

# Equivalent to pictures.sh
pictures:
	rm -f $(SRC_DIR)/*.ixx
	rm -f $(SRC_DIR)/*.cxx
	rm -f $(SRC_DIR)/*.hxx
	rm -f $(SRC_DIR)/*.sql
	rm -rf $(OUT_DIR)

# Equivalent to format.sh
format:
	astyle *.cpp *.h --indent=force-tab --style=java -A2 --recursive

# Equivalent to odb.sh
odb:
	cd $(SRC_DIR) && rm -f *.ixx *.cxx *.hxx *.sql
	cd $(SRC_DIR) && \
	odb --generate-schema-only --database pgsql --at-once --input-name database $(ODB_HEADERS)
	cd $(SRC_DIR) && \
	for hdr in $(ODB_HEADERS); do \
		odb --database pgsql --generate-query $$hdr; \
	done

# Equivalent to clean.sh
clean:
	mkdir -p $(OUT_DIR)
	cp -rf $(RES_DIR)/* $(OUT_DIR)/

.PHONY: all pictures format odb clean
