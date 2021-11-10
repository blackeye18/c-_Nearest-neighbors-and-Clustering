/* Georgios Georgiou sdi1800220 - Panagiotis Mavrommatis sdi1800115 */
#ifndef __knn_ranges_brutes__
#define __knn_ranges_brutes__


long double vect_dist(vector<double> vecA,vector<double> vecB,int d);//ipologizei tin euklidia apostasi meta3i dio vectors(double) *to d ine i diastasi twn vectors 
//vector<dist_vec>* hypercube::NN_search(vec* qvect,int N);
//vector<vector<dist_vec>*>* hypercube:: all_NN_search(vec* qvectors,int N,int queries_no_of_vectors);
//vector<dist_vec>* hypercube::RANGE_search(vec* qvect,double R);
//vector<vector<dist_vec>*>* hypercube:: all_RANGE_search(vec* qvectors,double R,int queries_no_of_vectors);
vector<dist_vec>* brute_calculate(vec* qvector,vec* nvectors,int no_of_vectors,int no_of_coordinates,int N,int pos);//ipologismos nearest neighbours gia ena query vector me brute calculate
vector<vector<dist_vec>*>* brute_calculate_all(vec* qvectors,vec* nvectors,int no_of_vectors,int no_of_coordinates,int queries_no_of_vectors,int N,double*time_per_query_brute);//ipologismos nearest neighbours gia ena array apo query vectors me brute calculate xrisimopoiontas tin pio panw sinartisi


#endif