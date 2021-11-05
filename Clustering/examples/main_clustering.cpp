#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstring>
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
#include <chrono>

using namespace std;

using namespace std::chrono;

class vec
{
public:
    string name;//to id ths grammhs-dianismatos
    vector <double> coord;
};

long double vect_dist(vector<double> vecA,vector<double> vecB,int d)
{
    long double sum=0;
    for (int i = 0; i < d; ++i)
        {
        long double temp=abs(vecA[i]-vecB[i]);
        sum+=temp*temp;
        }
    return sqrt(sum);
}

class cluster
    {
    private:
        int K_medians;//number_of_clusters
        int L;//number_of_vector_hash_tables
        int k_lsh;//number_of_vector_hash_functions
        int M;//max_number_M_hypercube
        int k_hypercube;//number_of_hypercube_dimensions
        int probes;//number_of_probes
        int no_of_vectors;
        int no_of_coordinates;


    public:
        cluster(int,int,int);
        vector<int>* Kmeanplus(vec* nvect);
        vector<vector<vec*>>* lloyds(vec* nvect,vector<int>* clustersvec);
        vector<long double>* silhouette(vector<vector<vec*>>* cluster_neighbours,vector<int>* clustersvec,vec* nvect);
    };

cluster::cluster(int K_medians,int no_of_vectors,int no_of_coordinates)
:K_medians(K_medians),no_of_vectors(no_of_vectors),no_of_coordinates(no_of_coordinates){};

