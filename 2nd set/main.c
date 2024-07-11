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



int* initialization(int inp[],int testinp[],int testout[],int out[],int netlist[][3],int testvector[][5] ,int transistor[],int* node,int*sizeofnode){
char ch;
   FILE *fp;

   fp = fopen("file4.txt", "r"); // read modet          ///////////////////////////////////////////

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

   int vccarray[10]={};;
   i=0;
   while(strstr(S,"VCC" )!=NULL){  // find the nodes which are conected with VCC
    S=strstr(S,"VCC" );
    char temp1[3];
    int j=4;
    while(S[j]!=' ' && S[j]!='\n' ){
      temp1[j-4]=S[j];
      j++;
    }
    vccarray[i]=atoi(temp1);
    i++;
    S=S+4;
   }

   i=0;
   int gndarray[10]={};
   while(strstr(S,"GND" )!=NULL){  // find the nodes which are conected with GND
    S=strstr(S,"GND" );
    char temp2[3];
    int j=4;
    while(S[j]!=' ' && S[j]!='\n' ){
      temp2[j-4]=S[j];
      j++;
    }
    gndarray[i]=atoi(temp2);
    i++;
    S=S+4;
   }


   i=0;
   S=strstr(S,"INPUTS" );      /////// find the inputs
   char *ptrIN = strtok(stbetween('\n','#',S) , ";");
   while(ptrIN != NULL)
	 {
	 	inp[i]=atoi(ptrIN);
	 	ptrIN = strtok(NULL, ";");
	 	i++;
	 }


   i=0;
   S=strstr(S,"OUTPUTS" );      /////// find the outputs
   char *ptrOut = strtok(stbetween('\n','#',S) , ";");
   while(ptrOut != NULL)
	 {
	 	out[i]=atoi(ptrOut);
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
                        transistor[i]=PMOS;
                        ptrNet2 = strtok(NULL, " ");
                        j++;
                }
                else{
                        transistor[i]=NMOS;
                        ptrNet2 = strtok(NULL, " ");
                        j++;
                }

            }
            else if(j==3){
                netlist[i][0]=atoi(ptrNet2);
                if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                ptrNet2 = strtok(NULL, " ");
                j++;
            }
            else if(j==4){
                netlist[i][1]=atoi(ptrNet2);
                if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                ptrNet2 = strtok(NULL, " ");
                j++;
            }
             else if(j==5){
                netlist[i][2]=atoi(ptrNet2);
                if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                ptrNet2 = strtok(NULL, " ");
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

     node=node1;
       return node;


}


