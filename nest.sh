#!/usr/bin/env bash
../filesplit -i $1 -n 512 -p
for i in {0..511}
do
	dir=one$(($i / 256))/two$(($i / 128))/three$(($i / 64))/four$(($i / 32))/five$(($i / 16))/six$(($i / 8))/seven$(($i / 4))/eight$(($i / 2))/nine$i
	mkdir -p $dir
	mv f$(($i + 1)).csv $dir/f${i}.csv
done
