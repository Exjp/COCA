#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <Solving.h>
#include "Z3Tools.h"


Z3_ast getNodeVariable(Z3_context ctx, int number, int position, int k, int node){
    char nodechar[10000];
    sprintf(nodechar,"x, %d, %d, %d, %d", number, position, k, node);
    return mk_bool_var(ctx, nodechar);
}

Z3_ast graphsToPathFormula( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength){
    Z3_ast formulaofGraphsTab[5] = {
        firstClause(ctx, graphs, numGraphs, pathlength),
        secondClause(ctx, graphs, numGraphs, pathlength),
        thirdClause(ctx, graphs, numGraphs, pathlength),
        fourthClause(ctx, graphs, numGraphs, pathlength),
        fithClause(ctx, graphs, numGraphs, pathlength),
    }
    
    Z3_ast formulaofGraphs = Z3_mk_and(ctx, 5, formulaofGraphsTab);
    return formulaofGraphs;
}

Z3_ast graphsToFullFormula( Z3_context ctx, Graph *graphs,unsigned int numGraphs){
    


}


