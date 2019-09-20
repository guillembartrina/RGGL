# Random Graph Generator and Layout (RGGL)

This tool has two basic features:
  - Generate random graphs and trees using a variety of algorithms
  - Display the generated graphs or custom ones (from files) into an interactive and graphic environment

## Getting Started

This project is ready-to-build for windows x64 and linux x64 systems, using make.
For any other system you need to rebuild prerequisite libraries and put them in the correct folder.

### Prerequisites

SFML (https://github.com/SFML/SFML) -> build shared libs, then put object libs into 'sfml/lib/' and shared libs into 'bin/'  
ImGui (https://github.com/ocornut/imgui) -> build shared libs manually, then put object libs into 'imgui/lib/' and shared libs into 'bin/'  

### Building

It is a portable project, you don't need to install it.

Just run:
```
make
```

If you need to clean objects and binaries:
```
make clean
```

## Usage

If you are using linux you have to add the exe folder to LD PATH:
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
```

In order to open the program in GENERATOR mode you only have to execute it:
```
./rggl.exe 
```

In order to open the program in DISPLAY mode you have to execute it with parameters:
- First parameter specifies the input file encoding: 'al' for Adjacency List, 'am' for Adjacency Matrix
- Second parameter is the input file
```
./rggl.exe (al/am) filename
```
  
### Input file encoding

The input file can be encoded in two ways:

**AL: Adjacency List**
>numnodes  
>x x x ... (end with -1)  
>x x x ... (end with -1)  
>...  
>(numnode times)  

Where x is the number of the adjacent node

Example:
```
4
1 2 -1
3 -1
0 1 -1
2 -1
```

**AM: Adjacency Matrix**
>numnodes  
>x x x x x ... (numnodes times)  
>x x x x x ... (numnodes times)  
>...  
>(numnode times)  

Where x can be: 't' for true or 'f' for false

Example:
```
5
f t t t t
f f f f f
f f f f t
f t f f f
f t f f f
```

## Guide

In GENERATOR mode you have a menu at the top right where you can find multiple options for graph generation and to save the graph into a file

By default the graph is saved in a file called 'output.txt'

### Interactivism

In both modes you can inspect the graph:

>(Left click + Drag) -> move arround  
>(Right click on a node + Drag) -> move node arround  
>(Mouse scroll) -> zoom  
>(Left shift + mouse scroll) -> node size zoom  

