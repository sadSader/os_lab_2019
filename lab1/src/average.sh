
sum=0
count=0
while read LINE

    do sum=$(($sum+$LINE))	
    let count+=1

done < $1
echo $count numbers
echo average: $(($sum/$count))
