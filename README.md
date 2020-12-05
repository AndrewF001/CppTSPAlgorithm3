# CppTSPAlgorithm3

This is Andrew Foot algorithm at the problem TSP. It's created in C++ and QT5 using Visual studio and OpenMP.

How it works:

1. Finds the top most, right most, bottom most and left most points. (4N)
2. Creates a circle by connecting all those points, but include any points that would be on the out side. (4N)
3. For each point if it's not connected then check it against each connected points and evalute the delta cost of adding it.(N^2)
4. add the lowest delta cost point and repeat until all added.(N)
5. Use vector geometry to remove any lines that overlap (N^2)
6. Check each point again if swapping would be better. (N^2)

Tricks for making it quicker: QuadTree, short int, smarter geometry, multi-threading, ordering, limit scope with each itterations.

Worst:O(N^3) average:O(N^2LogN) Best:O(N^2)

1000 nodes takes 0.3 seconds to complete on my laptop and 0.1 seconds on my PC

Why does this work: there is no randomness! it just uses maths and geometry. It makes assumptions that are always true or false. The reason a circle a created at the start is because every single point is self contained you don't have to work out a route. Also the order of those points in the circle is 100% correct all the time no matter what happens in the middle.
