This program is written in C++ and uses the "SNAP" package for making a graph - http://snap.stanford.edu/. I have also used a csv reader program to read in the data file - http://ideone.com/DFjJE 

Algorithm aspects:
I have written code to read the data and build an undirected graph of people paying. The main algorithm usage here is Breadth First Search to get the shortest distance between nodes and if the distance is less than equal to 2( feature2) or 4(feature3) then it outputs trusted and unverified otherwise. 

Running the program:
The program needs to be compiled with snap files. For convenience I have included a copy of my program in the right folder. Run the make file, and pass the data to the program with the following command.

cd /src/Snap-3.0/tutorials/
make d-antifraud

The command to run the program after compilation: 
.d-antifraud [full path to batch file] [full path to stream file] [full path to output1.txt] [full path to output1.txt] [full path to output1.txt]

As an example:

./src/Snap-3.0/tutorials/d-antifraud paymo_input/batch_payment.csv paymo_input/stream_payment.csv paymo_output/output1.txt paymo_output/output2.txt paymo_output/output3.txt