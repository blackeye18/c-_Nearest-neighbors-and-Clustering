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


unsigned int hammingDistance(unsigned int n1, unsigned int n2)
	{
    unsigned int x = n1 ^ n2;
    unsigned int setBits = 0;
 
    while (x > 0) {
        setBits += x & 1;
        x >>= 1;
    }
 
    	return setBits;
	}
double normal_dist_generator(void)
    {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<float> d{0.0,1.0};
    map<int, int> hist{};
    return round(d(gen));
    }
//https://stackoverflow.com/questions/11714555/euclidean-integer-modulo-in-c
long int euclidean_remainder(long int a,long int b)
{
  assert(b != 0);
  long int r = a % b;
  return r >= 0 ? r : r + abs(b);
}

int h_function(vector<double> p,vector<double> v,double t){
    double in_prod;
    in_prod=inner_product(p.begin(),p.end(),v.begin(),0);
    in_prod+=t;
    double sum=in_prod/W;
    int h=floor(sum);
    return h;

}

hypercube::hypercube(int M,int probes,int d ,int k,int no_of_vectors)
	:M(M),probes(probes),d(d),k(k),no_of_vectors(no_of_vectors)
	{
	labels_count=no_of_vectors/BUCKET_RATIO;
	}

void hypercube::Hashfun_init(void)        
    {
    v.resize(k,vector<double> (d));

    labels.resize(k,vector<int>(labels_count));


    cube_vec.resize(powk,vector<vec*>(0));

    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);

    std::uniform_int_distribution<int>  distrl(0,1);
    std::uniform_real_distribution<double>  distrT(0.0,W);

    for (int ki = 0; ki < k; ++ki)
        {
        t.push_back(distrT(e));

        for (int di = 0; di < d; ++di)
            {
            v[ki][di]=normal_dist_generator();
            }

        for (int la = 0; la < labels_count; ++la)
            {
            labels[ki][la]=distrl(e);
            }
        }

    }
unsigned int hypercube::hash_calc(vec* vect)
    {
    unsigned int bits=0;
    for (int ki = 0; ki < k; ++ki)
        {
        int h=h_function(vect->coord,v[ki],t[ki]);
        //cout<<"labels_count: "<<labels_count<<endl;
        int hmod=euclidean_remainder(h,labels_count);
        bits=bits<<1;
        //cout<<labels[ki][hmod];
        bits+=labels[ki][hmod];
        }
      //cout<<endl;
    return bits;
    }
int hypercube::vec_insert(vec* vect)
    {
    unsigned int ki=this->hash_calc(vect);
    //cout<<"inserting "<<ki<<endl;
    //cube_map.insert(pair <vector<bool>,vec*> (this->hash_calc(vect),vect));
    cube_vec[ki].push_back(vect);
    return 0;
    }

void hypercube::cube_start(int no_of_vectors,vec*nvectors){
	powk=pow(2,k);
	this->Hashfun_init();
	int ret=0;
	for(int i=0;i<no_of_vectors;i++){
		ret=vec_insert(&(nvectors[i]));
		if(ret!=0){
            cout<<"Something went wrong with vec_insert"<<endl;
            return;
        }
	}
}