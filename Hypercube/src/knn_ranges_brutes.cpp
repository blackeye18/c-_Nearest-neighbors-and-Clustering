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



long double vect_dist(vector<double> vecA,vector<double> vecB,int d)//ipologismos apostasis meta3i 2 vec
	{
	long int sum=0;
	for (int i = 0; i < d; ++i)
		{
		long int temp=abs(vecA[i]-vecB[i]);//pernoume tin apoliti timi tis afairesis (apostasi panta >0)
		sum+=temp*temp;//ta ipsonoume stin 2ara
		}
	return sqrt(sum);//tetragwniki riza typos sel 17 tou nnCluster.pdf
	}

vector<dist_vec>* hypercube::NN_search(vec* qvect,int N)
    {
    int M_count=0;
    int probes_count=0;

    unsigned int qbits=this->hash_calc(qvect);//iologizume to bin hashvalue tou query  vec

    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;//priority queue

    for (int i = 0; i < cube_vec[qbits].size(); ++i)//pigenume sto probe pu 8a pigene an to query vec itan sto sinolo twn seimeiwn mas diladi hamming distance 0
    	{

    	probes_count=1;
    	M_count++;
        if(metric=="euclidean_distance"){
            long double dist= vect_dist(qvect->coord,cube_vec[qbits][i]->coord,d);
            Q.push(dist_vec(dist,cube_vec[qbits][i]));
        }else{
            cout<<"No function for metric:"<<metric<<endl;
                return NULL;
        }
        if (probes_count>=probes||M_count>=M)
            break;
    	}


    
    int temp=0;
    if (probes_count<=probes && M_count<=M)
    	{

    	for (int hd = 1; hd < k; ++hd)//elegxume probes me to analogo hammind distance
    		{

    		for (int ki = 0; ki <powk; ++ki)//gia ola ta probes
    			{
                temp++;

    			if(!cube_vec[ki].empty() && hammingDistance(ki,qbits)==hd)//elegxume oti to probe den ine adeio kai oti exei to analogo hamming distance
    				{

    				probes_count++;
    				for (int i = 0; i < cube_vec[ki].size(); ++i)
    					{
    					M_count++;
                        if(metric=="euclidean_distance"){
        				    long double dist= vect_dist(qvect->coord,cube_vec[ki][i]->coord,d);
        				    Q.push(dist_vec(dist,cube_vec[ki][i]));//to vazume sto priority queue
                        }else{
                            cout<<"No function for metric:"<<metric<<endl;
                            return NULL;
                        }
        				if (M_count>=M)
            				break;
    					}

            		
    				}
    			if (probes_count>=probes||M_count>=M)//an exume ftasei ton megisto aritmo probes kai M simeiwn stamatame
            		break;
    			}
    		if (probes_count>=probes||M_count>=M)
            	break;
    		}
    	}

    vector<dist_vec>* dsvec=new vector<dist_vec>;

    for (int i = 0; i < N; ++i)//kratame tus N prwtous apto priority queu diladi tus N me tin mikroteri distance apto qvector
        {
        if(!Q.empty())
            {
            dist_vec tempdv(Q.top().dist,Q.top().vect);
            dsvec->push_back(tempdv);
            Q.pop();
            }
        else
            break;
        }
        
    return dsvec;  
    }


vector<vector<dist_vec>*>* hypercube:: all_NN_search(vec* qvectors,int N,int queries_no_of_vectors,double*time_per_query_cube ){
	vector<vector<dist_vec>*>* dsvec2;
    dsvec2=new vector<vector<dist_vec>*>;

    for(int i=0;i<queries_no_of_vectors;i++){
        auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        vector<dist_vec>* temp=NN_search(&(qvectors[i]),N);//kaloume tin NN_search gia ka8e qvect
        if(temp==NULL){
            delete dsvec2;
            return NULL;
        }
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        double time1=((double)duration1.count()/1000000);

        time_per_query_cube[i]+=time1;
        dsvec2->push_back(temp);
    }
    return dsvec2;
}


