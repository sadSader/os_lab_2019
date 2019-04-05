count=1
while [ "$count" -le 150 ]      # Генерация 10 ($MAXCOUNT) случайных чисел.

do

  number=$RANDOM

  echo $number>>numbers.txt

  let "count += 1"  # Нарастить счетчик.

done
