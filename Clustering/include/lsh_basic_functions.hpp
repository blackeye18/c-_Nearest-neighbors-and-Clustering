#ifndef __lsh_basic_functions__
#define __lsh_basic_functions__

double normal_dist_generator(void);
//hashtable::hashtable();
//void hashtable::hashtable_init(int bnum);
//int hashtable:: hashtable_insert(vec* nvec, long hvalue);
//void hashtable::hashtable_print();
//hashtable::~hashtable();
//Lhashtables:: ~Lhashtables(void);
//Lhashtables::Lhashtables(int lh,int di,int ki);
//void Lhashtables::Hashfun_init(void);
int h_function(vector<double> p,vector<double> v,double t);
long int euclidean_remainder(long int a,long int b);
long int g_function(int h[],vector<int> r,int k);
//int Lhashtables:: lsh_continue(int no_of_ht,int no_of_vectors, vec* nvectors);
//int Lhashtables:: lsh_start(int no_of_vectors,vec *nvectors);

#endif