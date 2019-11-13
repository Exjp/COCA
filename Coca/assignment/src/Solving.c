#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <Solving.h>
#include "Z3Tools.h"
#include <stdarg.h>

extern bool v_mode;

extern bool g_mode;

extern bool F_mode; 

extern bool s_mode;

extern bool d_mode;

extern bool a_mode;

extern bool t_mode;

extern bool k_mode;

extern int getkmax(Graph *graphs, unsigned int numGraphs);

Z3_ast firstClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength){
    Z3_ast tab1[numGraphs];
    for( int i = 0 ; i < numGraphs ; i ++){
        Z3_ast tab2[pathLength+1];
        for(int j = 0 ; j <= pathLength ; j++ ){
            Z3_ast tab3[graphs[i].numNodes];
            for( int q = 0 ; q < graphs[i].numNodes ; q++){
                Z3_ast tab4[graphs[i].numNodes-1];
                int cpt = 0;
                for( int q2 = 0 ; q2 < graphs[i].numNodes ; q2++){
                    if(q!=q2){
                        Z3_ast x[2] = {Z3_mk_not(ctx,getNodeVariable(ctx,i,j,pathLength,q)), Z3_mk_not(ctx,getNodeVariable(ctx,i,j,pathLength,q2))};
                        tab4[cpt] = Z3_mk_or(ctx,2, x);
                        cpt++;
                    }
                }
                tab3[q] = Z3_mk_and(ctx,graphs[i].numNodes-1,tab4);
            }
            tab2[j]= Z3_mk_and(ctx,graphs[i].numNodes,tab3);
        }
        tab1[i] = Z3_mk_and(ctx,pathLength+1,tab2);
    }
    Z3_ast f1 = Z3_mk_and(ctx,numGraphs,tab1);
    int n = 9;
    if (!v_mode) return f1;
    Z3_lbool isSat = isFormulaSat(ctx,f1);

        switch (isSat)
        {
        case Z3_L_FALSE:
            printf("the formula 1 is not satisfiable.\n");
            break;

        case Z3_L_UNDEF:
                printf("We don't know if the formula 1 is satisfiable.\n");
            break;

        case Z3_L_TRUE:
                printf("the formula 1 is satisfiable.\n");
                break;
        }
    return f1;
}


Z3_ast secondClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength){
    Z3_ast tab1[numGraphs];
    for( int i = 0 ; i < numGraphs ; i ++){
        Z3_ast tab2[pathLength+1];
        for(int j = 0 ; j <= pathLength ; j++ ){
            Z3_ast tab3[graphs[i].numNodes];
            for( int q = 0 ; q < graphs[i].numNodes ; q++){
                tab3[q] = getNodeVariable(ctx,i,j,pathLength,q);
            }
            tab2[j]= Z3_mk_or(ctx,graphs[i].numNodes,tab3);
        }
        tab1[i] = Z3_mk_and(ctx,pathLength+1,tab2);
    }
    Z3_ast f2 = Z3_mk_and(ctx,numGraphs,tab1);
    if (!v_mode) return f2;
    Z3_lbool isSat = isFormulaSat(ctx,f2);

        switch (isSat)
        {
        case Z3_L_FALSE:
            printf("the formula 2 is not satisfiable.\n");
            break;

        case Z3_L_UNDEF:
                printf("We don't know if the formula 2 is satisfiable.\n");
            break;

        case Z3_L_TRUE:
                printf("the formula 2 is satisfiable.\n");
                break;
        }
        return f2;
}


