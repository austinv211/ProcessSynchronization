# run the makefile
make

#clear the screen
clear

# test buffer consumer problem
printf '\n*****Testing Problem #1*****\n\n'

read -p "Press Enter to Continue..."

printf "\n\n"

./boundedBuffer

# test the mh problem
printf '\n*****Testing Problem #2 with 10 cycles*****\n\n'

read -p "Press Enter to Continue..."

printf "\n\n"

./mh 10

# test the mh problem with invalid input
printf '\n*****Testing Problem #2 with invalid input*****\n\n'

read -p "Press Enter to Continue..."

printf "\n\n"

./mh blah

#test the mh problem with an int out of range
printf '\n*****Testing Problem #2 with an Int out of range*****\n\n'

read -p "Press Enter to Continue..."

printf "\n\n"

./mh 100000000000000000000000

#test the airline problem
printf '\n*****Testing Problem #3*****\n\n'

read -p "Press Enter to Continue..."

printf "\n\n"

./airline 50 5 2 3

#test the airline problem with invalid input
printf '\n*****Testing Problem #3 with Invalid Input*****\n\n'

read -p "Press Enter to Continue..."

printf "\n\n"

./airline 50 blah 2 3


