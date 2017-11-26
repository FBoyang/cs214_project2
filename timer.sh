#/bin/sh
for dir in flat nested
do
	echo -n "$dir: "
	for i in {1..400}
	do
		(time ./sorter -c movie_title -d $dir) 2>&1 >/dev/null | grep real
	done | ./average
done
