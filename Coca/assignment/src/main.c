#include <stdio.h>
#include <Graph.h>
#include <Parsing.h>

void usage(){
    printf("Usage: graphParser file\n");
    printf(" Displays information on the graph discribed by file. It should be provided with a .dot file\n");
}

int main(int argc, char* argv[]){

    if(argc<2) {
        usage();
        return 0;
    }
    


}