# Grad
Grad is a C library providing structures and functions to manipulate square and hexagonal 2D grids.

The Grad represents internally the grid as a graph where the links between a cell and its neighbour can be deleted or recreated to define connection between cells of the grid. Links are automatically generated according to the type of Grad. Cells can also be marked as blocked to temporarily be ignored by the two main functions of the Grad: flooding and search for the shortest path between two cells.

The flooding can be done from several sources simultaneously, and be constrained by the number of steps and/or the distance from the source. The flooding occurs in order consistent with the distance between cells. The distance is set up by default and can be modified by the user.

The search path is done using the A* algorithm with a look up table for the evaluation of distance. The Grad provides an automatically generated look up table, but the user can also use its own. The automatically generated look up table ensures the fastest and optimal search if there was no modification (link edited, cell blocked) to the Grad between the table generation and the search.

Hexagonal Grad supports the 4 types of possible alignements (line/column, even/odd).

Content of the cells of the Grad can be extended via a void pointer toward a user defined structure.

## How to install this repository
1) Create a directory which will contains this repository and all the repositories it is depending on. Lets call it "Repos"
2) Download the master branch of this repository into "Repos". Unzip it if necessary.
3) The folder's name for the repository should be followed by "-master", rename the folder by removing "-master".
4) cd to the repository's folder
5) If wget is not installed: ```sudo apt-get update && sudo apt-get install wget``` on Ubuntu 16.04, or ```brew install wget``` on Mac OSx
6) If gawk is not installed: ```sudo apt-get update && sudo apt-get install gawk```  on Ubuntu 16.04, ```brew install gawk``` on Mac OSx
7) If this repository is the first one you are installing in "Repos", run the command ```make -k pbmake_wget```
8) Run the command ```make``` to compile the repository. 
9) Eventually, run the command ```main``` to run the unit tests and check everything is ok.
10) Refer to the documentation to learn how to use this repository.

The dependancies to other repositories should be resolved automatically and needed repositories should be installed in the "Repos" folder. However this process is not completely functional and some repositories may need to be installed manually. In this case, you will see a message from the compiler saying it cannot find some headers. Then install the missing repository with the following command, e.g. if "pbmath.h" is missing: ```make pbmath_wget```. The repositories should compile fine on Ubuntu 16.04. On Mac OSx, there is currently a problem with the linker.
If you need assistance feel free to contact me with my gmail address: at bayashipascal.
