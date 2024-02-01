SRC_DIR := ./src/persistence/
DEFAULT_DIR := ./Default/

ODB := odb --output-dir $(SRC_DIR)

#ODB_FILES := $(shell find $(SRC_DIR) -name '*.h')

SCHEMA_FILES=$(addprefix $(SRC_DIR), Game.h Bet.h Win.h GameConfiguration.h PlayHistory.h Session.h MachineConfiguration.h Account.h DoorState.h BillStackerAccess.h PowerState.h Event.h)
DB_FILES=$(SCHEMA_FILES) $(addprefix $(SRC_DIR), GameHistoryInfo.h)

odb: clean_files _odb1 _odb2
.PHONY: odb

_odb1: $(SCHEMA_FILES)
	$(ODB) --generate-schema-only --database pgsql --at-once --input-name database  $^

_odb2: $(DB_FILES)
	$(foreach file, $^, $(ODB) --database pgsql --generate-query $(file);)

pictures:
	@mkdir -p $(DEFAULT_DIR)
	cp -rf ./res/* $(DEFAULT_DIR)

format:
	astyle *.cpp *.h --indent=force-tab --style=java / -A2 --recursive

clean: clean_files _clean_default_dir
.PHONY: clean

clean_files:
	$(RM) $(SRC_DIR)/*.ixx
	$(RM) $(SRC_DIR)/*.cxx
	$(RM) $(SRC_DIR)/*.hxx
	$(RM) $(SRC_DIR)/*.sql

_clean_default_dir:
	$(RM) -rf $(DEFAULT_DIR)

.PHONY: help
help:
	@echo  'Targets:'
	@echo  'odb'
	@echo  'format'
	@echo  'pictures'
	@echo  'clean'
