# CS205-Project2
Feature Selection

To run the code, compile using the command
```
g++ FeatureSelection_FINAL.cpp
```
This command will create the executable file "a.out". To execute the program, type the executable's path as follows:
```
./a.out
```
At this point, you will be prompted to enter the file name for the dataset that you want to be input. Enter this file name on the same line and press ENTER. For example, if you want to run the algorithm on the small dataset, enter:
```
CS205_SP_2022_SMALLtestdata__41.txt
```
After this you will be prompted by the program to choose whether to use Forward Selection of Backward Eliminatino for the search algorithm. Enter 1 for Forward Selection or 2 for Backward Elimination and press ENTER.

The algorithm will output a trace to either "forward_trace.txt" or "backward_trace.txt" depending on which search algorithm you chose. When the program is finished executing, it will output a message showing the exection time, such as:
```
elapsed time: 751.715s
```
Again, all program output for the actual feature selection will be written in one of the trace files.