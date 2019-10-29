#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <Solving.h>
#include "Z3Tools.h"

Z3_ast firstClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength){
    Z3_ast tab1[numGraph];
    for( int i = 0 ; i < numGraph ; i ++){
        Z3_ast tab2[pathLength];
        for(int j = 0 ; j < pathLength ; j++ ){
            Z3_ast tab3[graphs[i].numNodes];
            for( int q = 0 ; q < graphs[i].numNodes ; q++){
                for( int q2 = 0 ; q < graphs[i].numNodes ; q2++){
                    if(q!=q2){
                        tab3[q] = Z3_mk_or(ctx,2, { Z3_mk_not(ctx,getNodeVariable(ctx,i,j,k,q)), Z3_mk_not(ctx,getNodeVariable(ctx,i,j,k,q)) });
                    }
                }
            }
            tab2[j]= Z3_mk_and(ctx,graphs[i].numNodes,tab3)
        }
        tab1[i] = Z3_mk_and(ctx,pathLength,tab2)
    }
    return Z3_mk_and(ctx,numGraph,tab1);
}
Z3_ast firstClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength)
Z3_ast firstClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength)
Z3_ast firstClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength)
Z3_ast firstClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength)

Z3_ast getNodeVariable(Z3_context ctx, int number, int position, int k, int node){
    char nodechar[10000];
    sprintf(nodechar,"x, %d, %d, %d, %d", number, position, k, node);
    return mk_bool_var(ctx, nodechar);
}

Z3_ast graphsToPathFormula( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength){
    Z3_ast formulaofGraphsTab[5] = {
        firstClause(ctx, graphs, numGraphs, pathLength),
        secondClause(ctx, graphs, numGraphs, pathLength),
        thirdClause(ctx, graphs, numGraphs, pathLength),
        fourthClause(ctx, graphs, numGraphs, pathLength),
        fithClause(ctx, graphs, numGraphs, pathLength),
    }

    Z3_ast formulaofGraphs = Z3_mk_and(ctx, 5, formulaofGraphsTab);
    return formulaofGraphs;
}

Z3_ast graphsToFullFormula( Z3_context ctx, Graph *graphs,unsigned int numGraphs){



}
