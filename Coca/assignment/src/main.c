#include <stdio.h>
#include <Graph.h>
#include <Parsing.h>
#include "Z3Tools.h"
#include "Solving.h"

bool printverbose;
bool printgraph;
bool printformula;

void usage(){
    printf("Usage: graphParser file\n");
    printf(" Displays information on the graph discribed by file. It should be provided with a .dot file\n");
}

int main(int argc, char* argv[]){


    if(argc<2) {
        usage();
        return 0;
    }

    Z3_context ctx = makeContext();

    printverbose = false;
    printgraph = false;
    printformula = false;

    int startind = 1; // debut des arg graphe
    int k;
    bool full = true;

    while(argv[startind][0] == '-'){
        switch (argv[startind][1])
        {
            case ('v'):
            {
                printverbose = true;
                break;
            }
            case('g'):
            {
                printgraph = true;
                break;
            }
            case('f'):
            {
                printformula = true;
                break;
            }
            case('k'):
            {
                startind++;
                if(startind >= argc){
                    usage();
                    return 0;
                }
                k = atoi(argv[startind]);
                full = false;
                break;
            }
            default:
            {
                printf("wrong argument: %c unknown\n", argv[startind][1] );
                return 0;
            }
        }
        startind++;
        if(startind >= argc){
            usage();
            return 0;
        }
    }
    Graph graph[argc-startind];
    for(int i= startind ; i< argc ; i++)
    {
        graph[i-startind] = getGraphFromFile(argv[i]);
        if(printgraph){
            printf("graph %d:\n",i-startind);
            printGraph(graph[i-startind]);
        }
    }
    Z3_ast formula = (full)? graphsToFullFormula(ctx, graph, argc - startind) : graphsToPathFormula(ctx, graph, argc-startind, k);

    Z3_lbool isSat = isFormulaSat(ctx,formula);

        switch (isSat)
        {
        case Z3_L_FALSE:
            printf("the formula is not satisfiable.\n");
            break;

        case Z3_L_UNDEF:
                printf("We don't know if the formula is satisfiable.\n");
            break;

        case Z3_L_TRUE:
                printf("the formula is satisfiable.\n");
                Z3_model model = getModelFromSatFormula(ctx,formula);
                if(full) k = getSolutionLengthFromModel(ctx,model,graph);
                printf("k= %d\n",k);
                printPathsFromModel(ctx,model,graph,argc - startind, k);
                break;
        }
    //printf("Formula %s created.\n",Z3_ast_to_string(ctx,formula));
    

}