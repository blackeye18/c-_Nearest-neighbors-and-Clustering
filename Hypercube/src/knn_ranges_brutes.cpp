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



long double vect_dist(vector<double> vecA,vector<double> vecB,int d)
	{
	long int sum=0;
	for (int i = 0; i < d; ++i)
		{
		long int temp=abs(vecA[i]-vecB[i]);
		sum+=temp*temp;
		}
	return sqrt(sum);
	}

vector<dist_vec>* hypercube::NN_search(vec* qvect,int N)
    {
    int M_count=0;
    int probes_count=0;

    unsigned int qbits=this->hash_calc(qvect);

    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;

    for (int i = 0; i < cube_vec[qbits].size(); ++i)
    	{
    		//cout<<"Mphka"<<endl;
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

    //cube_vec[qbits].clear();
    
int temp=0;
    if (probes_count<=probes && M_count<=M)
    	{//cout<<"Mphka2"<<endl;

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
        				    Q.push(dist_vec(dist,cube_vec[ki][i]));
                        }else{
                            cout<<"No function for metric:"<<metric<<endl;
                            return NULL;
                        }
        				if (M_count>=M)
            				break;
    					}

            		//cube_vec[ki].clear();
    				}
    			if (probes_count>=probes||M_count>=M)
            		break;
    			}
    		if (probes_count>=probes||M_count>=M)
            	break;
    		}
    	}
//cout<<"M_count"<<M_count<<"probes_count"<<probes<<endl;
//cout<<"temp"<<temp<<endl;
    vector<dist_vec>* dsvec=new vector<dist_vec>;//=new vector<dist_vec>;
    //cout<<"alright4"<<endl;
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


vector<vector<dist_vec>*>* hypercube:: all_NN_search(vec* qvectors,int N,int queries_no_of_vectors){
	vector<vector<dist_vec>*>* dsvec2;
    dsvec2=new vector<vector<dist_vec>*>;

    for(int i=0;i<queries_no_of_vectors;i++){
        vector<dist_vec>* temp=NN_search(&(qvectors[i]),N);
        if(temp==NULL){
            delete dsvec2;
            return NULL;
        }
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
            //cout<<"Mphka"<<endl;
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

    //cube_vec[qbits].clear();
    
int temp=0;
    if (probes_count<=probes && M_count<=M)
        {//cout<<"Mphka2"<<endl;

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
                        if(dist<R)
                            Q.push(dist_vec(dist,cube_vec[ki][i]));
                        }else{
                            cout<<"No function for metric:"<<metric<<endl;
                            return NULL;
                        }
                        if (M_count>=M)
                            break;
                        }

                    //cube_vec[ki].clear();
                    }
                if (probes_count>=probes||M_count>=M)
                    break;
                }
            if (probes_count>=probes||M_count>=M)
                break;
            }
        }
//cout<<"M_count"<<M_count<<"probes_count"<<probes<<endl;
//cout<<"temp"<<temp<<endl;
    vector<dist_vec>* dsvec=new vector<dist_vec>;//=new vector<dist_vec>;
    //cout<<"alright4"<<endl;
   
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
        //cout<<dist<<endl;
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

vector<vector<dist_vec>*>* brute_calculate_all(vec* qvectors,vec* nvectors,int no_of_vectors,int no_of_coordinates,int queries_no_of_vectors,int N){
    vector<vector<dist_vec>*>* dsvec2;
    dsvec2=new vector<vector<dist_vec>*>;
    for(int i=0;i<queries_no_of_vectors;i++){
        dsvec2->push_back(brute_calculate(qvectors,nvectors,no_of_vectors,no_of_coordinates,N,i));
    }
    return dsvec2;
}

