# Grad
Grad is a C library providing structures and functions to manipulate square and hexagonal 2D grids.

The Grad represents internally the grid as a graph where the links between a cell and its neighbour can be deleted or recreated to define connection between cells of the grid. Links are automatically generated according to the type of Grad. Cells can also be marked as blocked to temporarily be ignored by the two main functions of the Grad: flooding and search for the shortest path between two cells.

The flooding can be done from several sources simultaneously, and be constrained by the number of steps and/or the distance from the source. The flooding occurs in order consistent with the distance between cells. The distance is set up by default and can be modified by the user.

The search path is done using the A* algorithm with a look up table for the evaluation of distance. The Grad provides an automatically generated look up table, but the user can also use its own. The automatically generated look up table ensures the fastest and optimal search if there was no modification (link edited, cell blocked) to the Grad between the table generation and the search.

Hexagonal Grad supports the 4 types of possible alignements (line/column, even/odd).

Content of the cells of the Grad can be extended via a void pointer toward a user defined structure.
