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



int* initialization(int inp[],int testinp[],int testout[],int out[],int netlist[][3],int testvector[][5] ,int logicgate[],int* node,int*sizeofnode,char** Lib){
char ch;
   FILE *fp;

   fp = fopen("file2.txt", "r"); // read modet          ///////////////////////////////////////////

   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }                                                     // open and read file
   int i=0;
   char A[10000];
   while((ch = fgetc(fp)) != EOF){
      printf("%c", ch);
      A[i]=ch;
      i++;
      }                                                 //////////////////////////////////////////
    fclose(fp);


   char * S;
   S=A;                           // set a new string S = with the content of file = A


    S=strstr(S,"LIBRARY" );
    *Lib=stbetween('\n','\n',S);

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
   char *ptrIN = strtok(stbetween('\n','#',S) , ",");
   while(ptrIN != NULL)
	 {
	 	inp[i]=atoi(ptrIN);
	 	ptrIN = strtok(NULL, ",");
	 	i++;
	 }


   i=0;
   S=strstr(S,"OUTPUTS" );      /////// find the outputs
   char *ptrOut = strtok(stbetween('\n','#',S) , ",");
   while(ptrOut != NULL)
	 {
	 	out[i]=atoi(ptrOut);
	 	ptrOut = strtok(NULL, ",");
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
       char *ptrNet2 = strtok(temparr[i] , ",");///////////////////// the GATE_NODE , P_SOURCE_NODE/N_DRAIN_NODE and
       int j=1;
       int k=2;                                //////////////////// P_DRAIN_NODE/N_SOURCE_NODE
        while(ptrNet2 != NULL)
         {
            if(j==1){
                ptrNet2 = strtok(NULL, ",");
                j++;
            }
            else if(j==2){
                if(strcmp(remove_white_spaces(ptrNet2),"NOT")==0){
                        logicgate[i]=NOT;
                        ptrNet2 = strtok(NULL, ",");
                        j++;
                }
                else if(strcmp(remove_white_spaces(ptrNet2),"NOR_2")==0){
                        logicgate[i]=NOR_2;
                        ptrNet2 = strtok(NULL, ",");
                        j++;
                }
                else if (strcmp(remove_white_spaces(ptrNet2),"NAND_2")==0){
                    logicgate[i]=NAND_2;
                    ptrNet2 = strtok(NULL, ",");
                    j++;
                }
                else if (strcmp(remove_white_spaces(ptrNet2),"XOR_2")==0){
                    logicgate[i]=XOR_2;
                    ptrNet2 = strtok(NULL, ",");
                    j++;
                }
                else if(strcmp(remove_white_spaces(ptrNet2),"NMOS")==0) {
                    logicgate[i]=NMOS;
                    ptrNet2 = strtok(NULL, ",");
                    j++;
                }
                else {
                    logicgate[i]=PMOS;
                    ptrNet2 = strtok(NULL, ",");
                    j++;
                }

            }
            else if(j==3){
                if(logicgate[i]==PMOS || logicgate[i]==NMOS){
                    netlist[i][0]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                }
                ptrNet2 = strtok(NULL, ",");
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
                ptrNet2 = strtok(NULL, ",");
                j++;
            }
             else if(j==5){
                if(strcmp(remove_white_spaces(ptrNet2),"OUT")!=0) {
                    netlist[i][1]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                }
                else if(logicgate[i]==PMOS || logicgate[i]==NMOS){
                    netlist[i][2]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                }
                else{
                    netlist[i][1]=netlist[i][0];
                }
                ptrNet2 = strtok(NULL, ",");
                j++;
            }
            else if(j==6){
             if(strcmp(remove_white_spaces(ptrNet2),"OUT")!=0) {
                    netlist[i][2]=atoi(ptrNet2);
                    if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                }
             ptrNet2 = strtok(NULL, ",");
             j++;
            }
            else if(j>=7){
                 netlist[i][k]=atoi(ptrNet2);
                 k++;
                 if(atoi(ptrNet2)>max){max=atoi(ptrNet2);}
                 ptrNet2 = strtok(NULL, ",");
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


int main()
{

    char* Lib=NULL;

    int temp=1;
    int inp[10]={};           /////////////  define the arrays we need to store the information from file
    int testinp[10]={};
    int out[10]={};
    int testout[10]={};
    int netlist[20][3]={};
    int logicgate[20]={};
    int testvector[32][5]={};
    int* sizeofnode=&temp;
    int* node;                  //////////////


    node = initialization(inp,testinp,testout,out,netlist,testvector,logicgate,node,sizeofnode,&Lib);  ////it stores the information from file in the appropriate array


    struct Lgate gNOT={{},{},0,0,{},{},0,{},0,0,0}; ////structures that store iformation for every gate from Library
    struct Lgate gNAND={{},{},0,0,{},{},0,{},0,0,0};
    struct Lgate gNOR={{},{},0,0,{},{},0,{},0,0,0};
    struct Lgate gXOR={{},{},0,0,{},{},0,{},0,0,0};////



    initializeLib(&gNOT,&gNAND,&gNOR,&gXOR,Lib);    ////set properties for every Lgate = information from Library


    int netlistOut[50]={};
    int FinalNet[50][3] ={};//// this is the final netlist
    int FinalNetcount=0;
    int offset=0;
    int transistorNet[50] ={};////this is the transistor array for every row of final netlist
    int inpNet[15]={};////this is the input of the final netlist
    int numOfInp=0;
    int nodeNet[100]={};////this is the array that stores the value of every node
    int matchGate[50][2]={};

    int flag=1;
    int IsInList[50] ={};

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
     int OutOfNet[10]={} ;      //////////////// sets the outputs of the new netlist
     while(testout[i]!=0){
       OutOfNet[i]= netlistOut[testout[i]];
       i++;
    }

    int testInpNet[15]={};     ////////////// sets the test_Inputs of the new netlist
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


    i=0;
   while(testvector[i][0]!=0){ //for every test vector
        int j=0;
        int newNode[100]={};
        while(j<FinalNetcount){   //////////// copies all nodes to a new array so we can work with it
            newNode[j]=nodeNet[j];
            j++;
        }
        j=0;
        while(testInpNet[j]!=0){   ///////////////////  set the values of input nodes
              newNode[testInpNet[j]]= testvector[i][j]-1;
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
            if(newNode[OutOfNet[k]]==Z){printf(" the output %d is : Z \n",testout[k]);}
            else if(newNode[OutOfNet[k]]==SC){printf(" the output %d is : SC \n",testout[k]);}
            else if(newNode[OutOfNet[k]]==X){printf(" the output %d is : X \n",testout[k]);}
            else{printf(" the output %d is : %d \n",testout[k],newNode[OutOfNet[k]]);}
            k++;
        }

        i++;
        k=0;
        while(k<FinalNetcount){          /////////////////// finde where is the short-circuited/////////////////////////////////
//            printf("%d\n",newNode[k]);
            if(newNode[k]==SC){
                int j=0;
                int countNot=1;
                int countNand=1;
                int countNor=1;
                int countP=1;
                int countN=1;
                int countXOR=1;
                while(matchGate[j][1]<k){
                    if(matchGate[j][0]==NOT) countNot++;
                    else if(matchGate[j][0]==NOR_2) countNor++;
                    else if (matchGate[j][0]==NAND_2)  countNand++;
                    else if (matchGate[j][0]==XOR_2)  countXOR++;
                    else if (matchGate[j][0]==PMOS)   countP++;
                    else if (matchGate[j][0]==NMOS) countN++;
                    j++;
                }
                if(matchGate[j][0]==NOT){
                printf("Node %d of gate NOT number %d is short-circuited\n",k-matchGate[j-1][1],countNot);
                }
                if(matchGate[j][0]==NOR_2){
                printf("Node %d of gate NOR_2 number %d is short-circuited\n",k-matchGate[j-1][1],countNor);
                }
                if(matchGate[j][0]==NAND_2){
                printf("Node %d of gate NAND_2 number %d is short-circuited\n",k-matchGate[j-1][1],countNand);
                }
                if(matchGate[j][0]==XOR_2){
                printf("Node %d of gate XOR_2 number %d is short-circuited\n",k-matchGate[j-1][1],countXOR);
                }
                if(matchGate[j][0]==PMOS){
                printf("Node %d of transistor PMOS number %d is short-circuited\n",k-matchGate[j-1][1],countP);
                }
                if(matchGate[j][0]==NMOS){
                printf("Node %d of transistor NMOS number %d is short-circuited\n",k-matchGate[j-1][1],countN);
                }
            }
            k++;
        }///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   }
   return 0;
}
