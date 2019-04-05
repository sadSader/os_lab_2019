$( /dev/random | od -N 1 | awk '{ print $2 }')>>numbers.txt
