#!/bin/bash

cd "$(dirname "$0")"
cd ..

###########################################
echo DFA
echo ---------

#run thomas_vb_1 vb
./GOL input_files/ward_tests/test.ini > /dev/null 2>&1 &
read -p "Press enter to continue"
killall GOL > /dev/null 2>&1

echo PDA
echo ---------

#run thomas_vb_2 vb
./GOL input_files/thomas_test/test.ini > /dev/null 2>&1 &
read -p "Press enter to continue"
killall GOL > /dev/null 2>&1

echo PDA/PA
echo ---------

#run thomas_vb_3 vb
./GOL input_files/thomas_test2/test.ini > /dev/null 2>&1 &
read -p "Press enter to continue"
killall GOL > /dev/null 2>&1
###########################################


###########################################
echo automatic examples
echo ---------

for i in {0..6}
do
	./GOL "input_files/mano_test_inis/test$i.ini" > /dev/null 2>&1 &
	read -p "Press enter to view png and code"
	xdg-open "input_files/mano_test_other_files/laws$i.law"
	xdg-open DFA++Minimized.png
	read -p "Press enter to continue"
	killall GOL > /dev/null 2>&1
done
###########################################

###########################################
echo Wireworld
echo ---------

#run ward_vb_1 vb
./GOL input_files/ward_tests/wireworld/electron.ini > /dev/null 2>&1 &

#wacht totdat de png gegenereerd is
read -p "Press enter to view png and code"

#toon de DFA++ png en code
xdg-open input_files/ward_tests/wireworld/wireworld.law
xdg-open DFA++Minimized.png

#wacht tot het voorbeeld af is
read -p "Press enter to continue"
killall GOL > /dev/null 2>&1

#run tweede vb
./GOL input_files/ward_tests/wireworld/10500.ini > /dev/null 2>&1 &

#wacht tot het voorbeeld af is
read -p "Press enter to continue"
killall GOL > /dev/null 2>&1
###########################################
