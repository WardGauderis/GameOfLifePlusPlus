#!/bin/bash

cd "$(dirname "$0")"
cd ..

###########################################

echo DFA
echo ---------

#run thomas_vb_1 vb
./GOL input_files/ward_tests/test.ini 2> /dev/null &
read -p "Press enter to continue"
killall GOL 2> /dev/null

echo PDA
echo ---------

#run thomas_vb_2 vb
./GOL input_files/thomas_test/test.ini 2> /dev/null &
read -p "Press enter to continue"
killall GOL 2> /dev/null

echo PDA/PA
echo ---------

#run thomas_vb_3 vb
./GOL input_files/thomas_test2/test.ini 2> /dev/null &
read -p "Press enter to continue"
killall GOL 2> /dev/null

echo Wireworld
echo ---------

#run ward_vb_1 vb
./GOL input_files/ward_tests/wireworld/electron.ini 2> /dev/null &

#wacht totdat de png gegenereerd is
read -p "Press enter to view png and code"

#toon de DFA++ png en code
xdg-open input_files/ward_tests/wireworld/wireworld.law
xdg-open DFA++Minimized.png

#wacht tot het voorbeeld af is
read -p "Press enter to continue"
killall GOL 2> /dev/null

#run tweede vb
./GOL input_files/ward_tests/wireworld/10500.ini 2> /dev/null &

#wacht tot het voorbeeld af is
read -p "Press enter to continue"
killall GOL 2> /dev/null
###########################################
