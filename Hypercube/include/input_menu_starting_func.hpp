#ifndef __input_menu_starting_func__
#define __input_menu_starting_func__
//sxolia gia tis pio katw entoles iparxun stin lsh
int argsOK(int argc, char *argv[]);
int input_handler(int argc, char *argv[],int* k, int* M,int *probes ,int* N,double* R,char (&input_file)[256], char (&query_file)[256], char (&output_file)[256]);
vec* open_and_create_vectors(char input_file[256],int* no_of_coordinates,int *no_of_vectors);
int repeat_handler(vec* nvectors, vec* qvectors,char* input_file,char*query_file,char* output_file,hypercube *cube);
int print_to_file(char output_file[256],string lsh_or_hypercube,vector<vector<dist_vec>*>* dsvec2,int queries_no_of_vectors,vec* qvectors,double* time_per_query_cube,double* time_per_query_brute,vector<vector<dist_vec>*>* dsvec3,vector<vector<dist_vec>*>* dsvec4,double R);




#endif