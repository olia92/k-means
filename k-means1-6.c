/*
*********************************************************************
gcc -v:	gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04)
OS: 	Ubuntu 20.04.2 LTS
CPU:	4-core, Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz, 4393.71 BogoMips
*/
//************************************************************



#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <math.h>

#define N 100000 //observations
#define Nv 1000 //d-dimensional
#define Nc 100 //sets
#define THR_KMEANS 0.000001

void InitializeVec();
void InitializeCenters();
float Euclidean();
void CalculateCalsses();
float CalculateCenters();

//*******************
float Vec[N][Nv];
float Center[Nc][Nv];
int Classes[N];


int main(){
    clock_t start, end;
    double cpu_time_used=0;
    float sum = 1;
    int counter =0;

    InitializeVec();
    InitializeCenters();
    
    printf("Entering while...\n");
    printf("count . error . time per loop \n");
    while (sum>THR_KMEANS && counter<3){
        start=clock();
        CalculateCalsses();
        sum=CalculateCenters();
        counter++;
        end = clock();
        printf("%d . %f . (time: %lf)\n",counter,sum,(double)(end - start) / CLOCKS_PER_SEC);
        cpu_time_used += ((double) (end - start)) / CLOCKS_PER_SEC;
    }
    
    printf("Converged after %d  cirlces\n",counter);
    printf("Total time :%lf s\n",cpu_time_used);
    return 0;
}


//************************

void InitializeVec(){
    printf("Initializing Vector...\n");
    
    for(int i=0;i<N;i++){
        for(int j=0;j<Nv;j++){
            //random values [-10,10]
            Vec[i][j]=(rand() % (10 - (-10) + 1)) + (-10);//(rand()/(float)RAND_MAX);
        }
    }
}

void InitializeCenters(){
    printf("Initializing Centers...\n");
    

   memcpy (Center,Vec,sizeof(Center));

   int counter = Nc;
   for(int i=0;i<Nc;i++){
        for(int k = i+1; k<Nc;k++){
            if(!memcmp(Center[i],Center[k],sizeof(Center[k]))){
                if(counter>=N) {
                    printf("!!!Same Center Values\n");
                    return;
                }
                else{
                    memcpy(Center[i],Vec[counter],sizeof(Vec[counter]));
                    counter++;
                    k--;         
                }
            }
       }
   }
}


float Euclidean(float *C, float *V){
    float sum = 0.0;
    for(int i=0;i<Nv;i++){
        float temp =C[i]-V[i];
        sum += temp*temp;
    }
    return sum;
}

void CalculateCalsses(){

    float dist = 0;
    float mindist=0;
    for(int i=0;i<N;i++){
        mindist=Euclidean(Center[0],Vec[i]);
        Classes[i]=0;
        for(int j=1;j<Nc;j++){
            dist=Euclidean(Center[j],Vec[i]);
            if(dist<mindist){
                Classes[i]=j;
                mindist=dist;
            }
        }
    }
};

float CalculateCenters(){

    float newCenters[Nc][Nv]={0};
    int counter[Nc]={0}; //number of observations at each class

    for(int i=0;i<N;i++){
        int classi=Classes[i];
        
        for(int j=0;j<Nv;j++){
        newCenters[classi][j] += Vec[i][j];
        }
        counter[classi]++;
    }
    //*
    for(int i=0;i<Nc;i++){
        
        for(int j=0;j<Nv;j++){
        newCenters[i][j]=newCenters[i][j]/counter[i];
        }
    }//*/

    float sum=0;
    for(int i=0;i<Nc;i++){        
        sum+=Euclidean(Center[i],newCenters[i]);
    }
    
    memcpy(Center,newCenters,sizeof(Center));
    return sqrt(sum/Nc);
}

