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
#include <queue>
#include <bitset>


using namespace std;
using namespace std::chrono;

#include "classes_and_defines.hpp"
#include "input_menu_starting_func.hpp"
#include "cube_basic_functions.hpp"
#include "knn_ranges_brutes.hpp"


int main(int argc, char *argv[]){
	char input_file[256],query_file[256],output_file[256];
	int k,M,probes,N;
    double R;
    int no_of_vectors,no_of_coordinates;
    int queries_no_of_vectors,queries_no_of_coordinates;
    int ret;
    vec* nvectors;
    vec* qvectors;
    hypercube *cube;
    string lsh_or_hypercube="distanceHypercube: ";	

    if(input_handler(argc,argv,&k,&M,&probes,&N,&R,(input_file), query_file, output_file))
        return -1;

    printf("input_file: %s, query_file: %s, output_file: %s,k:%d,M:%d,probes:%d,N:%d,R:%f\n",input_file,query_file,output_file,k,M,probes,N,R);

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
        cout<<"Now using hypercube and KNN"<<endl;
        auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        if(flag==0 || flag==1){
            cube=new hypercube(M,probes,no_of_coordinates,k,no_of_vectors);
            cube->cube_start(no_of_vectors,nvectors);
            //->cube_map_print();
        }
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        double time1=((double)duration1.count()/1000000);
        time1=time1/no_of_vectors;
        double time_per_query_cube[queries_no_of_vectors];
        for(int i=0;i<queries_no_of_vectors;i++)//apothikevoume ton xrono poy xreiastike gia na dhmiourgithoun oi domes gia to cube
            time_per_query_cube[i]=time1;
        //cout<<"TIME CUBE:"<<time1<<endl;;
        vector<vector<dist_vec>*>* dsvec2;
        dsvec2=cube->all_NN_search(qvectors,N,queries_no_of_vectors,time_per_query_cube);
        
        if(dsvec2==NULL){
            delete cube;
            delete [] nvectors;
            delete [] qvectors;
            //cout<<"No function for metric:"<<metric<<endl;
            return -1;
        }


        cout<<"Now using brute calculation"<<endl;

        double time_per_query_brute[queries_no_of_vectors];
        for(int i=0;i<queries_no_of_vectors;i++)//arxikopoioume ton xrono me 0
            time_per_query_brute[i]=0;

        vector<vector<dist_vec>*>* dsvec3;
        dsvec3=brute_calculate_all(qvectors,nvectors,no_of_vectors,no_of_coordinates,queries_no_of_vectors,N,time_per_query_brute);
       
        cout<<"Now using Radius Search"<<endl;
        vector<vector<dist_vec>*>* dsvec4;
        dsvec4=cube->all_RANGE_search(qvectors,R,queries_no_of_vectors);
        print_to_file(output_file,lsh_or_hypercube,dsvec2,queries_no_of_vectors,qvectors,time_per_query_cube,time_per_query_brute,dsvec3,dsvec4,R);

        cout<<"Output File Created!!"<<endl;
        flag=repeat_handler(nvectors,qvectors,input_file,query_file,output_file,cube);//,lht
    	
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
    delete cube;
    delete [] nvectors;
    delete [] qvectors;
}