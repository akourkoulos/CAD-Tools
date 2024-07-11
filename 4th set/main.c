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

struct Lgate{
    int inpg[10];
    int outg[10];
    int vccg;
    int gndg;
    int netlist[20][3];
    int transistor[20];
    int* nodeg;
    int takeInp[5];
    int putOut;
    int sizeofnodeg;
    int* Helpnodeg;
};

struct Graph
{
    // An array of pointers to Node to represent an adjacency list
    struct GraphNode* head[1000];
};

// Data structure to store adjacency list nodes of the graph
struct GraphNode
{
    int nodenumber;
    int coloure;
    struct GraphNode* next[14];

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



int* initialization(int inp[],int testinp[],int testout[],int out[],int netlist[][3],int testvector[][50] ,int logicgate[],int* node,int*sizeofnode,char** Lib,char* filname){
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


	*sizeofnode=max+1;
    int node1[max+1];

    for(i=0;i<max+1;i++){       /////////// set value of every node = unknown {(X)=2}
        node1[i]=X;
    }


    i=0;
    while(vccarray[i]!=NULL){   /////////// set value of every conected node with VCC=1
        node1[vccarray[i]]=VCC;
        i++;
    }

     i=0;
    while(gndarray[i]!=NULL){
        node1[gndarray[i]]=GND;    /////////// set value of every conected node with GND=0
        i++;
    }


   i=0;
   S=strstr(S,"TEST_IN" );      /////// find the test inputs
   if(S!=0){
       char *ptrtin = strtok(stbetween('\n','#',S) , ";");
       while(ptrtin != NULL)
         {
            testinp[i]=atoi(ptrtin);
            ptrtin = strtok(NULL, ";");
            i++;
         }

       i=0;
       S=strstr(S,"TEST_OUT" );      /////// find the test outputs
       char *ptrtout = strtok(stbetween('\n','#',S) , ";");
       while(ptrtout != NULL)
         {
            testout[i]=atoi(ptrtout);
            ptrtout = strtok(NULL, ";");
            i++;
         }


        i=0;
        while(strstr(S,"TEST_VECTORS")){//////////////////finds every TEST_VECTORS
            S=strstr(S,"TEST_VECTORS");
            char *ptrttv = strtok(stbetween('\n','#',S) , ";");
            int j=0;
            while(ptrttv != NULL)
             {
                testvector[i][j] =atoi(ptrttv)+1;
                ptrttv = strtok(NULL, ";");
                j++;
             }
             i++;
             S=S+12;
        }
     }
     node=node1;
       return node;


}




void initializeLib(struct Lgate* gNOT,struct Lgate* gNAND,struct Lgate* gNOR,struct Lgate* gXOR,char* Lib){
   char ch;
   FILE *fp;
   fp = fopen(remove_white_spaces(Lib), "r"); // read modet          ///////////////////////////////////////////
   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }                                                     // open and read file
   int i=0;
   char A[1000];
   while((ch = fgetc(fp)) != EOF){
      printf("%c", ch);
      A[i]=ch;
      i++;
      }                                                 //////////////////////////////////////////
    fclose(fp);


   char * S;
   S=A;                           // set a new string S = with the content of file = A

    while(S=strstr(S,"## GATE")){
        S=S+7;
        struct Lgate * x;                                                         /////////// find in which struct this gate belongs
        if(strcmp(remove_white_spaces(stbetween(' ','\n',S)),"NOT")==0){x= gNOT; }
        if(strcmp(remove_white_spaces(stbetween(' ','\n',S)),"NAND_2")==0){x= gNAND;}
        if(strcmp(remove_white_spaces(stbetween(' ','\n',S)),"NOR_2")==0){x= gNOR;}
        if(strcmp(remove_white_spaces(stbetween(' ','\n',S)),"XOR_2")==0){x= gXOR;}

        S=strstr(S,"VCC");
                                                    //// set VCC
        char* str=stbetween(' ',';',S);
        (*x).vccg=atoi(str);

        S=strstr(S,"GND");
                                                    //// set GND
        str=stbetween(' ',';',S);
        (*x).gndg=atoi(str);

       int i=0;
       S=strstr(S,"INPUTS" );      /////// find the inputs
       char *ptrIN = strtok(stbetween('\n','#',S) , ";");
       while(ptrIN != NULL)
         {
            (*x).inpg[i]=atoi(ptrIN);
            ptrIN = strtok(NULL, ";");
            i++;
         }

       i=0;
       S=strstr(S,"OUTPUTS" );      /////// find the outputs
       char *ptrOut = strtok(stbetween('\n','#',S) , ";");
       while(ptrOut != NULL)
       {
          (*x).outg[i]=atoi(ptrOut);
          ptrOut = strtok(NULL, ";");
          i++;
       }

       i=0;
       char* temparr[20]={};
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

        while(temparr[i]!=NULL){                    ///////////////////// find which transistors are PMOS and NMOS and
           char *ptrNet2 = strtok(temparr[i] , " ");///////////////////// the GATE_NODE , P_SOURCE_NODE/N_DRAIN_NODE and
           int j=1;                                  //////////////////// P_DRAIN_NODE/N_SOURCE_NODE
            while(ptrNet2 != NULL)
             {
                if(j==1){
                    ptrNet2 = strtok(NULL, " ");
                    j++;
                }
                else if(j==2){
                    if(strcmp(ptrNet2,"PMOS")==0){
                            (*x).transistor[i]=PMOS;
                            ptrNet2 = strtok(NULL, " ");
                            j++;
                    }
                    else{
                            (*x).transistor[i]=NMOS;
                            ptrNet2 = strtok(NULL, " ");
                            j++;
                    }

                }
                else if(j==3){
                    (*x).netlist[i][0]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                    ptrNet2 = strtok(NULL, " ");
                    j++;
                }
                else if(j==4){
                    (*x).netlist[i][1]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                    ptrNet2 = strtok(NULL, " ");
                    j++;
                }
                 else if(j==5){
                    (*x).netlist[i][2]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                    ptrNet2 = strtok(NULL, " ");
                    j++;
                }
             }
             i++;
        }                                       ////////////////////////////////////////////////////////////////////////////////////

        (*x).sizeofnodeg=max+1;


        (*x).nodeg= (int*)malloc((max+1) * sizeof(int));
        (*x).Helpnodeg= (int*)malloc((max+1) * sizeof(int));
        for(i=0;i<max+1;i++){       /////////// set value of every node = unknown {(X)=2}
            (*x).nodeg[i]=X;
        }

        (*x).nodeg[(*x).vccg ]=VCC;   /////////// set value of every conected node with VCC=1


        (*x).nodeg[(*x).gndg ]=GND;    /////////// set value of every conected node with GND=0



    }
}

