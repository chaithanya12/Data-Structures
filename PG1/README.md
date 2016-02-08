Patrick Drumm, pdrumm

Programming Assignment 1 - README
=================================

See `pg1.pdf` for the assignment.


_Your complexity analysis and other notes here_

#2d. How much memory does my program use?
My program uses O(n^2) space. The undo stack that I am using to store the variables is actually a stack of all previous stacks that were once the calculator stack. Therefore, if a user were to enter N numbers into the calculator, the undo stack would have N stacks, with the last one containing N elements. I also have a user calculator stack that only contains the current numbers, but that is only O(n) space. So the program would still take O(n^2) space.


#4b. How much memory does my program use?
My program uses O(n) space. As a worst-case, my final program for part 4 will only add one node per user-inputted command. My program contains a single stack, which functions as both the calculator stack and the undo stack. Any time that a user enters a number, an arithmetic operation, or "drop", a node is added to the stack. No node is added to the stack when "swap" is called. Similarly, no nodes are added to the stack when "undo" is called. Therefore, the worst case scenario of the user entering N commands of only numbers, arithmetic commands, or "drop"s would result in N nodes being pushed into the stack.

Additionally, my new list does (barely) pass both tests! It may take a few tries to get the speed test to pass though. The speed generally varies between 0.95 and 1.05 seconds(please make sure to compile with -O3 or -O4 as the Makefile does!).


#5. Short Reflection of Assignment:
This programming assignment was a great learning experience for me to learn and think critically about time and space complexity(as well as experience in debugging!). I must say that one of the things that I was most amazed at was how much more efficiently my program ran in calc3.cpp when compared to calc2.cpp. The only difference between the two is that I replaced the stdlib stack with a templated class of my own. However, the program ran incredibly more efficiently, and even passed both the speed and memory requirements for part 4!
After part 3, I had quite a tough time implementing part 4. The requirements to reduce redundancy while still allowing "drop"s and "swap"s was challenging. In the end, I was able to do it, but with a cost of increased overhead. In order to complete part 4, I added some variables and functions to my Node class, which ended up increasing the memory that I used. If I were to do this program again, I would likely try to think of how I could improve that facet and attempt to reduce the overhead memory that each of my Nodes contain.



_Grader comments here_

Great commenting throughout the code. Also, it is a good practice to put class and struct definitions in header files.
