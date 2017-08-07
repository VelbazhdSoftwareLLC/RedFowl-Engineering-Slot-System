#!/bin/bash
#TODO Add as make script commands.
rm *.ixx
rm *.cxx
rm *.hxx
rm *.sql
odb --generate-schema-only --database pgsql --at-once --input-name database Game.h Bet.h Win.h GameConfiguration.h PlayHistory.h Session.h MachineConfiguration.h Account.h DoorState.h BillStackerAccess.h PowerState.h Event.h
odb --database pgsql --generate-query Game.h
odb --database pgsql --generate-query Bet.h
odb --database pgsql --generate-query Win.h
odb --database pgsql --generate-query GameConfiguration.h
odb --database pgsql --generate-query PlayHistory.h
odb --database pgsql --generate-query Session.h
odb --database pgsql --generate-query MachineConfiguration.h
odb --database pgsql --generate-query Account.h
odb --database pgsql --generate-query DoorState.h
odb --database pgsql --generate-query BillStackerAccess.h
odb --database pgsql --generate-query PowerState.h
odb --database pgsql --generate-query Event.h
odb --database pgsql --generate-query GameHistoryInfo.h