int createFinalNetAddMOS(int n1,int isinp1,int n2,int isinp2,int n3,int isinp3,int FinalNet[][3] ,int *offset,int transistorNet[] ,int *numOfInp,int inpNet[] ,int nodeNet[],int MOS,int*flag,int*FinalNetcount,int node0,int node1,int node2){

    if((n1==0 && isinp1==0) || (n2==0 && isinp2==0) || (n3==0 && isinp3==0)){
        *flag=1;
        return 0;
    }
    else{
        transistorNet[*offset]=MOS;
        if(isinp1==1){
            FinalNet[*offset][0]=*FinalNetcount;
            inpNet[(*numOfInp)]=*FinalNetcount;
            nodeNet[*FinalNetcount]=node0;
            *numOfInp=*numOfInp+1;
            *FinalNetcount=*FinalNetcount+1;
        }
        else{
            FinalNet[*offset][0]=n1;
        }
        if(isinp2==1){
            FinalNet[*offset][1]=*FinalNetcount;
            inpNet[(*numOfInp)]=*FinalNetcount;
            nodeNet[*FinalNetcount]=node1;
            *numOfInp=*numOfInp+1;
            *FinalNetcount=*FinalNetcount+1;
        }
        else{
            FinalNet[*offset][1]=n1;
        }
        if(isinp3==1){
            FinalNet[*offset][2]=*FinalNetcount;
            inpNet[(*numOfInp)]=*FinalNetcount;
            nodeNet[*FinalNetcount]=node2;
            *numOfInp=*numOfInp+1;
            *FinalNetcount=*FinalNetcount+1;
            *offset=*offset+1;
            return FinalNet[*offset][2];
        }
        else{
            FinalNet[*offset][2]=n1;
            *offset=*offset+1;
            return FinalNet[*offset][2];
        }

    }

   }


