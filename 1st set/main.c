#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int multiplications;
int additions;
int substractions;
int divisions;


float power(float a,float b ){                                          //just calculate the a^b
    float c=1;
    if(b==0){
        multiplications++;
        return 1;
    }
    for (int i=0;i<b;i++){
        c=c*a;
        multiplications++;
    }
    return c;
}


float derivativeOnPointAnalytically(int rank,float coef[],float point){  // this function calculate the derivative on a specific point
    float newcoef[rank];
    float derOnPoint=0;

    for(int i=0;i<rank;i++){                                                // the new derivative cooeficients
        newcoef[i]=coef[i+1]*(i+1);
        multiplications++;
    }

    for(int i=0;i<rank;i++){
         derOnPoint=derOnPoint+newcoef[i]*power(point,i);                   // the value of derivative in a spesific point
         additions++;
         multiplications++;
    }
    return(derOnPoint);
}


float givey(float point,float coef[] , int rank){                              //gives the y value of a x number it takes
     float y=0;
     for(int i=0;i<rank+1;i++){
         y=y+coef[i]*power(point,i);
          multiplications++;
          additions++;
    }
    return y;
}


float derivativeOnPointArithmetically(int rank,float coef[],float point){          // this function calculate the derivative on a specific point arithmetically

    float derOnPoint=0;

    derOnPoint=(givey(point+0.0001,coef,rank)-givey(point,coef,rank))/(0.0001);    //  Dy/Dx
    additions++;
    substractions++;
    divisions++;

    return derOnPoint;
}


float tangentIsZero(float der,float y,float x){      // here we calculate the point that the tangent cross the x axes
    float newx;
    newx=(der*x-y)/der;                              //y-yo=f'(xo)(x-xo)   is the wa to find the tangent
     multiplications++;
     substractions++;
     divisions++;
    return newx;

}





int main()
{

    int rank;

    while (rank>5 || rank<1){                                   //input for polyonimal's rank
    printf("give the rank of polyonimal from 1 - 5  : \n");
    scanf("%d",&rank);
    }

    float coef[rank+1];
    int i;

    for( i =0;i< sizeof(coef)/ sizeof( float) ; i++){              //input for polyonimal's coefficient
        printf("give the coefficient of the %d factor \n",i);
        scanf("%f",&coef[i]);
    }

    float der;
    float y=1;
    float x=-10;
    int count=0;

    while((y>0.001 || y<-0.001) && count<20 ){                  // loop we do until we find the solution

    der=derivativeOnPointAnalytically(rank,coef,x);             // this function calculate the derivative on a specific point
    //der=derivativeOnPointArithmetically(rank,coef,x);         // the second wa to calculate the derivative arithmetically on a specific point
    printf("X(%d):%f \n",count,x);
    x=tangentIsZero(der,givey(x,coef,rank),x);                  // here we calculate the point that the tangent cross the x axes
    y=givey(x,coef,rank);                                       // the y value of  the point that the tangent cross the x axes
    count++;
    }

    printf("repetation : %d \n",count);
    printf("additions:%d\n",additions);
    printf("substractions:%d\n",substractions);
    printf("multiplications:%d\n",multiplications);
    printf("divisions:%d\n",divisions);
    if(count<20){
    printf("the solution is for  x: %f ,y:%f \n",x,y);
    }
    else
    {
         printf("cant find solution");
    }
    return 0;
}



