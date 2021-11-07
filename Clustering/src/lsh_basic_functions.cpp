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
//#include "input_menu_starting_func.hpp"
#include "lsh_basic_functions.hpp"
#include "cube_basic_functions.hpp"
//#include "knn_ranges_brutes.hpp"

double normal_dist_generator(void)
    {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<float> d{0.0,1.0};
    map<int, int> hist{};
    return round(d(gen));
    }

hashtable::hashtable()
    {
    initialized=0;
    }
void hashtable::hashtable_init(int bnum)
    {
    buckets=(node**) malloc(bnum*sizeof(node*));
    for(int i=0;i<bnum;i++)
        buckets[i]=NULL;
    bucket_count=bnum;
    total_nodes=0;
    initialized=1;
    }

int hashtable:: hashtable_insert(vec* nvec, long hvalue)
    {
    if(not initialized){return 1;}
    total_nodes++;
    int id=hvalue % bucket_count;
    if (buckets[id]==NULL)
        {
        buckets[id]=new node;
        buckets[id]->hashvalue=hvalue;
        buckets[id]->id=id;
        buckets[id]->vect=nvec;
        buckets[id]->next=NULL;
        }
    else
        {
        node* tempn= new node;
        tempn->hashvalue=hvalue;
        tempn->id=id;
        tempn->vect=nvec;
        tempn->next=buckets[id];
        buckets[id]=tempn;
        }
    return 0;
    }


void hashtable::hashtable_print()
    {
    cout<<"total_nodes of htable "<<total_nodes;
    int j=0;
    for (int i = 0; i < bucket_count; ++i)
        {
        cout<<endl<<"bucket "<<i;

        node* currnodeptr=buckets[i];

        while(currnodeptr!=NULL)
            {
            cout<<" node "<<j<<" "<<currnodeptr->vect->name<<endl;
            cout<<"cord "<<currnodeptr->vect->coord[0];
            currnodeptr=currnodeptr->next;
            j++;
            }
        }
                 
    }

hashtable::~hashtable()
    {
    if(buckets!=NULL)
        {
        for (int i = 0; i < bucket_count; ++i)
            {
            node* currnd=buckets[i];
            while(currnd!=NULL)
                {
                node* temp=currnd;
                currnd=temp->next;
                delete temp;
                }

            }
        free(buckets);

        }
    buckets=NULL;
    }

Lhashtables:: ~Lhashtables(void){
    delete[] Lhtables;
}

Lhashtables::Lhashtables(int lh,int di,int ki)
 {  
    Lhtables= new hashtable[lh];
    L=lh;
    d=di;
    k=ki;
}

void Lhashtables::Hashfun_init(void)        
    {
    v.resize(L,vector<vector<double> >(k,vector<double>(d)));

    t.resize(L,vector<double>(k));

    r.resize(L,vector<int>(k));

    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);

    std::uniform_int_distribution<int>  distrR(-RLIMIT,RLIMIT);
    std::uniform_real_distribution<double>  distrT(0.0,W);


    for (int Li = 0; Li < L; ++Li)
        {
        for (int ki = 0; ki < k; ++ki)
            {
            t[Li][ki]=distrT(e);    //cout<<"t "<<t[Li][ki]<<endl;
            r[Li][ki]=distrR(e);    //cout<<"r "<<r[Li][ki]<<endl;

            for (int di = 0; di < d; ++di)
                {
                v[Li][ki][di]=normal_dist_generator();
                }
            //cout<<endl;
            }
        }

}


int h_function(vector<double> p,vector<double> v,double t){
    double in_prod;
    in_prod=inner_product(p.begin(),p.end(),v.begin(),0);
    in_prod+=t;
    double sum=in_prod/W;
    int h=floor(sum);
    return h;

}
//https://stackoverflow.com/questions/11714555/euclidean-integer-modulo-in-c
long int euclidean_remainder(long int a,long int b)
{
  assert(b != 0);
  long int r = a % b;
  return r >= 0 ? r : r + abs(b);
}

long int g_function(int h[],vector<int> r,int k){
    long int galmost=0;
    long int count_holder=0;
    for(int i=0;i<k;i++){
        count_holder=0;
        count_holder+=euclidean_remainder(r[i],PNUM);
        count_holder=count_holder*euclidean_remainder(h[i],PNUM);
        count_holder=euclidean_remainder(count_holder,PNUM);
        galmost+=count_holder;
    }
    galmost=euclidean_remainder(galmost,PNUM);
    return galmost;
}

int Lhashtables:: lsh_continue(int no_of_ht,int no_of_vectors, vec* nvectors,vector<int>* clustersvec){
    int h_return;
    int h[this->k];
    int tablesize=no_of_vectors/BUCKET_RATIO;//apo diafaneies
    this->Lhtables[no_of_ht].hashtable_init(tablesize);
    long int g_notablesize;
    for(int i=0;i<no_of_vectors;i++){
        auto it = find(clustersvec->begin(),clustersvec->end(), i);//elegxos gia an to sygkekrimeno dianisma einai cluster
        if(it==clustersvec->end()){
            for(int ki=0;ki<this->k;ki++){

                h_return=h_function(nvectors[i].coord,this->v[no_of_ht][ki],this->t[no_of_ht][ki]);
                h[ki]=h_return;
                //cout<<"H Function Return:"<<h[ki]<<endl;
            }
            //cout<<"calling g function"<<endl;
            g_notablesize=g_function(h,this->r[no_of_ht],this->k);
            //cout<<"Inserting to HT"<<endl;
            this->Lhtables[no_of_ht].hashtable_insert(&(nvectors[i]),g_notablesize);
        }
    }
    

    return 0;
}

int Lhashtables:: lsh_start(int no_of_vectors,vec *nvectors,vector<int>* clustersvec){
    this->Hashfun_init();
    int ret=0;
    for(int i=0;i<L;i++){
        ret=this->lsh_continue(i,no_of_vectors,nvectors,clustersvec);
        if(ret!=0){
            cout<<"Something went wrong with lsh_continue"<<endl;
            return-1;
        }

    }
    /*
    cout<<endl<<endl<<endl<<"PRINTING HASHTABLES:"<<endl;
    for(int i=0;i<L;i++){
        cout<<"hashtable "<<i<<endl;
        this->Lhtables[i].hashtable_print();
        cout<<endl<<endl;
    }
*/
    return 0;

}