int createFinalNet(int * inp1,int isinp1,int* inp2,int isinp2 ,int FinalNet[][3] ,int *offset,int transistorNet[] ,int * numOfInp,int inpNet[] ,int nodeNet[],struct Lgate g,int *flag,int* FinalNetcount){

    int addtooffset=0;
    if((*inp1==0 && isinp1==0) || (*inp2==0 && isinp2==0)){ //// if the input of gate is not input of the circuit and we dont have
        *flag=1;                                            ////the input node yet we do flag=1 so we can insert the gate later
        return 0;
    }
    else if(isinp1==1 && isinp2==1 && *inp1==0 && *inp2==0){////else we take every case that every input of the gate is input or not

        if(g.inpg[1]==0){        //set the final Input if gate is Not
            inpNet[(*numOfInp)]=g.inpg[0] +*FinalNetcount;
            *inp1=g.inpg[0] +*FinalNetcount;
            *numOfInp=*numOfInp+1;
        }
        else{                    //set the final Input if gate isn't Not
           inpNet[(*numOfInp)]=g.inpg[0] +*FinalNetcount;
           *inp1=g.inpg[0] +*FinalNetcount;
           inpNet[(*numOfInp)+1]=g.inpg[1] +*FinalNetcount;
           *inp2=g.inpg[1] +*FinalNetcount;
           *numOfInp=*numOfInp+2;
        }
        int i=0;
        while(g.netlist[i][0]!=0){  //// for every element of gate's netlist we create equivalent in the final netlist
           FinalNet[*offset+i][0]=g.netlist[i][0]+*FinalNetcount;
           FinalNet[*offset+i][1]=g.netlist[i][1]+*FinalNetcount;
           FinalNet[*offset+i][2]=g.netlist[i][2]+*FinalNetcount;
           transistorNet[*offset+i]=g.transistor[i];
           addtooffset=addtooffset+1;
           i++;
        }
        for(i=1;i<g.sizeofnodeg;i++){   ////create and set values of nodes in final cirquit
            nodeNet[i+*FinalNetcount]=g.nodeg[i];
        }
        *FinalNetcount=g.sizeofnodeg-1+*FinalNetcount;
        *offset=(*offset)+addtooffset;
        return g.outg[0] +(*FinalNetcount)-g.sizeofnodeg+1;
    }
    else if(isinp1==1 && isinp2==1 && *inp1!=0 && *inp2==0){////similarly we do for the rest of cases

       inpNet[(*numOfInp)+1]=g.inpg[1] +*FinalNetcount;
       *inp2=g.inpg[1] +*FinalNetcount;
       *numOfInp=*numOfInp+1;

        int i=0;
        while(g.netlist[i][0]!=0){
           if(g.netlist[i][0]==g.inpg[0]){
            FinalNet[*offset+i][0]=*inp1;
           }
           else{
           FinalNet[*offset+i][0]=g.netlist[i][0]+*FinalNetcount;
           }
           if(g.netlist[i][1]==g.inpg[0]){
            FinalNet[*offset+i][1]=*inp1;
           }
           else{
           FinalNet[*offset+i][1]=g.netlist[i][1]+*FinalNetcount;
           }
           if(g.netlist[i][2]==g.inpg[0]){
            FinalNet[*offset+i][2]=*inp1;
           }
           else{
           FinalNet[*offset+i][2]=g.netlist[i][2]+*FinalNetcount;
           }
           transistorNet[*offset+i]=g.transistor[i];
           addtooffset=addtooffset+1;
           i++;
        }
        for(i=1;i<g.sizeofnodeg;i++){
            nodeNet[i+*FinalNetcount]=g.nodeg[i];
        }
        *FinalNetcount=g.sizeofnodeg-1+*FinalNetcount;
        *offset=(*offset)+addtooffset;
        return g.outg[0] +(*FinalNetcount)-g.sizeofnodeg+1;
    }
    else if(isinp1==1 && isinp2==1 && *inp1==0 && *inp2!=0){

       inpNet[(*numOfInp)+1]=g.inpg[0] +*FinalNetcount;
       *inp1=g.inpg[0] +*FinalNetcount;
       *numOfInp=*numOfInp+1;

        int i=0;
        while(g.netlist[i][0]!=0){
           if(g.netlist[i][0]==g.inpg[1]){
            FinalNet[*offset+i][0]=*inp2;
           }
           else{
           FinalNet[*offset+i][0]=g.netlist[i][0]+*FinalNetcount;
           }
           if(g.netlist[i][1]==g.inpg[1]){
            FinalNet[*offset+i][1]=*inp2;
           }
           else{
           FinalNet[*offset+i][1]=g.netlist[i][1]+*FinalNetcount;
           }
           if(g.netlist[i][2]==g.inpg[1]){
            FinalNet[*offset+i][2]=*inp2;
           }
           else{
           FinalNet[*offset+i][2]=g.netlist[i][2]+*FinalNetcount;
           }
           transistorNet[*offset+i]=g.transistor[i];
           addtooffset=addtooffset+1;
           i++;
        }
        for(i=1;i<g.sizeofnodeg;i++){
            nodeNet[i+*FinalNetcount]=g.nodeg[i];
        }
        *FinalNetcount=g.sizeofnodeg-1+*FinalNetcount;
        *offset=(*offset)+addtooffset;
        return g.outg[0] +(*FinalNetcount)-g.sizeofnodeg+1;
    }
    else if(isinp1==1 && isinp2==1 && *inp1!=0 && *inp2!=0){

        int i=0;
        while(g.netlist[i][0]!=0){
           if(g.netlist[i][0]==g.inpg[1]){
            FinalNet[*offset+i][0]=*inp2;
           }
           else if(g.netlist[i][0]==g.inpg[0]){
            FinalNet[*offset+i][0]=*inp1;
           }
           else{
           FinalNet[*offset+i][0]=g.netlist[i][0]+*FinalNetcount;
           }
           if(g.netlist[i][1]==g.inpg[1]){
            FinalNet[*offset+i][1]=*inp2;
           }
           else if(g.netlist[i][1]==g.inpg[0]){
            FinalNet[*offset+i][1]=*inp1;
           }
           else{
           FinalNet[*offset+i][1]=g.netlist[i][1]+*FinalNetcount;
           }
           if(g.netlist[i][2]==g.inpg[1]){
            FinalNet[*offset+i][2]=*inp2;
           }
           else if(g.netlist[i][2]==g.inpg[0]){
            FinalNet[*offset+i][2]=*inp1;
           }
           else{
           FinalNet[*offset+i][2]=g.netlist[i][2]+*FinalNetcount;
           }
           transistorNet[*offset+i]=g.transistor[i];
           addtooffset=addtooffset+1;
           i++;
        }
        for(i=1;i<g.sizeofnodeg;i++){
            nodeNet[i+*FinalNetcount]=g.nodeg[i];
        }
        *FinalNetcount=g.sizeofnodeg-1+*FinalNetcount;
        *offset=(*offset)+addtooffset;
        return g.outg[0] +(*FinalNetcount)-g.sizeofnodeg+1;
    }
    else if(isinp1==0 && isinp2==1 && *inp2==0){
        inpNet[(*numOfInp)]=g.inpg[1]+*FinalNetcount;
        *inp2=g.inpg[1]+*FinalNetcount;
        *numOfInp=*numOfInp+1;
        int i=0;
        while(g.netlist[i][0]!=0){
           if(g.netlist[i][0]==g.inpg[0]){
               FinalNet[*offset+i][0]=*inp1;
           }
           else{
               FinalNet[*offset+i][0]=g.netlist[i][0]+*FinalNetcount;
           }
           if(g.netlist[i][1]==g.inpg[0]){
               FinalNet[*offset+i][1]=*inp1;
           }
           else{
               FinalNet[*offset+i][1]=g.netlist[i][1]+*FinalNetcount;
           }
            if(g.netlist[i][2]==g.inpg[0]){
               FinalNet[*offset+i][2]=*inp1;
           }
           else{
               FinalNet[*offset+i][2]=g.netlist[i][2]+*FinalNetcount;
           }
           transistorNet[*offset+i]=g.transistor[i];
           addtooffset=addtooffset+1;
           i++;
        }
         for(i=1;i<g.sizeofnodeg;i++){
            nodeNet[i+*FinalNetcount]=g.nodeg[i];
        }
        *FinalNetcount=g.sizeofnodeg-1+*FinalNetcount;
        *offset=(*offset)+addtooffset;
        return g.outg[0] +(*FinalNetcount)-g.sizeofnodeg+1;

    }
    else if(isinp1==0 && isinp2==1 && *inp2!=0){
        int i=0;
        while(g.netlist[i][0]!=0){
           if(g.netlist[i][0]==g.inpg[0]){
               FinalNet[*offset+i][0]=*inp1;
           }
           else if(g.netlist[i][0]==g.inpg[1]){
               FinalNet[*offset+i][0]=*inp2;
           }
           else{
               FinalNet[*offset+i][0]=g.netlist[i][0]+*FinalNetcount;
           }
           if(g.netlist[i][1]==g.inpg[0]){
               FinalNet[*offset+i][1]=*inp1;
           }
           else if(g.netlist[i][1]==g.inpg[1]){
               FinalNet[*offset+i][1]=*inp2;
           }
           else{
               FinalNet[*offset+i][1]=g.netlist[i][1]+*FinalNetcount;
           }
            if(g.netlist[i][2]==g.inpg[0]){
               FinalNet[*offset+i][2]=*inp1;
           }
           else if(g.netlist[i][2]==g.inpg[1]){
               FinalNet[*offset+i][2]=*inp2;
           }
           else{
               FinalNet[*offset+i][2]=g.netlist[i][2]+*FinalNetcount;
           }
           transistorNet[*offset+i]=g.transistor[i];
           addtooffset=addtooffset+1;
           i++;
        }
         for(i=1;i<g.sizeofnodeg;i++){
            nodeNet[i+*FinalNetcount]=g.nodeg[i];
        }
        *FinalNetcount=g.sizeofnodeg-1+*FinalNetcount;
        *offset=(*offset)+addtooffset;
        return g.outg[0] +(*FinalNetcount)-g.sizeofnodeg+1;

    }
    else if(isinp1==1 && isinp2==0 && *inp1==0){
        inpNet[(*numOfInp)]=g.inpg[0]+*FinalNetcount;
        *inp1=g.inpg[0]+*FinalNetcount;
        *numOfInp=*numOfInp+1;
        int i=0;
        while(g.netlist[i][0]!=0){
           if(g.netlist[i][0]==g.inpg[1]){
               FinalNet[*offset+i][0]=*inp2;
           }
           else{
               FinalNet[*offset+i][0]=g.netlist[i][0]+*FinalNetcount;
           }
           if(g.netlist[i][1]==g.inpg[1]){
               FinalNet[*offset+i][1]=*inp2;
           }
           else{
               FinalNet[*offset+i][1]=g.netlist[i][1]+*FinalNetcount;
           }
            if(g.netlist[i][2]==g.inpg[1]){
               FinalNet[*offset+i][2]=*inp2;
           }
           else{
               FinalNet[*offset+i][2]=g.netlist[i][2]+*FinalNetcount;
           }
           transistorNet[*offset+i]=g.transistor[i];
           addtooffset=addtooffset+1;
           i++;
        }
         for(i=1;i<g.sizeofnodeg;i++){
            nodeNet[i+*FinalNetcount]=g.nodeg[i];
        }
        *FinalNetcount=g.sizeofnodeg-1+*FinalNetcount;
        *offset=(*offset)+addtooffset;
        return g.outg[0] +(*FinalNetcount)-g.sizeofnodeg+1;

    }
    else if(isinp1==1 && isinp2==0 && *inp1!=0){
        int i=0;
        while(g.netlist[i][0]!=0){
           if(g.netlist[i][0]==g.inpg[1]){
               FinalNet[*offset+i][0]=*inp2;
           }
           else if(g.netlist[i][0]==g.inpg[0]){
               FinalNet[*offset+i][0]=*inp1;
           }
           else{
               FinalNet[*offset+i][0]=g.netlist[i][0]+*FinalNetcount;
           }
           if(g.netlist[i][1]==g.inpg[1]){
               FinalNet[*offset+i][1]=*inp2;
           }
           else if(g.netlist[i][1]==g.inpg[0]){
               FinalNet[*offset+i][1]=*inp1;
           }
           else{
               FinalNet[*offset+i][1]=g.netlist[i][1]+*FinalNetcount;
           }
            if(g.netlist[i][2]==g.inpg[1]){
               FinalNet[*offset+i][2]=*inp2;
           }
           else if(g.netlist[i][2]==g.inpg[0]){
               FinalNet[*offset+i][2]=*inp1;
           }
           else{
               FinalNet[*offset+i][2]=g.netlist[i][2]+*FinalNetcount;
           }
           transistorNet[*offset+i]=g.transistor[i];
           addtooffset=addtooffset+1;
           i++;
        }
         for(i=1;i<g.sizeofnodeg;i++){
            nodeNet[i+*FinalNetcount]=g.nodeg[i];
        }
        *FinalNetcount=g.sizeofnodeg-1+*FinalNetcount;
        *offset=(*offset)+addtooffset;
        return g.outg[0] +(*FinalNetcount)-g.sizeofnodeg+1;

    }
    else if(isinp1==0 && isinp2==0){
        int i=0;
        while(g.netlist[i][0]!=0){
           if(g.netlist[i][0]==g.inpg[0]){
               FinalNet[*offset+i][0]=*inp1;
           }
           else if(g.netlist[i][0]==g.inpg[1]){
               FinalNet[*offset+i][0]=*inp2;
           }
           else{
               FinalNet[*offset+i][0]=g.netlist[i][0]+*FinalNetcount;
           }
           if(g.netlist[i][1]==g.inpg[0]){
               FinalNet[*offset+i][1]=*inp1;
           }
           else if(g.netlist[i][1]==g.inpg[1]){
               FinalNet[*offset+i][1]=*inp2;
           }
           else{
               FinalNet[*offset+i][1]=g.netlist[i][1]+*FinalNetcount;
           }
           if(g.netlist[i][2]==g.inpg[0]){
               FinalNet[*offset+i][2]=*inp1;
           }
           else if(g.netlist[i][2]==g.inpg[1]){
               FinalNet[*offset+i][2]=*inp2;
           }
           else{
               FinalNet[*offset+i][2]=g.netlist[i][2]+*FinalNetcount;
           }
           transistorNet[*offset+i]=g.transistor[i];
           addtooffset=addtooffset+1;
           i++;
        }
         for(i=1;i<g.sizeofnodeg;i++){
            nodeNet[i+*FinalNetcount]=g.nodeg[i];
        }
        *FinalNetcount=g.sizeofnodeg-1+*FinalNetcount;
        *offset=(*offset)+addtooffset;
        return g.outg[0] +(*FinalNetcount)-g.sizeofnodeg+1;
    }


}

