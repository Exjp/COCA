#include <stdio.h>
#include <string.h>
#include <Graph.h>
#include <Parsing.h>
#include "Z3Tools.h"
#include "Solving.h"

bool v_mode;
bool g_mode;
bool F_mode;
bool s_mode;
bool d_mode;
bool a_mode;
bool t_mode;
bool k_mode;
bool f_mode;
bool o_mode;
bool h_mode;

int getkmax(Graph *graphs, unsigned int numGraphs);

void usage(){
    if(h_mode) return;
    printf("Use: equalPath [options] files...\n");
    printf(" Each file should contain a graph in dot format.\n");
    printf(" The program decides if there exists a length n such that each input graph has a valid simple path of length n.\n");
    printf(" Can display the result both on the command line or in dot format.\n");
    printf("Options: \n");
    printf(" -h          Displays this help\n");
    printf(" -v          Activate verbose mode (display satisfiability for each subclause)\n");
    printf(" -g          Displays parsed graphs\n");
    printf(" -F          Displays the formula computed (obviously not in this version, but you should really display it in your code)\n");
    printf(" -k [length] Tests the formula for a depth of [length]. [if not present: uses the global formula]\n");
    printf(" -s          Tests separately all formula by depth [if not present: uses the global formula]\n");
    printf(" -d          Only if -s is present. Explore the length in decreasing order. [if not present: in increasing order]\n");
    printf(" -a          Only if -s is present. Computes a result for every length instead of stopping at the first positive result (default behaviour)\n");
    printf(" -t          Displays the paths found on the terminal [if not present, only displays the existence of the path].\n");
    printf("-f         Writes the result with colors in a .dot file. See next option for the name. These files will be produced in the folder 'sol'.");
    printf("-o NAME    Writes the output in \"NAME-lLENGTH.dot\" where LENGTH is the length of the solution. Writes several files in this format if both -s and -a are present. [if not present: \"result-lLENGTH.dot\"]");
    printf("\n");
    printf("\n");
    h_mode = true;

}

int main(int argc, char* argv[]){


    if(argc<2) {
        usage();
        return 0;
    }

    Z3_context ctx = makeContext();

    v_mode = false;
    g_mode = false;
    F_mode = false;
    s_mode = false;
    d_mode = false;
    a_mode = false;
    t_mode = false;
    k_mode = false;
    f_mode = false;
    o_mode = false;
    h_mode = false;
    int name_ind = 0;

    int startind = 1; // debut des arg graphe
    int k;
    

    while(argv[startind][0] == '-'){
        switch (argv[startind][1])
        {
            case ('h'):
            {
                usage();
                break;
            }
            case ('v'):
            {
                v_mode = true;
                break;
            }
            case('g'):
            {
                g_mode = true;
                break;
            }
            case('F'):
            {
                F_mode = true;
                break;
            }
            case('k'):
            {
                startind++;
                if(startind >= argc){
                    printf("wrong argument: a number need to be placed after -k\n");
                    return 1;
                }
                k = atoi(argv[startind]);
                k_mode = true;
                break;
            }
            case('s'):
            {
                s_mode = true;
                break;
            }
            case('d'):
            {
                d_mode = true;
                break;
            }
            case('a'):
            {
                a_mode = true;
                break;
            }
            case('t'):
            {
                t_mode = true;
                break;
            }
            case('f'):
            {
                f_mode = true;
                break;
            }
            case('o'):
            {
                startind++;
                if(startind >= argc){
                    printf("wrong argument: a name need to be placed after -o\n");
                    return 1;
                }
                if(strlen(argv[startind]) > 1000){
                    printf("wrong argument: name too long\n");
                    return 1;
                }
                name_ind = startind;
                o_mode = true;
                break;
            }
            default:
            {
                printf("wrong argument: %c unknown\n", argv[startind][1] );
                return 1;
            }
        }
        startind++;
        if(startind >= argc){
            printf("missing argument: you need to give at least one graph\n\n");
            usage();
            return 1;
        }
    }

    if(k_mode && s_mode){
        printf("wrong argument: you can't have -s and -k at the same time\n");
        return 1;
    }
    if(d_mode && !s_mode){
        printf("wrong argument : you need to have -s in order to have -d\n");
        return 1;
    }
    if(a_mode && !s_mode){
        printf("wrong argument : you need to have -s in order to have -a\n");
        return 1;
    }
    if(o_mode && !f_mode){
        printf("wrong argument : you need to have -f in order to have -o\n");
        return 1;
    }
    Graph graph[argc-startind];
    for(int i= startind ; i< argc ; i++)
    {
        graph[i-startind] = getGraphFromFile(argv[i]);
        if(g_mode){
            printf("graph %d:\n\n",i-startind);
            printGraph(graph[i-startind]);
            printf("\n");
        }
    }

    if(!s_mode){
        Z3_ast formula = (!k_mode)? graphsToFullFormula(ctx, graph, argc - startind) : graphsToPathFormula(ctx, graph, argc-startind, k);
        if(v_mode) printf("Formula found\n");
        if(F_mode) printf("Formula is :\n%s\n",Z3_ast_to_string(ctx,formula));
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
                
                Z3_model model = getModelFromSatFormula(ctx,formula);
                if(v_mode) printf("Model done\n");
                if(!k_mode) k = getSolutionLengthFromModel(ctx,model,graph);
                printf("the formula is satisfiable, all graphs have a path of length %d.\n",k);
                if(t_mode) printPathsFromModel(ctx,model,graph,argc - startind, k);
                if(f_mode)
                    if(o_mode) createDotFromModel(ctx,model,graph,argc - startind,k,argv[name_ind]);
                    else createDotFromModel(ctx,model,graph,argc - startind,k,NULL);
                break;
        }
    }
    else{
        int kmax = getkmax(graph, argc-startind);
        if(v_mode) printf("preparing to test %d formulas\n",kmax);
        for(int ktmp=0;ktmp<=kmax;ktmp++)
        {   
            if(d_mode) k = kmax-ktmp;
            else k=ktmp;
            printf("result for k = %d : \n\n",k);
            Z3_ast formula = graphsToPathFormula(ctx, graph, argc-startind, k);
            if(F_mode) printf("Formula is :\n%s\n",Z3_ast_to_string(ctx,formula));
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
                if(t_mode) printPathsFromModel(ctx,model,graph,argc - startind, k);
                if(f_mode) 
                    if(o_mode)createDotFromModel(ctx,model,graph,argc - startind,k,argv[name_ind]);
                    else createDotFromModel(ctx,model,graph,argc - startind,k,NULL);
                if(!a_mode) return 0;
                break;
            }
        }
    }
}