Z3_ast thirdClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength){
    Z3_ast tab1[numGraphs];
    for( int i = 0 ; i < numGraphs ; i ++){
        Z3_ast tab2[graphs[i].numNodes];
        for(int q = 0 ; q < graphs[i].numNodes; q++){
            Z3_ast tab3[pathLength+1];
            for(int j = 0 ; j<= pathLength;j++){
                Z3_ast tab4[pathLength];
                int cpt=0;
                for(int j2 = 0; j2 <= pathLength; j2++){
                    if(j2 != j){
                        Z3_ast x[2] = {Z3_mk_not(ctx,getNodeVariable(ctx,i,j,pathLength,q)),Z3_mk_not(ctx,getNodeVariable(ctx,i,j2,pathLength,q))};
                        tab4[cpt] = Z3_mk_or(ctx,2,x) ;
                        cpt++;
                    }
                }
                tab3[j]= Z3_mk_and(ctx,pathLength,tab4);
            }
            tab2[q] = Z3_mk_and(ctx,pathLength+1,tab3);
        }
        tab1[i] = Z3_mk_and(ctx,graphs[i].numNodes,tab2) ;
    }
    Z3_ast f3 = Z3_mk_and(ctx,numGraphs,tab1);
    if (!v_mode) return f3;
    Z3_lbool isSat = isFormulaSat(ctx,f3);

        switch (isSat)
        {
        case Z3_L_FALSE:
            printf("the formula 3 is not satisfiable.\n");
            break;

        case Z3_L_UNDEF:
                printf("We don't know if the formula 3 is satisfiable.\n");
            break;

        case Z3_L_TRUE:
                printf("the formula 3 is satisfiable.\n");
                break;
        }
    return f3;
}

int getSourceNode(Graph graph){
    for(int i = 0; i < graph.numNodes; i++){
        if(isSource(graph, i)){
            return i;
        }
    }
    return -1; // pas de source ? retourne le premier noeud du graphe
}

int getEndNode(Graph graph){
    for(int i = 0; i < graph.numNodes; i++){
        if(isTarget(graph, i)){
            return i;
        }
    }
    return -1;
}

Z3_ast fourthClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength){
    int s, t;
    Z3_ast tab1[numGraphs];
    for( int i = 0 ; i < numGraphs ; i ++){
        s = getSourceNode(graphs[i]);
        t = getEndNode(graphs[i]);
        if(s==-1 || t==-1){
            tab1[i] = Z3_mk_false(ctx);
            continue;
        }
        Z3_ast x[2] = {getNodeVariable(ctx,0,s,pathLength,s), getNodeVariable(ctx,i,pathLength,pathLength,t)};
        tab1[i]= Z3_mk_and(ctx,2, x);
    }
    Z3_ast f4 = Z3_mk_and(ctx,numGraphs,tab1);
    if(!v_mode) return f4;
    Z3_lbool isSat = isFormulaSat(ctx,f4);

        switch (isSat)
        {
        case Z3_L_FALSE:
            printf("the formula 4 is not satisfiable.\n");
            break;

        case Z3_L_UNDEF:
                printf("We don't know if the formula 4 is satisfiable.\n");
            break;

        case Z3_L_TRUE:
                printf("the formula 4 is satisfiable.\n");
                break;
        }
    return f4;
}

Z3_ast fithClause( Z3_context ctx, Graph *graphs, unsigned int numGraphs, int pathLength){
    Z3_ast tab1[numGraphs];
    for( int i = 0 ; i < numGraphs ; i ++){
        Z3_ast tab2[pathLength];
        for(int j = 0 ; j <= pathLength-1 ; j++ ){
            Z3_ast tab3[graphs[i].numNodes*graphs[i].numNodes-graphs[i].numEdges];
            int cpt = 0;
            for( int u = 0 ; u < graphs[i].numNodes ; u++){
                for( int v = 0 ; v < graphs[i].numNodes ; v++){
                    if(!isEdge(graphs[i], u, v)){
                        Z3_ast x[2] = { Z3_mk_not(ctx,getNodeVariable(ctx,i,j,pathLength,u)), Z3_mk_not(ctx,getNodeVariable(ctx,i,j+1,pathLength,v)) };
                        tab3[cpt] = Z3_mk_or(ctx,2, x);
                        cpt++;
                    }
                }
            }
            tab2[j]= Z3_mk_and(ctx,cpt,tab3);
        }
        tab1[i] = Z3_mk_and(ctx,pathLength,tab2);
    }
    Z3_ast f5 = Z3_mk_and(ctx,numGraphs,tab1);
    if(!v_mode) return f5;
    Z3_lbool isSat = isFormulaSat(ctx,f5);

        switch (isSat)
        {
        case Z3_L_FALSE:
            printf("the formula 5 is not satisfiable.\n");
            break;

        case Z3_L_UNDEF:
                printf("We don't know if the formula 5 is satisfiable.\n");
            break;

        case Z3_L_TRUE:
                printf("the formula 5 is satisfiable.\n");
                break;
        }
    return f5;
}

