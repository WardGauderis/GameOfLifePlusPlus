#!/bin/bash

cd "$(dirname "$0")"
cd ..

###########################################
echo Wireworld
echo ---------

#run eerste vb
./GOL input_files/ward_tests/wireworld/electron.ini 2>&1 > /dev/null &

#wacht totdat de png gegenereerd is
read -p "Press enter to view png and code"

#toon de DFA++ png en code
xdg-open input_files/ward_tests/wireworld/wireworld.law
xdg-open DFA++Minimized.png

#wacht tot het voorbeeld af is
read -p "Press enter to continue"
killall GOL

#run tweede vb
./GOL input_files/ward_tests/wireworld/10500.ini 2>&1 > /dev/null &

#wacht tot het voorbeeld af is
read -p "Press enter to continue"
killall GOL 2>&1 > /dev/null
###########################################