void findPossibleBreak(int possibleBreak[], int FinalNet[][3]){
    int PBgate[500]={};
    int i=0;
    int k=0;
    while(FinalNet[i][0]!=0){ //finds the difference node number for every transistor's gate
        int isInside=0;
        int j=0;
        while(PBgate[j]!=0){
            if(FinalNet[i][0]==PBgate[j]){isInside=1;}
            j++;
        }
        if(isInside==0){
            PBgate[k]=FinalNet[i][0];
            k++;
        }

        i++;

    }

    int j=0;
    k=0;
    while(PBgate[j]!=0){  // finds which nodes are conected to a sorce and drain and gate of some transistors and set the possible breaks
        int  count1=0;
        int  count2=0;
        int i=0;
        while(FinalNet[i][0]!=0){
            if(PBgate[j]==FinalNet[i][1]){
                count1++;
            }
            if(PBgate[j]==FinalNet[i][2]){
                count2++;
            }
            i++;
        }
        if(count1>=1 && count2>=1){
            possibleBreak[k]=PBgate[j];
            k++;
        }
        j++;
    }

}

struct Graph* createGraph(int FinalNet[][3] ,int breakNode ){

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
        newNode->coloure=0;
        for (int j = 0; j < 14; j++) {
        newNode->next[j] = NULL;
        }
        newNode->nodenumber=i+1;
        graph->head[i]=newNode;
    }

    struct GraphNode* newNode = (struct GraphNode*)malloc(sizeof(struct GraphNode));  // create the extra node with diferend coloure which
        newNode->coloure=0;                                                           // will be conected with the break point of circuit
        for (int j = 0; j < 14; j++) {
            newNode->next[j] = NULL;
        }
        newNode->nodenumber=-breakNode;
        graph->head[max]=newNode;

    i=0;
    int found=0;
    while(FinalNet[i][0]!=0){////////////////////////////// create the eadges between all nodes
        int isconected1=0;
        int isconected2=0;
        int j=0;
        if(FinalNet[i][0]==breakNode && found==0){ //if the node we will conect now is the possible break point we conect it only with the extra helping
            found=1;                               // node and we replace its position with the extra node so it will do the right conections
            graph->head[FinalNet[i][0]-1]->next[0]=graph->head[max];
            graph->head[FinalNet[i][0]-1]->coloure=pink;
            graph->head[max]->coloure=blue;
            struct GraphNode* helpNode=graph->head[FinalNet[i][0]-1];
            graph->head[FinalNet[i][0]-1]=graph->head[max];
            graph->head[max]=helpNode;
        }

        while( graph->head[FinalNet[i][0]-1]->next[j]!=NULL){
            if(graph->head[FinalNet[i][0]-1]->next[j]==graph->head[FinalNet[i][1]-1]){//check if the node is alredy conected with adj nodes
                isconected1=1;
            }
            if(graph->head[FinalNet[i][0]-1]->next[j]==graph->head[FinalNet[i][2]-1]){
                isconected2=1;
            }
            j++;
        }
        if(isconected1==0){ ///// conecte the node with the adj nodes

            if(found==1 && FinalNet[i][1]==breakNode && FinalNet[i][1]> FinalNet[i][0]){//check if the breakNode is found but we have not conect it us an output to the subcircuit
                int point=0;
                while(graph->head[FinalNet[i][0]-1]->next[point]!=NULL) {point++;}
                graph->head[FinalNet[i][0]-1]->next[point]=graph->head[max];
                int point1=0;
                while(graph->head[max]->next[point1]!=NULL) {point1++;}
                graph->head[max]->next[point1]=graph->head[FinalNet[i][0]-1];
            }
            else{
                int point=0;
                while(graph->head[FinalNet[i][0]-1]->next[point]!=NULL) {point++;}
                graph->head[FinalNet[i][0]-1]->next[point]=graph->head[FinalNet[i][1]-1];
                int point1=0;
                while(graph->head[FinalNet[i][1]-1]->next[point1]!=NULL) {point1++;}
                graph->head[FinalNet[i][1]-1]->next[point1]=graph->head[FinalNet[i][0]-1];
            }
        }
        if(isconected2==0){//// conecte the node with the adj nodes
            if(found==1 && FinalNet[i][2]==breakNode && FinalNet[i][2]> FinalNet[i][0]){//check if the breakNode is found but we have not conect it us an output to the subcircuit
                int point=0;
                while(graph->head[FinalNet[i][0]-1]->next[point]!=NULL) {point++;}
                graph->head[FinalNet[i][0]-1]->next[point]=graph->head[max];
                int point1=0;
                while(graph->head[max]->next[point1]!=NULL) {point1++;}
                graph->head[max]->next[point1]=graph->head[FinalNet[i][0]-1];
            }
            else{
                int point=0;
                while(graph->head[FinalNet[i][0]-1]->next[point]!=NULL) {point++;}
                graph->head[FinalNet[i][0]-1]->next[point]=graph->head[FinalNet[i][2]-1];
                int point1=0;
                while(graph->head[FinalNet[i][2]-1]->next[point1]!=NULL) {point1++;}
                graph->head[FinalNet[i][2]-1]->next[point1]=graph->head[FinalNet[i][0]-1];
            }
        }
        i++;
    }//////////////////////////////////////////////////////////////////////////////////////////////

    return graph;
}



