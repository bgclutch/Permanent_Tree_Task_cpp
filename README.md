# Persistent Tree
This project provides an implementation of an Binary Search Tree in C++
with rollback- and save tree methods.

## Features:
1. An implementation of an BST with ```save``` and ```rollback``` requests
2. Python scripts for automated testing and output verification

## Installation:
Clone this repository, then reach the project directory:
```sh
git clone https://github.com/bgclutch/Persistent_Tree_Task_cpp.git
cd Persistent_Tree_Task_cpp
```

## Building:
1. Build the project:
 ```sh
cmake -B build
cmake --build build
```

## Usage:
1. Navigate to the ```build``` folder:
```sh
cd build
```
2. Run tree:
```sh
./persistant_tree/persistant_tree
```

## Running tests:
For End To End tests:
1.1 Navigate to the ```tests``` directory:
```sh
cd tests/EndToEnd
```
1.2 Run default tests with the Python script:
```sh
python3 testrun.py
```
1.3 (Optional) Or ```regenerate``` test cases:
```sh
python3 testgen.py
```
And run it as in step 2.

For unit tests:
2.1 Navigate to the ```build``` folder:
```sh
cd build
```
2. Run unit tests:
```sh
./tests/tests
```

or

1. Build the project with Docker container
```sh
docker build -t persistent-tree .
docker run --rm persistent-tree
```