vector<int>* cluster::Kmeanplus(vec* nvect)
    {
    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
    std::uniform_int_distribution<int>  distrC(0,no_of_vectors-1);

    vector<int>* clusters=new vector<int>;
    clusters->push_back(distrC(e));
    vector<long double> partial_sums; 
    vector<int> r;

    while(clusters->size()<K_medians)
        {
        r.clear();
        partial_sums.clear();
        long double sum=0;
        for (int i = 0; i < no_of_vectors; ++i)
            {
            auto it = find(clusters->begin(),clusters->end(), i);
            if(it==clusters->end())
                {
                long double mdist;
                int mci;
                for (int ci = 0; ci < clusters->size(); ++ci)
                    {
                    int clust=clusters->at(ci);
                    if(ci==0)
                        {
                        mdist=vect_dist(nvect[clust].coord,nvect[i].coord,no_of_coordinates);
                        mci=0;
                        }
                    else
                        {
                        long double dist=vect_dist(nvect[clust].coord,nvect[i].coord,no_of_coordinates);
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

        clusters->push_back(r[position]);
        }
    return clusters;
    }
vector<vector<vec*>>* cluster::lloyds(vec* nvect,vector<int>* clustersvec)
    {
    vector<vector<vec*>>* lloydsclust=new vector<vector<vec*>>;
    lloydsclust->resize(clustersvec->size(),vector<vec*>(0));

    for (int i = 0; i < no_of_vectors; ++i)
        {
        auto it = find(clustersvec->begin(),clustersvec->end(), i);
        if(it==clustersvec->end())
            {
            long double mdist;
            int mci;
            for (int ci = 0; ci < clustersvec->size(); ++ci)
                {
                int clust=clustersvec->at(ci);
                if(ci==0)
                    {
                    mdist=vect_dist(nvect[clust].coord,nvect[i].coord,no_of_coordinates);
                    mci=0;
                    }
                else
                    {
                    long double dist=vect_dist(nvect[clust].coord,nvect[i].coord,no_of_coordinates);
                    if (dist<mdist)
                        {
                        mdist=dist;
                        mci=ci;
                        }
                    }
                }
            ((*lloydsclust)[mci]).push_back(&(nvect[i]));
            }
        }
    return lloydsclust;
    }
vector<long double>* cluster::silhouette(vector<vector<vec*>>* cluster_neighbours,vector<int>* clustersvec,vec* nvect)
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
                    int clust=clustersvec->at(nci);

                    if(nci==0||(ci==0 && nci==1))
                        {
                        nextbestclust_dist=vect_dist(nvect[clust].coord,(*cluster_neighbours)[ci][vi]->coord,no_of_coordinates);
                        nextbestclust_ci=nci;
                        }
                    else
                        {
                        long double dist=vect_dist(nvect[clust].coord,(*cluster_neighbours)[ci][vi]->coord,no_of_coordinates);
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
//elegxoume an to # twn arguments einai swsto
int argsOK(int argc, char *argv[])
{
    if (argc == 10 || argc== 9)
    {
        //printf("Error in # of args\n");
        return 1;
    }
    return 0;
}

int handle_conf_file(char configuration_file[256],int* K_medians,int*L,int*k_lsh,int*M,int*k_hypercube,int*probes){
	ifstream conf;
    conf.open(configuration_file);
    if(!conf){
        cout<<"Something Terrible Happened! Maybe the input_file doesn't exist?! Exiting.."<<endl;
        return 1;
    }

    string sline;
    string tok;
//default values:
    (*L)=3;
    (*k_lsh)=4;
    (*M)=10;
    (*k_hypercube)=3;
    (*probes)=2;
    int count=0;
     while (getline(conf,sline))
    {	count++;
    	stringstream line(sline);
    	string temp1,temp2;
    	line>> tok;
    	temp1=tok;
    	line>> tok;
    	temp2=tok;

    	if(count>6){
    		cout<<"Something went wrong with configuration_file... Exiting.."<<endl;
    		return 1;
    	}
    	//cout<<"temp1: "<<temp1;
    	if(temp1=="number_of_clusters:")
    		(*K_medians)=atoi(temp2.c_str());
    	else if(temp1=="number_of_vector_hash_tables:")
    		(*L)=atoi(temp2.c_str());
    	else if(temp1=="number_of_vector_hash_functions:")
    		(*k_lsh)=atoi(temp2.c_str());
    	else if(temp1=="max_number_M_hypercube:")
    		(*M)=atoi(temp2.c_str());
    	else if(temp1=="number_of_hypercube_dimensions:")
    		(*k_hypercube)=atoi(temp2.c_str());
    	else if(temp1=="number_of_probes:")
    		(*probes)=atoi(temp2.c_str());
    	else {
    		cout<<"Something went wrong with configuration_file... Exiting.."<<endl;
    		return 1;
    	}

    }

return 0;
}

int input_handler(int argc, char *argv[],char (&input_file)[256], char (&configuration_file)[256], char (&output_file)[256],char (&method)[256],int* K_medians,int*L,int*k_lsh,int*M,int*k_hypercube,int*probes,int*complete_flag){

	if(!argsOK(argc,argv)){
		cout<<"You entered something wrong.. Exiting"<<endl;
		return 1;
	}
	if(argc==10){
	
		strcpy(input_file,argv[2]);
		strcpy(configuration_file,argv[4]);
		strcpy(output_file,argv[6]);
		if(strcmp(argv[7],"-complete")==0){
			cout<<"Complete parameter given!"<<endl;
			(*complete_flag)=1;
		}else{
			cout<<"Something is wrong with input parameters... Exiting.."<<endl;
			return 1;
		}
		strcpy(method,argv[9]);
		//diavasma input file 
		//diavasma configuration file
        if(handle_conf_file(configuration_file,K_medians,L,k_lsh,M,k_hypercube,probes))
            return 1;

	}else{
		strcpy(input_file,argv[2]);
		strcpy(configuration_file,argv[4]);
		strcpy(output_file,argv[6]);
		(*complete_flag)=0;
		strcpy(method,argv[8]);
		//diavasma input file 
		if(handle_conf_file(configuration_file,K_medians,L,k_lsh,M,k_hypercube,probes))
			return 1;

	}

return 0;
}

vec* open_and_create_vectors(char input_file[256],int* no_of_coordinates,int *no_of_vectors){
    //cout<<"aaa1";
    int counter=0;
    int flag=0;
    ifstream input;
    input.open(input_file);
    if(!input){
        cout<<"Something Terrible Happened! Maybe the input_file doesn't exist?! Exiting.."<<endl;
        return NULL;
    }

    string sline;
    string tok;


    (*no_of_vectors)=0;
    (*no_of_coordinates)=-1;//-1 giati metrame kai to arxiko id pou den einai coord

    while (getline(input,sline))
    {
        (*no_of_vectors)++;
        
        if((*no_of_vectors)==1){
            stringstream line(sline);

            while(line>> tok)
                (*no_of_coordinates)++;
        }
    }
    input.close();
    input.clear();
    input.open(input_file);
    if(!input){
        cout<<"Something Terrible Happened! Maybe the input_file doesn't exist?! Exiting.."<<endl;
        return NULL;
    }
    vec* nvectors;
    nvectors = new vec[(*no_of_vectors)];
    while (getline(input,sline))
    {       flag=0;
            stringstream line(sline);
            while(line>> tok) {
                if(flag==0){
                    //cout<<"TOK:"<<tok<<endl;
                    nvectors[counter].name=tok;
                    flag=1;
                }
                else
                    nvectors[counter].coord.push_back(stof(tok));
                    
            }
        counter++;
    }
    //cout<<"NEW FUNCTION: no_of_vectors:"<<(*no_of_vectors)<<"no_of_coordinates:"<<(*no_of_coordinates)<<endl;
    return nvectors;
            
}

void print_to_file(vector<int>* clustersvec,vector<vector<vec*>>* cluster_neighbours,int complete_flag,char output_file[256],char method[256],int no_of_coordinates,int no_of_vectors,vec* nvectors,double time1, vector<long double>* silhouette_vec){

    ofstream outfile;
    outfile.open(output_file);
    outfile<<"Algorithm: ";
    if(strcmp(method,"Classic")==0)
        outfile<<"Lloyds"<<endl;
    else if(strcmp(method,"LSH")==0)
        outfile<<"Range Search LSH"<<endl;
    else if(strcmp(method,"Hypercube")==0)
        outfile<<"Range Search Hypercube"<<endl;
    for(int i=0;i<clustersvec->size();i++){
        outfile<<"CLUSTER-"<<i+1<<" {size: "<<(*cluster_neighbours)[i].size()<<", centroid: ";
        for(int j=0;j<no_of_coordinates;j++){
            outfile<<nvectors[i].coord[j]<<" ";
        }
        outfile<<"}"<<endl;
    }
    outfile<<"clustering_time: "<<time1<<endl;
    outfile<<"Silhouette: [";
    double sum=0;
    for(int i=0;i<silhouette_vec->size();i++){
        sum+=(*silhouette_vec)[i];
    }
    sum=sum/silhouette_vec->size();
    for(int i=0;i<silhouette_vec->size();i++){
        outfile<<(*silhouette_vec)[i]<<",";
    }
    outfile<<sum<<"]";
    if(complete_flag==1){
        for(int i=0;i<clustersvec->size();i++){
        outfile<<"CLUSTER-"<<i+1<<" {size: "<<(*cluster_neighbours)[i].size()<<", centroid: ";
        for(int j=0;j<no_of_coordinates;j++){
            outfile<<nvectors[i].coord[j]<<" ";
        }
        //outfile<<",";
       for(int k=0;k<(*cluster_neighbours)[i].size();k++){
        outfile<<", "<<((*cluster_neighbours)[i])[k]->name;
        }
       outfile<<"}";
        } 
    }   
    return;
}

int main(int argc, char *argv[]){
	int complete_flag;
	int K_medians,L,k_lsh,M,k_hypercube,probes;
	char input_file[256],configuration_file[256],output_file[256],method[256];
	vec* nvectors;
	int no_of_vectors,no_of_coordinates;

	if(input_handler(argc,argv,input_file,configuration_file,output_file,method,&K_medians,&L,&k_lsh,&M,&k_hypercube,&probes,&complete_flag))
		return -1;

	cout<<"input_file: "<<input_file<<" configuration_file: "<<configuration_file<<" output_file :"<<output_file<<" method: "<<method<<endl;
	cout<<"K_medians: "<<K_medians<<" L: "<<L<<" k_lsh: "<<k_lsh<<" M: "<<M<<" k_hypercube: "<<k_hypercube<<" probes: "<<probes<<" Complete: "<<complete_flag<<endl;


	nvectors=open_and_create_vectors(input_file,&no_of_coordinates,&no_of_vectors);
	if(nvectors==NULL)
	    return -1;
	printf("Input:: no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);
	
    auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/

    cluster clus(K_medians,no_of_vectors,no_of_coordinates);
    vector<int>* clustersvec;
    clustersvec=clus.Kmeanplus(nvectors);
    cout<<endl<<clustersvec->size()<<endl;
    vector<vector<vec*>>* cluster_neighbours;
    cluster_neighbours=clus.lloyds(nvectors,clustersvec);


    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);
    double time1=((double)duration1.count()/1000000);


    for(int w=0;w<cluster_neighbours->size();w++){
        cout<<(*cluster_neighbours)[w].size()<<endl;
    }

    vector<long double>* silhouette_vec =clus.silhouette(cluster_neighbours,clustersvec,nvectors);
    for (int i = 0; i < silhouette_vec->size(); ++i)
        {
        cout<<"silhouette for cluster: "<<i<<" : "<<(*silhouette_vec)[i]<<endl;
        }

    print_to_file(clustersvec,cluster_neighbours,complete_flag,output_file,method,no_of_coordinates,no_of_vectors,nvectors,time1,silhouette_vec);

	return 0;
}