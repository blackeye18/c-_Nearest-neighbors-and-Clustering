#include <iostream>
#include <cstring>
#include <vector>
#include <cctype>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <map>
#include <cassert>
#include <chrono>
#include <queue>

using namespace std;
using namespace std::chrono;

#include "classes_and_defines.hpp"
#include "input_menu_starting_func.hpp"
#include "lsh_basic_functions.hpp"
#include "knn_ranges_brutes.hpp"

int main(int argc, char *argv[]){
    char input_file[256],query_file[256],output_file[256],temp[256];
    int k,L,N;
    double R;
    int no_of_vectors,no_of_coordinates;
    int queries_no_of_vectors,queries_no_of_coordinates;
    int ret;
    vec* nvectors;
    vec* qvectors;
    Lhashtables *lht;
    string lsh_or_hypercube="distanceLSH: ";
    if(input_handler(argc,argv,&k,&L,&N,&R,(input_file), query_file, output_file))
        return -1;

    printf("input_file: %s, query_file: %s, output_file: %s,k:%d,L:%d,N:%d,R:%f\n",input_file,query_file,output_file,k,L,N,R);
    int flag=0;
    while(flag!=-1){

        if(flag==0 || flag==1){
            nvectors=open_and_create_vectors(input_file,&no_of_coordinates,&no_of_vectors);
            if(nvectors==NULL)
                return -1;
            printf("Input:: no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);
        }

        if(flag==0 || flag==2){
            qvectors=open_and_create_vectors(query_file,&queries_no_of_coordinates,&queries_no_of_vectors);  
            if(nvectors==NULL)
                return -1;
            printf("Queries:: queries_no_of_vectors: %d, queries_no_of_coordinates: %d\n",queries_no_of_vectors,queries_no_of_coordinates);
        }

        cout<<"Now using LSH and KNN"<<endl;
        auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        if(flag==0 || flag==1){
            lht=new Lhashtables(L,no_of_coordinates,k);
            lht->lsh_start(no_of_vectors,nvectors);
        }
        vector<vector<dist_vec>*>* dsvec2;
        dsvec2=lht->find_k_nearest(qvectors,N,queries_no_of_vectors);
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        double time1=((double)duration1.count()/1000000);
        if(dsvec2==NULL){
            delete lht;
            delete [] nvectors;
            delete [] qvectors;
            return -1;
        }

        cout<<"Now using brute calculation"<<endl;
        auto start2 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        vector<vector<dist_vec>*>* dsvec3;
        dsvec3=brute_calculate_all(qvectors,nvectors,no_of_vectors,no_of_coordinates,queries_no_of_vectors,N);
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        double time2=((double)duration2.count()/1000000);
        
        cout<<"Now using Radius Search"<<endl;
        vector<vector<dist_vec>*>* dsvec4;
        dsvec4=lht->find_in_LRadius(qvectors,R,queries_no_of_vectors);

        print_to_file(output_file,lsh_or_hypercube,dsvec2,queries_no_of_vectors,qvectors,time1,time2,dsvec3,dsvec4,R);

        cout<<"Output File Created!!"<<endl;
  
    flag=repeat_handler(nvectors,qvectors,input_file,query_file,output_file,lht);
    for(int i=0;i<queries_no_of_vectors;i++){

        vector<dist_vec>* dstemp2 =(*dsvec2)[i];
        vector<dist_vec>* dstemp3 =(*dsvec3)[i];
        vector<dist_vec>* dstemp4 =(*dsvec4)[i];

        dstemp2->clear();delete dstemp2;
        dstemp3->clear();delete dstemp3;
        dstemp4->clear();delete dstemp4;
    }
    

    dsvec2->clear();delete dsvec2;
    dsvec3->clear();delete dsvec3;
    dsvec4->clear();delete dsvec4;
    }
    delete lht;
    delete [] nvectors;
    delete [] qvectors;
        
}