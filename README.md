# C++ Pathfinding Algorithm Visualizer
This is a grid pathfinding visualizer built using OpenGl, GLFW, GLAD, and ImGui in C++ for the intention of being able to visualize how a variety of pathfinding algorithms work step by step and in a variety of conditions.

<p align= "center">
<img src="AlgoVisAnimation.gif" width="500" height="500" />
</p>

## Usage
Only works on Windows. To use, clone or download this repo and go into the *scripts* folder.
There is a windows batch file there that you can execute and it will build the Visual studio 2022 solution.
The build is done using *premake*.
## Algorithms
### Breadth-First-Search (BFS)
**Unweighted** and **guarantees** shortest path.
### Depth-First-Search (DFS)
**Unweighted** and does **not** guarantee shortest path.
### Dijkstra
**Weighted** and **guarantees** shortest path.
### A*
**Weighted** and **guarantees** shortest path.
## Program Features
### Grid Resizing
By using the Up and Down arrow keys, you are able to resize the grid.
### Animation speed up
By using the Left and Right arrow keys, one can speed up or slow down the algorithms execution.
### Start and Pause
Can start or pause the algorithm using the UI or with the space bar. 
## TODO
2. Dynamic Status Updates on what is going on or what the user does in the text region above the legend
3. Choose from a variety of distance heuristics for the Astar algorithm
