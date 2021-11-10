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
#include <algorithm>


using namespace std;
using namespace std::chrono;


#include "classes_and_defines.hpp"
#include "input_menu_starting_func.hpp"
#include "lsh_basic_functions.hpp"
#include "cube_basic_functions.hpp"
#include "cluster_main_functions_silhouette.hpp"
//#include "knn_ranges_brutes.hpp"

long double Mean_coord(vector<vec*> nvects,vec*  cvec,int d)
 	{
 	long double Avg_diff=0;
 	long double Sum_diff=0;
 	for (int di = 0; di < d; ++di)
 		{
 		double sum=0;
 		for (int ni = 0; ni < nvects.size(); ++ni)
 			{
 			sum+=nvects[ni]->coord[di];
 			}

 		double Avg=sum/nvects.size();
 		Sum_diff+=abs(cvec->coord[di]-Avg);
 		cvec->coord[di]=Avg;
 		}
 	Avg_diff=Sum_diff/d;
 	return Avg_diff;
 	}



cluster::cluster(int K_medians,int no_of_vectors,int no_of_coordinates)
:K_medians(K_medians),no_of_vectors(no_of_vectors),no_of_coordinates(no_of_coordinates){};

vector<vec>* cluster::Kmeanplus(vec* nvect)
    {
    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
    std::uniform_int_distribution<int>  distrC(0,no_of_vectors-1);

    //vector<int>* clusters=new vector<int>;
    int first_clust=distrC(e);
    //nvect[first_clust].clustered_flag=-2;
    vector<vec>*clusters=new vector<vec>;
    clusters->resize(K_medians);
    int counter=0;
    //vec* temp_vect=new vec[K_medians];

    for(int i=0;i<K_medians;i++)
        (clusters->at(i)).coord.resize(no_of_coordinates);

    (clusters->at(counter)).coord=nvect[first_clust].coord;

    counter++;
    //clusters->push_back(first_clust);
    vector<long double> partial_sums; 
    vector<int> r;

    while(counter<K_medians)
        {
        r.clear();
        partial_sums.clear();
        long double sum=0;
        for (int i = 0; i < no_of_vectors; ++i)
            {
            //auto it = find(clusters->begin(),clusters->end(), i);
            int tempflag=0;
            for(int k=0;k<counter;k++){
                if(nvect[i].coord==(clusters->at(counter)).coord)
                    tempflag=1;
            }
            if(tempflag==0)
                {
                long double mdist;
                int mci;
                for (int ci = 0; ci < counter; ++ci)
                    {
                    //int clust=clusters->at(ci);
                    if(ci==0)
                        {
                        mdist=vect_dist((clusters->at(counter)).coord,nvect[i].coord,no_of_coordinates);
                        mci=0;
                        }
                    else
                        {
                        long double dist=vect_dist((clusters->at(counter)).coord,nvect[i].coord,no_of_coordinates);
                        if (dist<mdist)
                            {
                            mdist=dist;
                            mci=ci;
                            }
                        }
                    }
                sum+=mdist*mdist;
                partial_sums.push_back(sum);
                r.push_back(i);
                }
            }
        std::uniform_real_distribution<long double>  distrX(0,sum);
        long double X=distrX(e);
        int position=upper_bound(partial_sums.begin(), partial_sums.end(), X)-partial_sums.begin();
        
        if(position>0)
            if(partial_sums[position-1]>=X)
                position--;
            //nvect[r[position]].clustered_flag=-2;
        //clusters->push_back(r[position]);
        (clusters->at(counter)).coord=nvect[r[position]].coord;
        counter++;
        }
    return clusters;
    }


vector<vector<vec*>>* cluster::lloyds(vec* nvect,vector<vec>* clustersvec)
    {
    vector<vector<vec*>>* lloydsclust=new vector<vector<vec*>>;
    lloydsclust->resize(clustersvec->size(),vector<vec*>(0));

    for (int i = 0; i < no_of_vectors; ++i)
        {

        long double mdist;
        int mci;
        for (int ci = 0; ci < clustersvec->size(); ++ci)
            {

            if(ci==0)
                {
                mdist=vect_dist(clustersvec->at(ci).coord,nvect[i].coord,no_of_coordinates);
                mci=0;
                }
            else
                {
                long double dist=vect_dist(clustersvec->at(ci).coord,nvect[i].coord,no_of_coordinates);
                if (dist<mdist)
                    {
                    mdist=dist;
                    mci=ci;
                    }
                }
            }
        ((*lloydsclust)[mci]).push_back(&(nvect[i]));  
        }
    return lloydsclust;
    }