int getkmax(Graph *graphs, unsigned int numGraphs){
    int maxEdges = graphs[0].numEdges;
    for(int i = 1; i < numGraphs; i++){
        if(graphs[i].numEdges < maxEdges){
            maxEdges = graphs[i].numEdges;
        }
    }
    return maxEdges;
}

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
    };

    Z3_ast formulaofGraphs = Z3_mk_and(ctx, 5, formulaofGraphsTab);
    if(!v_mode) return formulaofGraphs;
    Z3_lbool isSat = isFormulaSat(ctx,formulaofGraphs);

        switch (isSat)
        {
        case Z3_L_FALSE:
            printf("the formula with pathlength %d is not satisfiable.\n",pathLength);
            break;

        case Z3_L_UNDEF:
                printf("We don't know if the formula with pathlength %d is satisfiable.\n");
            break;

        case Z3_L_TRUE:
                printf("the formula with pathlength %d is satisfiable.\n");
                break;
        }
    return formulaofGraphs;
}

Z3_ast graphsToFullFormula( Z3_context ctx, Graph *graphs,unsigned int numGraphs){
    int kmax = getkmax(graphs, numGraphs);
    Z3_ast formulakmax[kmax];
    int cpt = 0;
    for(int i = 0; i < kmax; i++){
        Z3_ast tmp = graphsToPathFormula(ctx,graphs,numGraphs,i);
        if(isFormulaSat(ctx,tmp)){
            formulakmax[cpt] = tmp;
            cpt++;
        }
    }
    if(cpt == 0) return Z3_mk_false(ctx);
    Z3_ast ret = Z3_mk_or(ctx, cpt, formulakmax);
    return ret;
}

int getSolutionLengthFromModel(Z3_context ctx, Z3_model model, Graph *graphs){
    int kmax = graphs[0].numEdges;
    int k;
    for(k=0;k<=kmax;k++){
        bool oneok = false;
            for(int q=0;q<graphs[0].numNodes;q++){
                if(valueOfVarInModel(ctx,model,getNodeVariable(ctx, 0,k,k,q))){
                    oneok =true;
                    break;
                }
            }
        if(oneok) break;
    }
    return k;
}

void printPathsFromModel(Z3_context ctx, Z3_model model, Graph *graphs, int numGraph, int pathLength){
    for(int i =0; i<numGraph;i++){
        printf("Path for graphe %d:\n",i);
        for(int j=0; j<=pathLength; j++){
            for(int q = 0;q<graphs[i].numNodes;q++){
                if(valueOfVarInModel(ctx,model,getNodeVariable(ctx, i,j, pathLength,q))){
                    printf("pos %d: %s ",j,graphs[i].nodes[q]);
                    continue;
                }
            }
            if(j+1<=pathLength) printf("-> ");
        }
        printf("\n");
    }
}

void createDotFromModel(Z3_context ctx, Z3_model model, Graph *graphs, int numGraph, int pathLength, char* name){
    
    File* fp;
    for(int i = 0; i < numGraph; i++){
        if(name[i] == NULL){
            fp = fopen("result-l%d.dot",pathLength);
        }
        else{
            fp = fopen("%s-l%d.dot",name[i], pathLength);
        }
        fprintf("q%d [initial=1,color=green];\n",getSourceNode(graphs[i]));
        fprintf("q%d [initial=1,color=red];\n", getEndNode(graphs[i]));
        for(int j = 0; j < pathlength; j++){                              
            for(int s = 0; s < graphs[i].numNodes; s++){
                for(int t = s+1; t < graphs[i].numNodes - 1; t++){
                    if(isEdge(graphs[i],s,t)){
                        fprintf(fp,"q%d -> q%d",s,t);
                    }
                    for(int k = 0; k < pathlength; k++){
                        if(valueOfVarInModel(ctx,model,getNodeVariable(ctx,i,0,pathLength,q))){
                            fprintf(fp," [color=blue]");
                        }
                    }
                    fprintf(fp, ";\n");
                }
            }
        }
        fprintf(fp,"}");
        fclose(fp);
    }
    
}

