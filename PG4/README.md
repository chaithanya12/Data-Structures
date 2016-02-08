Patrick Drumm, pdrumm

Programming Assignment 4
========================

_Your description here_

For my program, I used two structs and an unordered_map to organize my data storage. The structs created were side and segment. The unordered_map used a side as the key and a vector of segments as the value. Thus, I defined my hash function to hash on the street name and parity.
The constructor of street_map reads in the file and stores the data into the unordered_map. It does so by first inserting two pairs into the map when a new street name is read. These two pairs have the two different parity values in the key. The vector of segments in this initial pair is simply an empty vector. Then, as data is read in for new segments, a segment is created and inserted into the appropriate vector (by finding the corresponding key in the map).
The geocode function is used to search for the url of an inputted address. My algorithm is very simple because the time constraint did not end up to be a problem. My algorithm simply gets a pointer that points to the correct pair in the map for the inputted addresss, and then loops through each segment until it either finds the correct range or reaches the end of that vector.

_Grader comments here_