vector<vector<vec*>>* cluster::repeat(vec* nvect,vector<vec>* clustersvec,int method,void* ss)
	{
	vector<vector<vec*>>* cluster_neighbours=NULL;
	long double diff=0;
	int iteration=0;
	do
		{
		iteration++;
		//to kanw free an den ine NULL
		if(cluster_neighbours!=NULL)
			{
			for (int ci = 0; ci < cluster_neighbours->size(); ++ci)
        		{
        		(*cluster_neighbours)[ci].clear();
        		}
    		cluster_neighbours->clear();
    		delete cluster_neighbours;
    		cluster_neighbours=NULL;
			}

		long double sum_diff=0;
		//analoga me tin methodo vazume ta simeia sta clusters
		if(method==0)
			{
			cluster_neighbours=this->lloyds(nvect,clustersvec);
			}
		else if(method==1)
			{
			Lhashtables* lhtables=(Lhashtables*) ss;
			cluster_neighbours=lhtables->ANN_lsh(nvect,clustersvec,no_of_vectors);
			}
		else if (method==2)
			{
			//edw ine gia hypercube
            hypercube* hcube=(hypercube*) ss;
            cluster_neighbours=hcube->ANN_cube(nvect,clustersvec,no_of_vectors);
			}

		//vriskume ta nea clusters
		for (int ci = 0; ci < clustersvec->size(); ++ci)
			{

			sum_diff+=Mean_coord(cluster_neighbours->at(ci),&(clustersvec->at(ci)),no_of_coordinates);

			//cout<<clustersvec->at(ci).coord[ci]<<endl;
			}
			
		diff=sum_diff/clustersvec->size();//cout<<"diff "<<diff<<endl;
		}
	while(diff>=PERCISION && iteration<MAXIT);
	return cluster_neighbours;
	}
	
vector<long double>* cluster::silhouette(vector<vector<vec*>>* cluster_neighbours,vector<vec>* clustersvec,vec* nvect)
    {
    vector<long double>* S=new vector<long double>;


    for (int ci = 0; ci < cluster_neighbours->size(); ++ci)
        {
         long double sumSi=0;
        int cluster_size=(*cluster_neighbours)[ci].size();
        vector<vector<long double>> prev_dist;
        prev_dist.resize(cluster_size,vector<long double>(cluster_size));

        for (int vi = 0; vi <cluster_size ; ++vi)
            {
            //ipologizoume to  A(i)
            long double sumA=0;
            for (int ni = 0; ni <cluster_size ; ++ni)//simeio sto idio cluster
                {
                if(ni!=vi)
                    {
                    long double dist;
                    if(vi<ni)
                        {
                        dist=vect_dist((*cluster_neighbours)[ci][vi]->coord,(*cluster_neighbours)[ci][ni]->coord,no_of_coordinates);
                        prev_dist[vi][ni]=dist;
                        }
                    else
                        dist=prev_dist[ni][vi];

                    sumA+=dist;
                    }
                }
            long double Avga=sumA/(cluster_size-1);

            //ipologizoume to 2o pio kontino cluster
            long double nextbestclust_dist;
            int nextbestclust_ci;
            //cout<<"ok2"<<endl;
            for (int nci = 0; nci < cluster_neighbours->size(); ++nci)
                {
                if(nci!=ci)
                    {
                    //int clust=clustersvec->at(nci);

                    if(nci==0||(ci==0 && nci==1))
                        {
                        nextbestclust_dist=vect_dist(clustersvec->at(nci).coord,(*cluster_neighbours)[ci][vi]->coord,no_of_coordinates);
                        nextbestclust_ci=nci;
                        }
                    else
                        {
                        long double dist=vect_dist(clustersvec->at(nci).coord,(*cluster_neighbours)[ci][vi]->coord,no_of_coordinates);
                        if(dist<nextbestclust_dist)
                            {
                            nextbestclust_dist=dist;
                            nextbestclust_ci=nci;
                            }
                        }
                    }
                }
            //cout<<"ok3"<<endl;
            //ipologizoume to b(i)
            long double sumB=0;

            int Bcluster_size=(*cluster_neighbours)[nextbestclust_ci].size();

            //cout<<"ok4"<<endl;
            for (int nbvi = 0; nbvi <Bcluster_size ; ++nbvi)//ipologizoume to  B(i)
                {

                long double dist=vect_dist((*cluster_neighbours)[ci][vi]->coord,(*cluster_neighbours)[nextbestclust_ci][nbvi]->coord,no_of_coordinates);
                sumB+=dist;
                }
            //cout<<"ok5"<<endl;
            long double Avgb=sumB/Bcluster_size;

            long double si=(Avgb-Avga)/max(Avga,Avgb);//cout<<"si"<<si<<endl;
            sumSi+=si;
            }
        long double AvgSi=sumSi/cluster_size;
        S->push_back(AvgSi);
        }
    return S;
    }