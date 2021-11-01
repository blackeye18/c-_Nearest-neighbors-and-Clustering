#ifndef __knn_ranges_brutes__
#define __knn_ranges_brutes__


long double vect_dist(vector<double> vecA,vector<double> vecB,int d);
//vector<dist_vec>* hypercube::NN_search(vec* qvect,int N);
//vector<vector<dist_vec>*>* hypercube:: all_NN_search(vec* qvectors,int N,int queries_no_of_vectors);
//vector<dist_vec>* hypercube::RANGE_search(vec* qvect,double R);
//vector<vector<dist_vec>*>* hypercube:: all_RANGE_search(vec* qvectors,double R,int queries_no_of_vectors);
vector<dist_vec>* brute_calculate(vec* qvector,vec* nvectors,int no_of_vectors,int no_of_coordinates,int N,int pos);
vector<vector<dist_vec>*>* brute_calculate_all(vec* qvectors,vec* nvectors,int no_of_vectors,int no_of_coordinates,int queries_no_of_vectors,int N);


#endif