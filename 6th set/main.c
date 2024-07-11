#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VCC (1)
#define GND (0)
#define X (2)
#define Z (3)
#define SC (4)

#define NMOS (1)
#define PMOS (2)

#define NOT (3)
#define NOR_2 (4)
#define NAND_2 (5)
#define XOR_2 (6)

#define pink (1)
#define blue (2)



struct Graph
{
    // An array of pointers to Node to represent an adjacency list
    struct GraphNode* head[1000];
};

// Data structure to store adjacency list nodes of the graph
struct GraphNode
{
    int nodenumber;
    int numOfGate ;
    int whatGate ;
    int degreeNext;
    int degreePrev;
    struct GraphNode* next[14];
    struct GraphNode* prev[14];

};


char *remove_white_spaces(char *str)
{
	int i = 0, j = 0;
	while (str[i])
	{
		if (str[i] != ' ')
          str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
	return str;
}

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Function to perform Selection Sort
void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {

        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

// Function to perform Selection Sort
void selectionSortForNodeArray(struct GraphNode* arr[], int n)
{
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {

        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j]->whatGate < arr[min_idx]->whatGate)
                min_idx = j;

        // Swap the found minimum element
        // with the first element
        struct GraphNode* temp =arr[min_idx] ;
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

sortMatchingGates(int arr[],int arr2[],int  n){
     int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {

        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
        swap(&arr2[min_idx], &arr2[i]);
    }
}
char* stbetween(char PATTERN1 ,char PATTERN2,char *s ){///////////////////////////// this function funds a string between 2 characters

    char *target = NULL;
    char *start, *end;

    if ( start = strchr( s, PATTERN1 ) )
    {
        start = start+1;
        if ( end = strchr( start, PATTERN2 ) )
        {
            target = ( char * )malloc( end - start + 1 );
            memcpy( target, start, end - start );
            target[end - start] = '\0';
        }
    }

    if ( target )  return target;

    else return 0;

}



void initialization(int inp[],int out[],int netlist[][3],int logicgate[],char** Lib,char* filname){
char ch;
   FILE *fp;

   fp = fopen(filname, "r"); // read modet          ///////////////////////////////////////////

   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }                                                     // open and read file
   int i=0;
   char A[10000];
   while((ch = fgetc(fp)) != EOF){
      A[i]=ch;
      i++;
      }                                                 //////////////////////////////////////////
    fclose(fp);


   char * S;
   S=A;                           // set a new string S = with the content of file = A


   if(strstr(S,"LIBRARY" )!=NULL){
        S=strstr(S,"LIBRARY" );
        *Lib=stbetween('\n','\n',S);
    }
   int vccarray[100]={};

   while(strstr(S,"VCC" )!=NULL){  // find the nodes which are conected with VCC
       i=0;
       S=strstr(S,"VCC" );      /////// find the inputs
       S=S+2;
       char *ptrvcc = strtok(stbetween('C','G',S) , ";");
       while(ptrvcc != NULL)
         {
            vccarray[i]=atoi(ptrvcc);
            ptrvcc = strtok(NULL, ";");
            i++;
         }
   }


   int gndarray[100]={};
   while(strstr(S,"GND" )!=NULL){  // find the nodes which are conected with GND
       i=0;
       S=strstr(S,"GND" );      /////// find the inputs
       S=S+2;
       char *ptrgnd = strtok(stbetween('D','#',S) , ";");
       while(ptrgnd != NULL)
         {
            gndarray[i]=atoi(ptrgnd);
            ptrgnd = strtok(NULL, ";");
            i++;
         }
    }

   char* delimiter=NULL;
   char* helpingstring=NULL;
   i=0;
   S=strstr(S,"INPUTS" );      /////// find the inputs
   helpingstring=stbetween('\n','\n',S);
   if(strstr(S,";" )!=NULL){delimiter=";";}
   if(strstr(S,"," )!=NULL){delimiter=",";}

   char *ptrIN = strtok(stbetween('\n','#',S) , delimiter);
   while(ptrIN != NULL)
	 {
	 	inp[i]=atoi(ptrIN);
	 	ptrIN = strtok(NULL, delimiter);
	 	i++;

	 }


   i=0;
   S=strstr(S,"OUTPUTS" );      /////// find the outputs
   char *ptrOut = strtok(stbetween('\n','#',S) , delimiter);
   while(ptrOut != NULL)
	 {
	 	out[i]=atoi(ptrOut);
	 	ptrOut = strtok(NULL, delimiter);
	 	i++;
	 }


   i=0;
   char* temparr[500]={};
   S=strstr(S,"NETLIST" );
   char *ptrNet = strtok(stbetween('\n','#',S) , "\n");
   while(ptrNet != NULL)
	 {
	 	temparr[i]=ptrNet;
	 	ptrNet = strtok(NULL, "\n");
	 	i++;
	 }

	i=0;
    int max=1;
    S=strstr(S,"\n" );
    S=S+3;
    char* mychar=NULL;
    if(S[0]==','){mychar=",";}
    else if(S[0]==' '){mychar=" ";}

	while(temparr[i]!=NULL){                    ///////////////////// find which transistors are PMOS and NMOS and
       char *ptrNet2 = strtok(temparr[i] , mychar);///////////////////// the GATE_NODE , P_SOURCE_NODE/N_DRAIN_NODE and
       int j=1;
       int k=2;                                //////////////////// P_DRAIN_NODE/N_SOURCE_NODE
        while(ptrNet2 != NULL)
         {
            if(j==1){
                ptrNet2 = strtok(NULL, mychar);
                j++;
            }
            else if(j==2){
                if(strcmp(remove_white_spaces(ptrNet2),"NOT")==0){
                        logicgate[i]=NOT;
                        ptrNet2 = strtok(NULL, mychar);
                        j++;
                }
                else if(strcmp(remove_white_spaces(ptrNet2),"NOR_2")==0){
                        logicgate[i]=NOR_2;
                        ptrNet2 = strtok(NULL, mychar);
                        j++;
                }
                else if (strcmp(remove_white_spaces(ptrNet2),"NAND_2")==0){
                    logicgate[i]=NAND_2;
                    ptrNet2 = strtok(NULL, mychar);
                    j++;
                }
                else if (strcmp(remove_white_spaces(ptrNet2),"XOR_2")==0){
                    logicgate[i]=XOR_2;
                    ptrNet2 = strtok(NULL, mychar);
                    j++;
                }
                else if(strcmp(remove_white_spaces(ptrNet2),"NMOS")==0) {
                    logicgate[i]=NMOS;
                    ptrNet2 = strtok(NULL, mychar);
                    j++;
                }
                else {
                    logicgate[i]=PMOS;
                    ptrNet2 = strtok(NULL, mychar);
                    j++;
                }

            }
            else if(j==3){
                if(logicgate[i]==PMOS || logicgate[i]==NMOS){
                    netlist[i][0]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                }
                ptrNet2 = strtok(NULL, mychar);
                j++;
            }
            else if(j==4){
                if(logicgate[i]==PMOS || logicgate[i]==NMOS){
                    netlist[i][1]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                }
                else {
                    netlist[i][0]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                    }
                ptrNet2 = strtok(NULL, mychar);
                j++;
            }
             else if(j==5){
                if(logicgate[i]==PMOS || logicgate[i]==NMOS){
                    netlist[i][2]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                }
                else if(strcmp(remove_white_spaces(ptrNet2),"OUT")!=0) {
                    netlist[i][1]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                }
                else{
                    netlist[i][1]=netlist[i][0];
                }
                ptrNet2 = strtok(NULL, mychar);
                j++;
            }
            else if(j==6){
             if(strcmp(remove_white_spaces(ptrNet2),"OUT")!=0) {
                    netlist[i][2]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                }
             ptrNet2 = strtok(NULL, mychar);
             j++;
            }
            else if(j>=7){
                 netlist[i][k]=atoi(ptrNet2);
                 k++;
                 if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                 ptrNet2 = strtok(NULL, mychar);
                 j++;
            }
         }
         i++;
	}                                       ////////////////////////////////////////////////////////////////////////////////////
}

int homomorphicGraphs(struct Graph* prototypeGr,struct Graph* checkingGr,struct GraphNode* arrayOfNodes[],struct GraphNode* arrayOfNodesCheck[],int numOfNodes,int numOfEdges,int numOfNodesCheck,int numOfEdgesCheck,int matchedProt[],int matchedCheck[]){
    int count=1;
    int match=0;
    int numOfGatesProt=0;
    int numOfGatesCheck=0;
    int i=0;
    while(i!=numOfNodesCheck){
        if(checkingGr->head[i]->whatGate!=0){numOfGatesCheck++;}
        i++;
    }
    i=0;
    while(i!=numOfNodes){
        if(prototypeGr->head[i]->whatGate!=0){numOfGatesProt++;}
        i++;
    }
    if(numOfGatesProt!=numOfGatesCheck ){ // if the two graphs have different number of gates then they are not homomorphic
        printf("The 2 graphs have different numbero of Nodes");
        return -1;
    }
    else if(numOfEdges!=numOfEdgesCheck){ // if the two graphs have different number of edges then they are not homomorphic
        printf("The 2 graphs have different numbero of Edges");
        return -2;
    }
    else{
         int i=0;
        while(i!=numOfNodesCheck){//loop for every node of cheking graph
            int j=0;
            int matchedCheckCopy[100]={};
            int matchedProtCopy[100]={};

            int failureMatchedCheck[100]={};
            int failureMatchedProt[100]={};
            int failurecount=0;
            //loop for each node of prototype graph that is possible to match with the some node of checking graph
            while(arrayOfNodesCheck[i]->whatGate >= arrayOfNodes[j]->whatGate && matchedCheck[arrayOfNodesCheck[i]->nodenumber-1]==0 ){
                // if the 2 nodes seems to matching
                if(arrayOfNodesCheck[i]->whatGate==arrayOfNodes[j]->whatGate && arrayOfNodesCheck[i]->degreeNext==arrayOfNodes[j]->degreeNext && arrayOfNodesCheck[i]->degreePrev==arrayOfNodes[j]->degreePrev ){
                    memset( matchedCheckCopy,0,sizeof(matchedCheckCopy));
                    memset( matchedProtCopy,0,sizeof(matchedProtCopy));
                    int k=0;
                    while(k<numOfNodesCheck){//copy the arrays that keep the matched nodes so we can use them in recursion function
                        matchedCheckCopy[k]=matchedCheck[k];
                        matchedProtCopy[k]=matchedProt[k];
                        k++;
                    }
                    int countCopy=count;
                    // call the function to find if the nodes are actually equal by checking the equality of all their neighbors
                    match=recursion(&countCopy,matchedProtCopy,matchedCheckCopy,arrayOfNodesCheck[i],arrayOfNodes[j]);
                    if(match==1&&(arrayOfNodesCheck[i]->degreeNext!=0||arrayOfNodesCheck[i]->degreePrev!=0)){ // if the 2 nodes is successfully matched
                        int k=0;
                        count=countCopy;
                        while(k<numOfNodesCheck){// update the arrays that keep the matched nodes
                            matchedCheck[k]=matchedCheckCopy[k];
                            matchedProt[k]=matchedProtCopy[k];
                            k++;
                        }
                    }
                    if(match==0&&(arrayOfNodesCheck[i]->degreeNext!=0||arrayOfNodesCheck[i]->degreePrev!=0)){ // if the 2 nodes is not matched
                        int k=0;
                        if(countCopy>failurecount){ // keep the most matched nodes
                        failurecount=countCopy;
                        while(k<numOfNodesCheck){// update the arrays that keep the matched nodes
                            failureMatchedCheck[k]=matchedCheckCopy[k];
                            failureMatchedProt[k]=matchedProtCopy[k];
                            k++;
                        }
                        }
                    }
                }
                j++;
            }
            if(match==0&&(arrayOfNodesCheck[i]->degreeNext!=0||arrayOfNodesCheck[i]->degreePrev!=0)){ // if there is a node which cant match with anything then the 2 graphs are not homomorphic
                printf("the 2 graphs are not homomorpic");
                int k=0;
                while(k<numOfNodesCheck){// update the arrays that keep the matched nodes
                    matchedCheck[k]=failureMatchedCheck[k];
                    matchedProt[k]=failureMatchedProt[k];
                    k++;
                }
                return -3;
            }
            i++;
        }
    }
    return 1;
}

int recursion(int*count,int matchedProt[],int matchedCheck[],struct GraphNode* NodeCheck,struct GraphNode* NodeProt){
    //exti condition
    if(NodeCheck->whatGate==NodeProt->whatGate && NodeCheck->degreeNext==0 && NodeProt->degreeNext==0 && NodeCheck->degreePrev==NodeProt->degreePrev && matchedCheck[NodeCheck->nodenumber-1]==matchedProt[NodeProt->nodenumber-1] ){
        if(matchedCheck[NodeCheck->nodenumber-1]==0 && matchedProt[NodeProt->nodenumber-1]==0){// put the matched nodes in the arrayes
            matchedCheck[NodeCheck->nodenumber-1]=*count;
            matchedProt[NodeProt->nodenumber-1]=*count;
            *count=*count+1;
        }
        return 1;
    }//
    int i=0;
    int match=0;
    while(NodeCheck->next[i]!=0){// loop for every neighbor of the cheking node
        int j=0;
        match=0;
        while(NodeProt->next[j]!=0){// loop for every neighbor of the prototype node
            //if the 2 nodes match
            if(match==0 && NodeCheck->next[i]->whatGate==NodeProt->next[j]->whatGate && NodeCheck->next[i]->degreeNext == NodeProt->next[j]->degreeNext&& NodeCheck->next[i]->degreePrev==NodeProt->next[j]->degreePrev && matchedCheck[NodeCheck->next[i]->nodenumber-1]==matchedProt[NodeProt->next[j]->nodenumber-1] ){
                //do the same thing for the matched nodes
                match=recursion(count,matchedProt,matchedCheck,NodeCheck->next[i],NodeProt->next[j]);
            }
            j++;
        }
        //if there is a node that doesnt match with any of prototypes nodes then the 2 firstly examined nodes are not the same
        if(match==0){return 0;}
        i++;
    }


    if(match==1 && matchedCheck[NodeCheck->nodenumber-1]==0 && matchedProt[NodeProt->nodenumber-1]==0){ // put the matched nodes in the arrayes
        matchedCheck[NodeCheck->nodenumber-1]=*count;
        matchedProt[NodeProt->nodenumber-1]=*count;
        *count=*count+1;
    }
    return 1;

}

struct Graph* createDAG(int FinalNet[][3],int logicgate[],int*numOfNodes,int*numOfEdges,struct GraphNode* arrayOfNodes[] ){

    int i=0;
    int max=0;
    int distinctnode[1000]={};
    int countdistinctnode=0;
    while(FinalNet[i][0]!=0){ //finds the number of the bigest node of Netlist
        int k=0;
        while(k<3){
            int check=0;
            int j=0;
            while(distinctnode[j]!=0){
                if(distinctnode[j]==FinalNet[i][k]) {check=1;}
                j++;
            }
            if(check==0){
                distinctnode[countdistinctnode]=FinalNet[i][k];
                if(distinctnode[countdistinctnode]>max){max=distinctnode[countdistinctnode];}
                countdistinctnode++;
            }
            k++;
        }
        i++;
    }
    // allocate storage for the graph data structure
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

     // initialize head pointer for all vertices
    for (i = 0; i < 1000; i++) {
        graph->head[i] = NULL;
    }

    for(i=0;i<max;i++){   //creates all nodes and put them in the graph
        struct GraphNode* newNode = (struct GraphNode*)malloc(sizeof(struct GraphNode));
        arrayOfNodes[i]=newNode;
        newNode->degreeNext=0;
        newNode->numOfGate=0;
        newNode->degreePrev=0;
        newNode->whatGate=0;
        for (int j = 0; j < 14; j++) {
        newNode->next[j] = NULL;
        newNode->prev[j] = 0;
        }
        newNode->nodenumber=i+1;
        graph->head[i]=newNode;
    }

    i=0;
    while(FinalNet[i][0]!=0){////////////////////////////// create the eadges between all nodes
        int point=0;
        graph->head[FinalNet[i][2]-1]->whatGate=logicgate[i];
        graph->head[FinalNet[i][2]-1]->numOfGate=i+1;
        while(graph->head[FinalNet[i][2]-1]->prev[point]!=NULL) {point++;}
        graph->head[FinalNet[i][2]-1]->prev[point]=graph->head[FinalNet[i][0]-1];
        graph->head[FinalNet[i][2]-1]->degreePrev++;
        *numOfEdges=*numOfEdges+1;
        if(logicgate[i]!=NOT){
            graph->head[FinalNet[i][2]-1]->prev[point+1]=graph->head[FinalNet[i][1]-1];
            graph->head[FinalNet[i][2]-1]->degreePrev++;
            *numOfEdges=*numOfEdges+1;
        }

        int point1=0;
        while(graph->head[FinalNet[i][0]-1]->next[point1]!=NULL) {point1++;}
        graph->head[FinalNet[i][0]-1]->next[point1]=graph->head[FinalNet[i][2]-1];
        graph->head[FinalNet[i][0]-1]->degreeNext++;
        if(logicgate[i]!=NOT){
            int point2=0;
            while(graph->head[FinalNet[i][1]-1]->next[point2]!=NULL) {point2++;}
            graph->head[FinalNet[i][1]-1]->next[point2]=graph->head[FinalNet[i][2]-1];
            graph->head[FinalNet[i][1]-1]->degreeNext++;
        }
        i++;
    }//////////////////////////////////////////////////////////////////////////////////////////////

    *numOfNodes=max;
    selectionSortForNodeArray(arrayOfNodes,max);
    return graph;
}

void createFileOfMatchingGates(int matchedGateProt[],int matchedGateCheck[],int netlist[][3],int logicgate[],int netlistCheck[][3],int ishomomorphic){
     FILE *fptr1;
       fptr1 = fopen("FileOfMatchingGates.txt","w");//open file for write
       if(fptr1 == NULL)
           {
              printf("Error!");
              exit(1);
           }
        if(ishomomorphic==-1){
            fprintf(fptr1,"THE 2 CIRUITS ARE NOT THE SAME\n \n ********the number of GATES is different between the two circuits********");
        }
        else if(ishomomorphic==-2){
            fprintf(fptr1,"THE 2 CIRUITS ARE NOT THE SAME\n \n ********the number of WIRES is different between the two circuits********");
        }
        else if(ishomomorphic==-3){
            fprintf(fptr1,"THE 2 CIRUITS ARE NOT THE SAME\n \n ********THIS IS THE BIGGEST MATCH BETWEEN THE 2 CIRCUITS THAT COULD BE FOUND********\n\n");
            fprintf(fptr1,"PROTOTYPE CIRCUIT          CHECKING CIRCUIT\n");

            int i=0;
            while(matchedGateCheck[i]!=0){
                 if(logicgate[matchedGateProt[i]-1]!=NOT){
                    fprintf(fptr1,"G%d IN %d ,%d OUT %d    ->    G%d IN %d ,%d OUT %d\n",matchedGateProt[i],netlist[matchedGateProt[i]-1][0],netlist[matchedGateProt[i]-1][1],netlist[matchedGateProt[i]-1][2],matchedGateCheck[i],netlistCheck[matchedGateCheck[i]-1][0],netlistCheck[matchedGateCheck[i]-1][1],netlistCheck[matchedGateCheck[i]-1][2]);
                 }
                 else if(matchedGateCheck[i]==0){
                    fprintf(fptr1,"G%d IN %d  OUT %d    ->    G%d IN %d  OUT %d\n",matchedGateProt[i],netlist[matchedGateProt[i]-1][0],netlist[matchedGateProt[i]-1][2],matchedGateCheck[i],netlistCheck[matchedGateCheck[i]-1][0],netlistCheck[matchedGateCheck[i]-1][2]);
                 }
                 i++;
            }
        }
        else if(ishomomorphic>0){
            fprintf(fptr1,"THE 2 CIRUITS ARE THE SAME!!!\n \n ********THIS IS THE MATCH BETWEEN THE 2 CIRCUITS********\n\n");
            fprintf(fptr1,"PROTOTYPE CIRCUIT          CHECKING CIRCUIT\n");

            int i=0;
            while(matchedGateCheck[i]!=0){
                 if(logicgate[matchedGateProt[i]-1]!=NOT){
                    fprintf(fptr1,"G%d IN %d ,%d OUT %d    ->    G%d IN %d ,%d OUT %d\n",matchedGateProt[i],netlist[matchedGateProt[i]-1][0],netlist[matchedGateProt[i]-1][1],netlist[matchedGateProt[i]-1][2],matchedGateCheck[i],netlistCheck[matchedGateCheck[i]-1][0],netlistCheck[matchedGateCheck[i]-1][1],netlistCheck[matchedGateCheck[i]-1][2]);
                 }
                 else if(matchedGateCheck[i]==0){
                    fprintf(fptr1,"G%d IN %d  OUT %d    ->    G%d IN %d  OUT %d\n",matchedGateProt[i],netlist[matchedGateProt[i]-1][0],netlist[matchedGateProt[i]-1][2],matchedGateCheck[i],netlistCheck[matchedGateCheck[i]-1][0],netlistCheck[matchedGateCheck[i]-1][2]);
                 }
                 i++;
            }
        }
}


int main()
{
    char* Lib=NULL;

    int inp[50]={};           /////////////  define the arrays we need to store the information from file
    int out[50]={};
    int netlist[100][3]={};
    int logicgate[100]={};
    int numOfNodes=0;
    int numOfEdges=0;
    struct GraphNode* arrayOfNodes[100]={}; // this array contains the nodes of the graph sorted by the type of gate

    initialization(inp,out,netlist,logicgate,&Lib,"Prototypefile.txt");  ////it stores the information from file in the appropriate arrays


    int inpCheck[50]={};           /////////////  define the arrays we need to store the information from file
    int outCheck[50]={};
    int netlistCheck[100][3]={};
    int logicgateCheck[100]={};
    int numOfNodesCheck=0;
    int numOfEdgesCheck=0;
    struct GraphNode* arrayOfNodesCheck[100]={}; // this array contains the nodes of the graph sorted by the type of gate

    initialization(inpCheck,outCheck,netlistCheck,logicgateCheck,&Lib,"checkingfile.txt");  ////it stores the information from file in the appropriate arrays


    struct Graph* prototypeGr=createDAG(netlist,logicgate,&numOfNodes,&numOfEdges,arrayOfNodes); //create the graph for the prototype circuit

    struct Graph* checkingGr=createDAG(netlistCheck,logicgateCheck,&numOfNodesCheck,&numOfEdgesCheck,arrayOfNodesCheck);  //create the graph for the circuit we want to know if is the same with prototype

    int matchedProt[100]={};
    int matchedCheck[100]={};

    // finde if the two graphs are homomorphics
    int ishomomorphic = homomorphicGraphs(prototypeGr,checkingGr,arrayOfNodes,arrayOfNodesCheck,numOfNodes,numOfEdges,numOfNodesCheck,numOfEdgesCheck,matchedProt,matchedCheck);

    int i=1;
    int matchedGateProt[100]={};
    int matchedGateCheck[100]={};
    int count1=0;
    int count2=0;

    while(i<numOfNodesCheck){// loop that finds the matching gates from 2 files
        int j=0;
        while(j<numOfNodesCheck){
                if(matchedCheck[j]==i&&checkingGr->head[j]->numOfGate!=0){
                matchedGateCheck[count1]=checkingGr->head[j]->numOfGate;
                count1++;
                }
                j++;
        }
        int k=0;
        while(k<numOfNodesCheck){
            if(matchedProt[k]==i&&prototypeGr->head[k]->numOfGate!=0){
            matchedGateProt[count2]=prototypeGr->head[k]->numOfGate;
            count2++;
            }
            k++;

        }
        i++;
    }

    //sorting the prototype matched gates and doing the corisponding swaps to the check matched gates
    sortMatchingGates(matchedGateProt,matchedGateCheck,count1); // we doing it to make a better visualy output

    createFileOfMatchingGates(matchedGateProt,matchedGateCheck,netlist,logicgate,netlistCheck,ishomomorphic);


    i=0;
     while(matchedGateCheck[i]!=0){
         printf("\nGate %d of checking netlist matches with Gate %d of prototype netlist",matchedGateCheck[i],matchedGateProt[i]);
         i++;

    }


 }