int  colorGraph(struct Graph* gr,int pbreak,int pinkNode[] ,int blueNode[] ,int *numofpinknode,int*numofbluenode){

    pinkNode[0]=pbreak;
    blueNode[0]=pbreak;
    int countForPink=1;
    int countForBlue=1;

    int flag=0;
    while(flag==0){////until nothing happens so we know we have finished
        flag=1;
        int i=0;
        while(gr->head[i] !=NULL ){ //// for every node in graph

            if(gr->head[i]->coloure==pink){//draw the adj nodes if they are not the possible break point node
                int j=0;
                while(gr->head[i]->next[j]!=0){
                     if(!(gr->head[i]->nodenumber==pbreak && gr->head[i]->next[j]->nodenumber==-pbreak) ){
                        if(gr->head[i]->next[j]->coloure==0){
                            gr->head[i]->next[j]->coloure=pink;
                            flag=0;
                            pinkNode[countForPink]=gr->head[i]->next[j]->nodenumber;
                            countForPink++;
                        }
                        if(gr->head[i]->next[j]->coloure==blue){//if the coloures are mixed then the possible break node is not a valid break point

                            return 0;
                        }
                     }
                    j++;
                }
            }
            if(gr->head[i]->coloure==blue){//draw the adj nodes if they are not the possible break point node
                int j=0;
                while(gr->head[i]->next[j]!=0){
                     if(!(gr->head[i]->nodenumber==pbreak && gr->head[i]->next[j]->nodenumber==-pbreak) ){
                        if(gr->head[i]->next[j]->coloure==0){
                            gr->head[i]->next[j]->coloure=blue;
                            flag=0;
                            blueNode[countForBlue]=gr->head[i]->next[j]->nodenumber;
                            countForBlue++;
                        }
                        if(gr->head[i]->next[j]->coloure==pink){//if the coloures are mixed then the possible break node is not a valid break point

                            return 0;
                        }
                     }
                    j++;
                }
            }
            i++;
        }
    }

    *numofbluenode=countForBlue;
    *numofpinknode=countForPink;


    return 1;

}



void  separateTheInpFileTo2(char** nameOfFile1,char** nameOfFile2 ,int FinalNet[][3] ,int transistorNet[] ,int inpNet[] ,int OutOfNet[] ,int testInpNet[] ,int nodeNet[] ,int bestbreak,int numofnodes ,int pinkNode[],int blueNode[]){

       FILE *fptr1;
       FILE *fptr2;
       // use appropriate location if you are using MacOS or Linux
       fptr1 = fopen("NEWFILE1.txt","w");
       fptr2 = fopen("NEWFILE2.txt","w");

       *nameOfFile1="NEWFILE1.txt","w";
       *nameOfFile2="NEWFILE2.txt","w";

       if(fptr1 == NULL)
       {
          printf("Error!");
          exit(1);
       }
       if(fptr2 == NULL)
       {
          printf("Error!");
          exit(1);
       }

       int i=0;
       fprintf(fptr1,"##RAILS\nVCC");

       while(pinkNode[i]!=0){   //put all VCC in file1
          if(nodeNet[pinkNode[i]]==1){
             fprintf(fptr1," %d ;",pinkNode[i]);
          }
          i++;
       }

       fprintf(fptr1,"\nGND");
       i=0;
       while(nodeNet[pinkNode[i]]!=0){ //put the first GND in file1
        i++;
       }
       fprintf(fptr1," %d",pinkNode[i]);
       i++;

       while(pinkNode[i]!=0){ // putthe rest of GND in file1
          if(nodeNet[pinkNode[i]]==0){
             fprintf(fptr1," ; %d",pinkNode[i]);
          }
          i++;
       }

        i=0;
       fprintf(fptr2,"##RAILS\nVCC");

       while(blueNode[i]!=0){   //put all VCC in file2
          if(nodeNet[blueNode[i]]==1){
             fprintf(fptr2," %d ;",blueNode[i]);
          }
          i++;
       }

       fprintf(fptr2,"\nGND");
       i=0;
       while(nodeNet[blueNode[i]]!=0){ //put the first GND in file2
        i++;
       }
       fprintf(fptr2," %d",blueNode[i]);
       i++;

       while(blueNode[i]!=0){ // put the rest of GND in file2
          if(nodeNet[blueNode[i]]==0){
             fprintf(fptr2," ; %d",blueNode[i]);
          }
          i++;
       }

      fprintf(fptr1,"\n## INPUTS\n");
      i=0;
      int foundfirst=0;
      while(pinkNode[i]!=0){  // put all inputs in file1
         int j=0;
         while(inpNet[j]!=0) {
            if(pinkNode[i]==inpNet[j]&&foundfirst==0){
               fprintf(fptr1,"%d",pinkNode[i]) ;
               foundfirst=1;
            }
            else if(pinkNode[i]==inpNet[j]){
               fprintf(fptr1," ; %d",pinkNode[i]) ;
            }
            j++;
         }
        i++;
      }


      fprintf(fptr2,"\n## INPUTS\n");
      fprintf(fptr2,"%d",bestbreak);// put first input in file2
      i=0;
      while(blueNode[i]!=0){ // put the rest of inputs in file2
         int j=0;
         while(inpNet[j]!=0) {
            if(blueNode[i]==inpNet[j]){
               fprintf(fptr2," ; %d",blueNode[i]) ;
            }
            j++;
         }
        i++;
      }


      fprintf(fptr1,"\n## OUTPUTS\n");
      i=0;
      foundfirst=0;
      while(pinkNode[i]!=0){  // put all outputs in file1 except last
         int j=0;
         while(OutOfNet[j]!=0) {
            if(pinkNode[i]==OutOfNet[j]&&foundfirst==0){
               fprintf(fptr1,"%d",pinkNode[i]) ;
               foundfirst=1;
            }
            else if(pinkNode[i]==OutOfNet[j]){
               fprintf(fptr1," ; %d",pinkNode[i]) ;
            }
            j++;
         }
        i++;
        }
        fprintf(fptr1," ; %d",bestbreak) ; //put final output of file1


      fprintf(fptr2,"\n## OUTPUTS\n");
      i=0;
      foundfirst=0;
      while(blueNode[i]!=0){ // put all outputs in file2
         int j=0;
         while(OutOfNet[j]!=0) {
            if(foundfirst==0 && blueNode[i]==OutOfNet[j]) {
                fprintf(fptr2,"%d",blueNode[i]) ;
                foundfirst=1;
            }
            else if(blueNode[i]==OutOfNet[j]){
               fprintf(fptr2," ; %d",blueNode[i]) ;
            }
            j++;
         }
        i++;
      }


      fprintf(fptr1,"\n## NETLIST\n");

      i=0;
      int numoftransis=0;
      while(FinalNet[i][0]!=0){  // put netlist in file1
            int j=0;
            int isinpink1=0;
            int isinpink2=0;
            while(pinkNode[j]!=0){ //check if the nodes of netlist is pink
                if(FinalNet[i][1]==pinkNode[j]){isinpink1=1;}
                if(FinalNet[i][2]==pinkNode[j]){isinpink2=1;}
                j++;
            }
            if(isinpink1==1&&isinpink2==1){
                fprintf(fptr1,"U%d ",numoftransis) ;
                if(transistorNet[i]==NMOS){fprintf(fptr1,"NMOS ");}
                else if(transistorNet[i]==PMOS){fprintf(fptr1,"PMOS ");}
                fprintf(fptr1,"%d ",FinalNet[i][0]);
                fprintf(fptr1,"%d ",FinalNet[i][1]);
                fprintf(fptr1,"%d\n",FinalNet[i][2]);
                numoftransis++;
            }
            i++;
      }

      fprintf(fptr1,"## END_SIMULATION");



      fprintf(fptr2,"\n## NETLIST\n");
      i=0;
      numoftransis=0;
      while(FinalNet[i][0]!=0){ // put netlist in file2
            int j=0;
            int isinblue1=0;
            int isinblue2=0;
            while(blueNode[j]!=0){//check if the nodes of netlist is blue
                if(FinalNet[i][1]==blueNode[j]){isinblue1=1;}
                if(FinalNet[i][2]==blueNode[j]){isinblue2=1;}
                j++;
            }
            if(isinblue1==1&&isinblue2==1){
                fprintf(fptr2,"U%d ",numoftransis) ;
                if(transistorNet[i]==NMOS){fprintf(fptr2,"NMOS ");}
                else if(transistorNet[i]==PMOS){fprintf(fptr2,"PMOS ");}
                fprintf(fptr2,"%d ",FinalNet[i][0]);
                fprintf(fptr2,"%d ",FinalNet[i][1]);
                fprintf(fptr2,"%d\n",FinalNet[i][2]);
                numoftransis++;
            }
            i++;
      }

      fprintf(fptr2,"## END_SIMULATION");


       fclose(fptr1);
       fclose(fptr2);

}



