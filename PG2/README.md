Patrick Drumm, pdrumm

Programming Assignment 2 - README
=================================

_Your experimental results here_

The following test results were run using the condor script.
Results run in (measured time(s), memory usage(Mbytes) )

distance3:	(10.159,90.687) (11.418,90.949) (10.267,90.687) (10.384,90.683) (10.209,90.824)
	average: (10.487,90.766)
The speed and memory of the vector and its sort performed just about as well(or better) than the other
std implementations. A vector is compactly stored and not bad for sorting in a reasonable time.

distance4a:	(10.743,121.652) (10.690,121.652) (10.624,121.644) (10.615,121.648) (10.315,121.652)
	average: (10.597,121.649)
4a performed similarly to 3 as far as speed is concerned, but it used a bit more memory. This is likely because
a linked list needs to store pointers to the next node while vectors are just contiguously stored in memory.

distance4b:	(11.473,245.359) (11.567,245.263) (11.838,246.218) (11.822,246.031) (11.582,245.621) 
	average: (11.656,245.698)
4b used a over twice as much memory as the other programs that I wrote and it was also the slowest. This is obviously
due to the fact that the list was copied into a vector and then back into a list - requiring much more time and
memory.

distance5:	(9.319,91.269) (9.337,91.203) (9.262,90.687) (10.316,91.175) (9.290,90.691)
	average: (9.504,91.005)
I used the bentley mcilroy 3 way partitioning algorithm on a vector for my sorting implementation.
This was certainly a little tougher to implement, however it was worth it in performance. The speed of my 
sorting algorithm was about 10% faster than any of the previous programs in this homework.




_Grader comments here_

