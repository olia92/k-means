#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime> 
#include <string.h>
#include <math.h>


using namespace std;

#define N 100000 //observations
#define Nv 1000 //d-dimensional
#define Nc 100 //sets
#define THR_KMEANS 0.000001

void PrintVec();
void PrintArray(int *a,int n);
void PrintArray(float *a,int n);
void InitializeVec();
void InitializeCenters();
float Euclidean();
void CalculateCalsses();
float CalculateCenters();
void SaveFile();
void SaveFile1();



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
    
    cout<<"Entering while..."<<endl;
    while (sum>THR_KMEANS && counter<3){
        start=clock();
        CalculateCalsses();
        sum=CalculateCenters();
        counter++;
        end = clock();
        cout<<counter<<". "<<sum<<" . (time:"<<(double)(end - start) / CLOCKS_PER_SEC<<"s)"<<endl;
        cpu_time_used += ((double) (end - start)) / CLOCKS_PER_SEC;
    }
    //PrintArray((float *)Center,Nc*Nv);
    //SaveFile();
    cout<<"Converged after "<<counter<<" cirlces"<<endl;
    cout<<"Total time :"<<cpu_time_used<<"s"<<endl;
    return 0;
}


//************************

void InitializeVec(){
    cout<<"Initializing Vector..."<<endl;
    
    for(int i=0;i<N;i++){
        for(int j=0;j<Nv;j++){
            //random values [0,1]
            Vec[i][j]=(rand()/(float)RAND_MAX);//(rand() % (10 - (-10) + 1)) + (-10);
        }
    }
}

void InitializeCenters(){
    cout<<"Initializing Centers..."<<endl;
    
/*
    for(int i=0;i<Nc;i++){
        for(int j=0;j<Nv;j++){
            Center[i][j]=Vec[i][j];
        }

    }
    //*/
   memcpy (Center,Vec,sizeof(Center));

   int counter = Nc;
   for(int i=0;i<Nc;i++){
        for(int k = i+1; k<Nc;k++){
            if(!memcmp(Center[i],Center[k],sizeof(Center[k]))){
                if(counter>=N) {
                    cout<<"!!!Same Center Values"<<endl;
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

void PrintVec(){
    cout<<"Printing Vec..."<<endl;
    for(int i=0;i<N;i++){
        for(int j=0;j<Nv;j++){
            cout << Vec[i][j] << " ";
        }
        cout<<" , ";
    }
    cout<<endl;
}


void PrintArray(float *a,int n){

    for(int i=0;i<n;i++){
            cout << a[i] << " ";
    }
    cout<<endl;
}


void PrintArray(int *a,int n){

    for(int i=0;i<n;i++){
            cout << a[i] << " ";
    }
    cout<<endl;
}

float Euclidean(float *C, float *V){
    float sum = 0.0;
    for(int i=0;i<Nv;i++){
        sum += (C[i]-V[i])*(C[i]-V[i]);
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
        sum+=Euclidean(Center[i],newCenters[i]);//abs(Center[i][j]-newCenters[i][j]);
    }
    
    memcpy(Center,newCenters,sizeof(Center));
    return sqrt(sum/Nc);
}


void SaveFile(){
    
    ofstream outFile1("Vec-Class.csv");
    for(int i=0;i<Nv;i++) outFile1 << "Vec-" <<i<< ",";
        outFile1 << "Class" << endl;
     for(int i=0;i<N;i++){
         for(int j=0; j<Nv;j++){
            outFile1 << Vec[i][j] << ",";
         }
         outFile1 << Classes[i] << endl;
    }
    ofstream outFile2("Center.csv");
     for(int i=0;i<Nc;i++){
         for(int j=0; j<Nv-1;j++){
            outFile2 << Vec[i][j] << ",";
         }
         outFile2 << Center[i][Nv-1] << endl;
    }

}

void SaveFile1(){
    
    ofstream outFile2("Center_old.csv");
     for(int i=0;i<Nc;i++){
         for(int j=0; j<Nv-1;j++){
            outFile2 << Vec[i][j] << ",";
         }
         outFile2 << Center[i][Nv-1] << endl;
    }

    ofstream outFile3("Class_old.csv");
     for(int i=0;i<N;i++){
            outFile3<< Classes[i] << endl;
    }
}