int main()
{

    char* Lib=NULL;

    int temp=1;
    int inp[50]={};           /////////////  define the arrays we need to store the information from file
    int testinp[50]={};
    int out[50]={};
    int testout[50]={};
    int netlist[100][3]={};
    int logicgate[100]={};
    int testvector[100][50]={};
    int* sizeofnode=&temp;
    int* node;                  //////////////


    node = initialization(inp,testinp,testout,out,netlist,testvector,logicgate,node,sizeofnode,&Lib,"file2.txt");  ////it stores the information from file in the appropriate array


    struct Lgate gNOT={{},{},0,0,{},{},0,{},0,0,0}; ////structures that store iformation for every gate from Library
    struct Lgate gNAND={{},{},0,0,{},{},0,{},0,0,0};
    struct Lgate gNOR={{},{},0,0,{},{},0,{},0,0,0};
    struct Lgate gXOR={{},{},0,0,{},{},0,{},0,0,0};////



    initializeLib(&gNOT,&gNAND,&gNOR,&gXOR,Lib);    ////set properties for every Lgate = information from Library


    int netlistOut[1000]={};
    int FinalNet[1000][3] ={};//// this is the final netlist
    int FinalNetcount=0;
    int offset=0;
    int transistorNet[1000] ={};////this is the transistor array for every row of final netlist
    int inpNet[100]={};////this is the input of the final netlist
    int numOfInp=0;
    int nodeNet[1000]={};////this is the array that stores the value of every node
    int matchGate[100][2]={};


    int flag=1;
    int IsInList[500] ={};

    while(flag==1){  //// loop we need in case that the gates are not with the corect order in the starting netlist
        flag=0;
            int c=0;
            while(netlist[c][0]!= 0){  ////Loop for every gate
                if(IsInList[c]==0) {
                    int isinp1=0;
                    int isinp2=0;
                    int isinp3=0;
                    int a=0;
                    while(inp[a]!=0){     ////find if the inputs of the gate is in input of circuit
                        if(netlist[c][0]==testinp[a]){isinp1=1;}
                        if(netlist[c][1]==testinp[a]){isinp2=1;}
                        if(netlist[c][2]==testinp[a]){isinp3=1;}
                        a++;
                    }
                    if(logicgate[c]==NOT){ //// for every case of Lgate call the function createFinalNet or createFinalNetAddMOS to add the gate or transistor to the final netlist
                    netlistOut[netlist[c][2]]=createFinalNet(&netlistOut[netlist[c][0]],isinp1,&netlistOut[netlist[c][1]],isinp2 ,FinalNet,&offset,transistorNet,&numOfInp,inpNet,nodeNet,gNOT,&flag,&FinalNetcount);
                    if(netlistOut[netlist[c][2]]!=0){IsInList[c]=1;} // in case of flag we should not put again the gate in netlist
                    matchGate[c][0]=NOT;// we need this to find sort circuit
                    matchGate[c][1]=FinalNetcount;// we need this to find sort circuit
                    }
                    else if(logicgate[c]==NOR_2){
                    netlistOut[netlist[c][2]]=createFinalNet(&netlistOut[netlist[c][0]],isinp1,&netlistOut[netlist[c][1]],isinp2,FinalNet,&offset,transistorNet,&numOfInp,inpNet,nodeNet,gNOR,&flag,&FinalNetcount);
                    if(netlistOut[netlist[c][2]]!=0){IsInList[c]=1;}
                    matchGate[c][0]=NOR_2;
                    matchGate[c][1]=FinalNetcount;
                    }
                    else if(logicgate[c]==NAND_2){
                    netlistOut[netlist[c][2]]=createFinalNet(&netlistOut[netlist[c][0]],isinp1,&netlistOut[netlist[c][1]],isinp2,FinalNet,&offset,transistorNet,&numOfInp,inpNet,nodeNet,gNAND,&flag,&FinalNetcount);
                    if(netlistOut[netlist[c][2]]!=0){IsInList[c]=1; }
                    matchGate[c][0]=NAND_2;
                    matchGate[c][1]=FinalNetcount;
                    }
                    else if(logicgate[c]==XOR_2){
                    netlistOut[netlist[c][2]]=createFinalNet(&netlistOut[netlist[c][0]],isinp1,&netlistOut[netlist[c][1]],isinp2,FinalNet,&offset,transistorNet,&numOfInp,inpNet,nodeNet,gXOR,&flag,&FinalNetcount);
                    if(netlistOut[netlist[c][2]]!=0){IsInList[c]=1;}
                    matchGate[c][0]=XOR_2;
                    matchGate[c][1]=FinalNetcount;
                    }
                    else if(logicgate[c]==PMOS){
                    netlistOut[netlist[c][2]]=createFinalNetAddMOS(netlistOut[netlist[c][0]],isinp1,netlistOut[netlist[c][1]],isinp2,netlistOut[netlist[c][2]],isinp3,FinalNet,&offset,transistorNet,&numOfInp,inpNet,nodeNet,PMOS,&flag,&FinalNetcount,node[netlist[c][0]],node[netlist[c][1]],node[netlist[c][2]]);
                    if(netlistOut[netlist[c][1]]==0){netlistOut[netlist[c][1]]=FinalNetcount-2;}
                    if(netlistOut[netlist[c][0]]==0){netlistOut[netlist[c][1]]=FinalNetcount-3;}
                    if(netlistOut[netlist[c][2]]!=0){IsInList[c]=1;}
                    matchGate[c][0]=PMOS;
                    matchGate[c][1]=FinalNetcount;
                    }
                    else if(logicgate[c]==NMOS){
                    netlistOut[netlist[c][2]]=createFinalNetAddMOS(netlistOut[netlist[c][0]],isinp1,netlistOut[netlist[c][1]],isinp2,netlistOut[netlist[c][2]],isinp3,FinalNet,&offset,transistorNet,&numOfInp,inpNet,nodeNet,NMOS,&flag,&FinalNetcount,node[netlist[c][0]],node[netlist[c][1]],node[netlist[c][2]]);
                    if(netlistOut[netlist[c][1]]==0){netlistOut[netlist[c][1]]=FinalNetcount-2;}
                    if(netlistOut[netlist[c][0]]==0){netlistOut[netlist[c][1]]=FinalNetcount-3;}
                    if(netlistOut[netlist[c][2]]!=0){IsInList[c]=1;}
                    matchGate[c][0]=NMOS;
                    matchGate[c][1]=FinalNetcount;
                    }
             }
             c++;
         }
    }


    int i=0;
     int OutOfNet[100]={} ;      //////////////// sets the outputs of the new netlist
     while(testout[i]!=0){
       OutOfNet[i]= netlistOut[testout[i]];
       i++;
    }

    int testInpNet[100]={};     ////////////// sets the test_Inputs of the new netlist
    i=0;
    while(testinp[i]!=0){
        int j=0;
        while (inp[j] !=0){
            if(testinp[i]==inp[j] ){
                testInpNet[i]=inpNet[j];
            }
            j++;
        }
        i++;
      }


      int possibleBreak[500]={};

      findPossibleBreak(possibleBreak,FinalNet);////finds the the nodes which are possible to be a good point to break the circuit

      int mycount=0;
      int validBreak[50]={};
      int validBreakcount=0;
      int numbOfNodes=0;
      int accuracyofvalidBreak[50]={};
      int pinkNode[1000]={};
      int blueNode[1000]={};
      int numofpinknode=0;
      int numofbluenode=0;

      while(possibleBreak[mycount]!=0){// for every possible break
        struct Graph* gr= createGraph(FinalNet,possibleBreak[mycount]);//create the graph given the possible break
        int validBreakCheck=0;
        validBreakCheck=colorGraph(gr,possibleBreak[mycount],pinkNode,blueNode,&numofpinknode,&numofbluenode);//draw every node and decides if the coloures are mixed or not
        if(validBreakCheck==1){//if coloures are not mixed
            validBreak[validBreakcount]=possibleBreak[mycount];//this possible break is a good enough break
            accuracyofvalidBreak[validBreakcount]=abs(2*possibleBreak[mycount]-FinalNetcount);//set how good is the current break point
            validBreakcount++;
        }
        mycount++;
      }


      i=0;
      while(validBreak[i]!=0){//print all good points for break
        printf("the break at the node %d is valid\n ",validBreak[i]);
        i++;
      }

      i=1;
      int min=accuracyofvalidBreak[0];
      int bestbreak=validBreak[i];
      while(accuracyofvalidBreak[i]!=0){ // finds the best break point
        if(accuracyofvalidBreak[i]<min){
            min=accuracyofvalidBreak[i];
            bestbreak=validBreak[i];
        }
        i++;
      }
      printf("the best break is at node %d \n",bestbreak);

      int FinalPinkNode[1000] ={};
      int FinalBlueNode[1000]={};
      struct Graph* gr= createGraph(FinalNet,bestbreak);// run these functions ones more to create the blue and pink node corectly for the best break
      int useless =colorGraph(gr,bestbreak,FinalPinkNode,FinalBlueNode,&numofpinknode,&numofbluenode);
      selectionSort(FinalPinkNode,numofpinknode) ; //sort pink nodes
      selectionSort(FinalBlueNode,numofbluenode); //sort blue nodes


      char* nameOfFile[2] ={};


      separateTheInpFileTo2(&nameOfFile[0] ,&nameOfFile[1] ,FinalNet,transistorNet,inpNet,OutOfNet,testInpNet,nodeNet,bestbreak,FinalNetcount,FinalPinkNode,FinalBlueNode);// create the 2 files


     for(int numOfFile=0;numOfFile<2;numOfFile++){
          memset(FinalNet, 0, sizeof(FinalNet) );
          memset(transistorNet, 0, sizeof(transistorNet) );
          memset(inpNet, 0, sizeof(inpNet) );
          memset(OutOfNet, 0, sizeof(OutOfNet) );
          sizeofnode=&temp;
          int *FinalNodeNet;

          FinalNodeNet=initialization(inpNet,testinp,testout,OutOfNet,FinalNet,testvector,transistorNet,FinalNodeNet,sizeofnode,&Lib,nameOfFile[numOfFile] );//initializate the new file

          int manualtestvector[6][9]={{1,1,1,1,1,1,1,1,1},{2,1,2,1,2,1,2,1,2},{1,1,2,1,2,1,2,1,2},{1,2,1,1,2,2,2,2,2},{2,2,2,2,2,2,2,2,2},{0,0,0,0,0,0,0,0,0}};//2means1 and 1means0
         i=0;
         while(manualtestvector[i][0]!=0){ //for every test vector
            int j=0;
            int newNode[1000]={};
            while(j<*sizeofnode){   //////////// copies all nodes to a new array so we can work with it
                newNode[j]=FinalNodeNet[j];
                j++;
            }
            j=0;
            while(inpNet[j]!=0){   ///////////////////  set the values of input nodes
                  newNode[inpNet[j]]= manualtestvector[i][j]-1;
                  j++;
                  }

            int check = 1;
            while(check==1){   ///////// run until nothing changed so we know that the algorithm has finished
                check=0;
                j=0;
                while(FinalNet[j][0]!=0){  ////run 1 1 time for every transistor

                    if(transistorNet[j]==PMOS && newNode[FinalNet[j][0]]==0 ){  ////what we do when the transistor is PMOS and the gate is 0
                        if(newNode[FinalNet[j][1]]==1 &&  newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=1;check=1;}
                        else if(newNode[FinalNet[j][1]]==1 &&  newNode[FinalNet[j][2]]==Z ){newNode[FinalNet[j][2]]=1;check=1;}
                        else if(newNode[FinalNet[j][1]]==1 &&  newNode[FinalNet[j][2]]==0 ){newNode[FinalNet[j][1]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==0 &&  newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=0;check=1;}
                        else if(newNode[FinalNet[j][1]]==0 &&  newNode[FinalNet[j][2]]==Z ){newNode[FinalNet[j][2]]=0;check=1;}
                        else if(newNode[FinalNet[j][1]]==0 &&  newNode[FinalNet[j][2]]==1 ){newNode[FinalNet[j][1]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==X &&  newNode[FinalNet[j][2]]==0 ){newNode[FinalNet[j][1]]=0;check=1;}
                        else if(newNode[FinalNet[j][1]]==X &&  newNode[FinalNet[j][2]]==1 ){newNode[FinalNet[j][1]]=1;check=1;}
                        else if(newNode[FinalNet[j][1]]==X &&  newNode[FinalNet[j][2]]==Z ){newNode[FinalNet[j][1]]=Z;check=1;}
                        else if(newNode[FinalNet[j][1]]==X &&  newNode[FinalNet[j][2]]==SC ){newNode[FinalNet[j][1]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==Z &&  newNode[FinalNet[j][2]]==0 ){newNode[FinalNet[j][1]]=0;check=1;}
                        else if(newNode[FinalNet[j][1]]==Z &&  newNode[FinalNet[j][2]]==1 ){newNode[FinalNet[j][1]]=1;check=1;}
                        else if(newNode[FinalNet[j][1]]==Z &&  newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=Z;check=1;}
                        else if(newNode[FinalNet[j][1]]==Z &&  newNode[FinalNet[j][2]]==SC ){newNode[FinalNet[j][1]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==SC &&  newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==SC &&  newNode[FinalNet[j][2]]==Z ){newNode[FinalNet[j][2]]=SC;check=1;}

                    }
                    else if(transistorNet[j]==PMOS && newNode[FinalNet[j][0]]==1 ){ ////what we do when the transistor is PMOS and the gate is 1
                        if(newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=Z;check=1;}
                        else if(newNode[FinalNet[j][1]]==X ){newNode[FinalNet[j][1]]=Z;check=1;}
                    }
                    else if(transistorNet[j] ==NMOS && newNode[FinalNet[j][0]]==0 ){ ////what we do when the transistor is NMOS and the gate is 0
                        if(newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=Z;check=1;}
                        else if(newNode[FinalNet[j][1]]==X ){newNode[FinalNet[j][1]]=Z;check=1;}
                    }
                    else if(transistorNet[j]==NMOS && newNode[FinalNet[j][0]]==1 ){ ////what we do when the transistor is NMOS and the gate is 1
                        if(newNode[FinalNet[j][1]]==1 &&  newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=1;check=1;}
                        else if(newNode[FinalNet[j][1]]==1 &&  newNode[FinalNet[j][2]]==Z ){newNode[FinalNet[j][2]]=1;check=1;}
                        else if(newNode[FinalNet[j][1]]==1 &&  newNode[FinalNet[j][2]]==0 ){newNode[FinalNet[j][1]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==0 &&  newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=0;check=1;}
                        else if(newNode[FinalNet[j][1]]==0 &&  newNode[FinalNet[j][2]]==Z ){newNode[FinalNet[j][2]]=0;check=1;}
                        else if(newNode[FinalNet[j][1]]==0 &&  newNode[FinalNet[j][2]]==1 ){newNode[FinalNet[j][1]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==X &&  newNode[FinalNet[j][2]]==0 ){newNode[FinalNet[j][1]]=0;check=1;}
                        else if(newNode[FinalNet[j][1]]==X &&  newNode[FinalNet[j][2]]==1 ){newNode[FinalNet[j][1]]=1;check=1;}
                        else if(newNode[FinalNet[j][1]]==X &&  newNode[FinalNet[j][2]]==Z ){newNode[FinalNet[j][1]]=Z;check=1;}
                        else if(newNode[FinalNet[j][1]]==X &&  newNode[FinalNet[j][2]]==SC ){newNode[FinalNet[j][1]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==Z &&  newNode[FinalNet[j][2]]==0 ){newNode[FinalNet[j][1]]=0;check=1;}
                        else if(newNode[FinalNet[j][1]]==Z &&  newNode[FinalNet[j][2]]==1 ){newNode[FinalNet[j][1]]=1;check=1;}
                        else if(newNode[FinalNet[j][1]]==Z &&  newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=Z;check=1;}
                        else if(newNode[FinalNet[j][1]]==Z &&  newNode[FinalNet[j][2]]==SC ){newNode[FinalNet[j][1]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==SC &&  newNode[FinalNet[j][2]]==X ){newNode[FinalNet[j][2]]=SC;check=1;}
                        else if(newNode[FinalNet[j][1]]==SC &&  newNode[FinalNet[j][2]]==Z ){newNode[FinalNet[j][2]]=SC;check=1;}

                    }
                    j++;
                }

            }
            printf("For the %d TEST_VECTORS ",i);
            int k=0 ;
            while(OutOfNet[k]!=0){                                                        //////// prints every output we want to see
                if(newNode[OutOfNet[k]]==Z){printf("For File %d the output %d is : Z \n",numOfFile+1 ,OutOfNet[k]);}
                else if(newNode[OutOfNet[k]]==SC){printf("For File %d the output %d is : SC \n",numOfFile+1,OutOfNet[k]);}
                else if(newNode[OutOfNet[k]]==X){printf("For File %d the output %d is : X \n",numOfFile+1,OutOfNet[k]);}
                else{printf("For File %d the output %d is : %d \n",numOfFile+1,OutOfNet[k],newNode[OutOfNet[k]]);}
                k++;
            }

            i++;
            k=0;
            while(k<*sizeofnode){          /////////////////// finde where is the short-circuited/////////////////////////////////
                if(newNode[k]==SC){
                    printf("!!!!!!!!!NODE %d IS SORT-CIRCUIT !!!!!!!!!!!!!!!!",newNode[k]);
                }
                k++;
            }///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       }
    }
   return 0;
}
