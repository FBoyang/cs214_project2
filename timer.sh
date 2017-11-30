#/bin/sh
for dir in flat nested testdir_level_5
do
	echo -n "$dir: "
	for i in {1..10}
	do
		(time ./sorter -c movie_title -d $dir) 2>&1 >/dev/null | grep real
	done | ./average
done