int main()
{
    int inp[10]={};           /////////////  define the arrays we need to store the information from file
    int testinp[10]={};
    int out[10]={};
    int testout[10]={};
    int netlist[20][3]={};
    int transistor[20]={};
    int testvector[32][5]={};
    int* sizeofnode;
    int* node;                  //////////////

    node = initialization(inp,testinp,testout,out,netlist,testvector,transistor,node,sizeofnode);  ////it stores the information from file in the appropriate array


   int i=0;

   while(testvector[i][0]!=0){
        int j=0;
        int newNode[30]={};
        while(j<*sizeofnode){   //////////// copies all nodes to a new array so we can work with it
            newNode[j]=*(node+j);
            j++;
        }
        j=0;
        while(testinp[j]!=0){   ///////////////////  set the values of input nodes
              newNode[testinp[j]]= testvector[i][j]-1;
              j++;
              }

        int check = 1;
        while(check==1){   ///////// run until nothing changed so we know that the algorithm has finished
            check=0;
            j=0;
            while(netlist[j][0]!=0){  ////run 1 1 time for every transistor

                if(transistor[j]==PMOS && newNode[netlist[j][0]]==0 ){  ////what we do when the transistor is PMOS and the gate is 0
                    if(newNode[netlist[j][1]]==1 &&  newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=1;check=1;}
                    else if(newNode[netlist[j][1]]==1 &&  newNode[netlist[j][2]]==Z ){newNode[netlist[j][2]]=1;check=1;}
                    else if(newNode[netlist[j][1]]==1 &&  newNode[netlist[j][2]]==0 ){newNode[netlist[j][1]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==0 &&  newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=0;check=1;}
                    else if(newNode[netlist[j][1]]==0 &&  newNode[netlist[j][2]]==Z ){newNode[netlist[j][2]]=0;check=1;}
                    else if(newNode[netlist[j][1]]==0 &&  newNode[netlist[j][2]]==1 ){newNode[netlist[j][1]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==X &&  newNode[netlist[j][2]]==0 ){newNode[netlist[j][1]]=0;check=1;}
                    else if(newNode[netlist[j][1]]==X &&  newNode[netlist[j][2]]==1 ){newNode[netlist[j][1]]=1;check=1;}
                    else if(newNode[netlist[j][1]]==X &&  newNode[netlist[j][2]]==Z ){newNode[netlist[j][1]]=Z;check=1;}
                    else if(newNode[netlist[j][1]]==X &&  newNode[netlist[j][2]]==SC ){newNode[netlist[j][1]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==Z &&  newNode[netlist[j][2]]==0 ){newNode[netlist[j][1]]=0;check=1;}
                    else if(newNode[netlist[j][1]]==Z &&  newNode[netlist[j][2]]==1 ){newNode[netlist[j][1]]=1;check=1;}
                    else if(newNode[netlist[j][1]]==Z &&  newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=Z;check=1;}
                    else if(newNode[netlist[j][1]]==Z &&  newNode[netlist[j][2]]==SC ){newNode[netlist[j][1]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==SC &&  newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==SC &&  newNode[netlist[j][2]]==Z ){newNode[netlist[j][2]]=SC;check=1;}

                }
                else if(transistor[j]==PMOS && newNode[netlist[j][0]]==1 ){ ////what we do when the transistor is PMOS and the gate is 1
                    if(newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=Z;check=1;}
                    else if(newNode[netlist[j][1]]==X ){newNode[netlist[j][1]]=Z;check=1;}
                }
                else if(transistor[j] ==NMOS && newNode[netlist[j][0]]==0 ){ ////what we do when the transistor is NMOS and the gate is 0
                    if(newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=Z;check=1;}
                    else if(newNode[netlist[j][1]]==X ){newNode[netlist[j][1]]=Z;check=1;}
                }
                else if(transistor[j]==NMOS && newNode[netlist[j][0]]==1 ){
                    if(newNode[netlist[j][1]]==1 &&  newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=1;check=1;} ////what we do when the transistor is NMOS and the gate is 1
                    else if(newNode[netlist[j][1]]==1 &&  newNode[netlist[j][2]]==Z ){newNode[netlist[j][2]]=1;check=1;}
                    else if(newNode[netlist[j][1]]==1 &&  newNode[netlist[j][2]]==0 ){newNode[netlist[j][1]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==0 &&  newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=0;check=1;}
                    else if(newNode[netlist[j][1]]==0 &&  newNode[netlist[j][2]]==Z ){newNode[netlist[j][2]]=0;check=1;}
                    else if(newNode[netlist[j][1]]==0 &&  newNode[netlist[j][2]]==1 ){newNode[netlist[j][1]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==X &&  newNode[netlist[j][2]]==0 ){newNode[netlist[j][1]]=0;check=1;}
                    else if(newNode[netlist[j][1]]==X &&  newNode[netlist[j][2]]==1 ){newNode[netlist[j][1]]=1;check=1;}
                    else if(newNode[netlist[j][1]]==X &&  newNode[netlist[j][2]]==Z ){newNode[netlist[j][1]]=Z;check=1;}
                    else if(newNode[netlist[j][1]]==X &&  newNode[netlist[j][2]]==SC){newNode[netlist[j][1]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==Z &&  newNode[netlist[j][2]]==0 ){newNode[netlist[j][1]]=0;check=1;}
                    else if(newNode[netlist[j][1]]==Z &&  newNode[netlist[j][2]]==1 ){newNode[netlist[j][1]]=1;check=1;}
                    else if(newNode[netlist[j][1]]==Z &&  newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=Z;check=1;}
                    else if(newNode[netlist[j][1]]==Z &&  newNode[netlist[j][2]]==SC ){newNode[netlist[j][1]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==SC &&  newNode[netlist[j][2]]==X ){newNode[netlist[j][2]]=SC;check=1;}
                    else if(newNode[netlist[j][1]]==SC &&  newNode[netlist[j][2]]==Z ){newNode[netlist[j][2]]=SC;check=1;}

                }
                j++;
            }

        }
        printf("For the %d TEST_VECTORS ",i);
        int k=0 ;
        while(testout[k]!=0){                                                        //////// prints every output we want to see
            if(newNode[testout[k]]==Z){printf(" the output %d is : Z \n",testout[k]);}
            else if(newNode[testout[k]]==SC){printf(" the output %d is : SC \n",testout[k]);}
            else if(newNode[testout[k]]==X){printf(" the output %d is : X \n",testout[k]);}
            else{printf(" the output %d is : %d \n",testout[k],newNode[testout[k]]);}
            k++;
        }

        i++;
        k=0;
        while(k<*sizeofnode){                                   //// finde where is ♂6he short-circuited
//            printf("%d\n",newNode[k]);
            if(newNode[k]==SC){printf("Node %d is short-circuited\n",k);}
            k++;
        }
   }



   return 0;
}
