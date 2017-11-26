#/bin/sh
for dir in flat nested
do
	for i in {1..100}
	do
		(time ./sorter -c movie_title -d $dir) 2>&1 >/dev/null | grep real
	done | ./average
done
