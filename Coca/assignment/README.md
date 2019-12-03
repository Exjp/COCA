#Documentation
This project is about to implement the reduction of common length. 
The demo was produced by Vincent Penelle. 
This archive was modified  for a project in group for the master of the University of Bordeaux, by Sofian Antri and Jacques Pourtier.

##Installation
To build the executable, do : 'make'. You will get the executable './equalPath'
the other executable './working-equalPath' is an example given to represent the output of this project.
To launch, do : './equalPath' then chose some option :
     -h          Displays this help
     -v          Activate verbose mode (display satisfiability for each subclause)
     -g          Displays parsed graphs
     -F          Displays the formula computed (obviously not in this version, but you should really display it in your code)
     -k [length] Tests the formula for a depth of [length]. [if not present: uses the global formula]
     -s          Tests separately all formula by depth [if not present: uses the global formula]
     -d          Only if -s is present. Explore the length in decreasing order. [if not present: in increasing order]
     -a          Only if -s is present. Computes a result for every length instead of stopping at the first positive result (default behaviour)
     -t          Displays the paths found on the terminal [if not present, only displays the existence of the path]. 
     -f          Writes the result with colors in a .dot file. See next option for the name. These files will be produced in the folder 'sol'.
     -o NAME     Writes the output in "NAME-lLENGTH.dot" where LENGTH is the length of the solution. Writes several files in this format if both -s and -a are present. [if not present: "result-lLENGTH.dot"]
The folder graphs contains all graphs to test.
In resume to execute the project, firstly add the option (you can add multiple options at once) then add all graphs you want
For example, to test if graphs have a length of path of 2 in common :
'./equalPath -k 2 graphs/assignment-instance/G1.dot graphs/assignment-instance/triangle.dot"