vector<dist_vec>* hypercube::RANGE_search(vec* qvect,double R)
    {
    int M_count=0;
    int probes_count=0;

    unsigned int qbits=this->hash_calc(qvect);

    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;

    for (int i = 0; i < cube_vec[qbits].size(); ++i)
        {
 
        probes_count=1;
        M_count++;
        if(metric=="euclidean_distance"){
            long double dist= vect_dist(qvect->coord,cube_vec[qbits][i]->coord,d);
            if(dist<=R)
                Q.push(dist_vec(dist,cube_vec[qbits][i]));
        }else
        {
            cout<<"No function for metric:"<<metric<<endl;
            return NULL;
        }
        if (probes_count>=probes||M_count>=M)
            break;
        }


    
int temp=0;
    if (probes_count<=probes && M_count<=M)
        {

        for (int hd = 1; hd < k; ++hd)
            {
                //temp++;
            for (int ki = 0; ki <powk; ++ki)
                {temp++;

                if(!cube_vec[ki].empty() && hammingDistance(ki,qbits)==hd)
                    {

                    probes_count++;
                    for (int i = 0; i < cube_vec[ki].size(); ++i)
                        {
                        M_count++;
                        if(metric=="euclidean_distance"){
                        long double dist= vect_dist(qvect->coord,cube_vec[ki][i]->coord,d);
                        if(dist<R)//an i apostasi ine mikroteri aptin aktina to vazume sto priority queue
                            Q.push(dist_vec(dist,cube_vec[ki][i]));
                        }else{
                            cout<<"No function for metric:"<<metric<<endl;
                            return NULL;
                        }
                        if (M_count>=M)
                            break;
                        }

                    }
                if (probes_count>=probes||M_count>=M)
                    break;
                }
            if (probes_count>=probes||M_count>=M)
                break;
            }
        }

    vector<dist_vec>* dsvec=new vector<dist_vec>;

   
        while(!Q.empty())
            {
            dist_vec tempdv(Q.top().dist,Q.top().vect);
            dsvec->push_back(tempdv);
            Q.pop();
            }
        
        
    return dsvec;  
    }





vector<vector<dist_vec>*>* hypercube:: all_RANGE_search(vec* qvectors,double R,int queries_no_of_vectors){
    vector<vector<dist_vec>*>* dsvec2;
    dsvec2=new vector<vector<dist_vec>*>;

    for(int i=0;i<queries_no_of_vectors;i++){
        dsvec2->push_back(RANGE_search(&(qvectors[i]),R));
    }
    return dsvec2;
}

//sxolia iparxun stin lsh
vector<dist_vec>* brute_calculate(vec* qvector,vec* nvectors,int no_of_vectors,int no_of_coordinates,int N,int pos){
    long double dist;
    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;
    for(int i=0;i<no_of_vectors;i++){
        if(metric=="euclidean_distance"){
            dist=vect_dist(qvector[pos].coord,nvectors[i].coord,no_of_coordinates);
            Q.push(dist_vec(dist,&(nvectors[i])));
        }else{
         cout<<"No function for metric:"<<metric<<endl;
         return NULL;
        }

    }

     vector<dist_vec>* dsvec=new vector<dist_vec>;//=new vector<dist_vec>;
    
    for (int i = 0; i < N; ++i)
        {
        if(!Q.empty())
            {
            dist_vec tempdv(Q.top().dist,Q.top().vect);
            dsvec->push_back(tempdv);
            Q.pop();
            }
        else
            break;
        }
        
    return dsvec;      
    
}

vector<vector<dist_vec>*>* brute_calculate_all(vec* qvectors,vec* nvectors,int no_of_vectors,int no_of_coordinates,int queries_no_of_vectors,int N,double *time_per_query_brute){
    vector<vector<dist_vec>*>* dsvec2;
    dsvec2=new vector<vector<dist_vec>*>;
    for(int i=0;i<queries_no_of_vectors;i++){
        auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        dsvec2->push_back(brute_calculate(qvectors,nvectors,no_of_vectors,no_of_coordinates,N,i));
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        double time1=((double)duration1.count()/1000000);
        time_per_query_brute[i]+=time1;

    }
    return dsvec2;
}

