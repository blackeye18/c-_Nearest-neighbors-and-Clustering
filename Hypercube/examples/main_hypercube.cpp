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

#define PNUM 4294967291
#define BUCKET_RATIO 16
#define W 4//meta3i 0 kai 6
const string metric = "euclidean_distance";



class vec
{
public:
    string name;//to id ths grammhs-dianismatos
    vector <double> coord;
};


class dist_vec
    {
    public:
    long double dist;
    vec* vect;
    dist_vec(long double dist, vec* vect)
       : dist(dist), vect(vect)
        {};
    //~dist_vec(void);
    };

struct pqcompare
    {
    bool operator()(dist_vec const& NN1, dist_vec const& NN2)
        {
        return NN1.dist > NN2.dist;
        }
    };
//new
class hypercube
	{
	private:
		vector<vector<double>> v;
    	vector<double> t;
    	vector<vector<int>> labels;

    	vector<vector<vec*>> cube_vec;

    	//int L;//number of hashtables;
    	int d;//dimensions
    	int k;//diastasi pu 8a provalonte ta simeia
    	int M;//maximum cout of points checked
    	int probes;//maximum count of cube_map checked
    	int no_of_vectors;
    	int labels_count;
    	unsigned int powk;

	public:
		hypercube(int M,int probes,int d ,int k,int no_of_vectors);
		//~hypercube();
		void Hashfun_init(void);//initializes tables used for hashing 
		unsigned int hash_calc(vec* vect);//returns the bin cube vector point
		int vec_insert(vec* vect);//inserts a vector to the multimap
		void cube_start(int ,vec*);
		vector<vector<dist_vec>*>* all_NN_search(vec* ,int ,int );
		vector<dist_vec>* NN_search(vec*,int);
        vector<dist_vec>* RANGE_search(vec* ,double );
        vector<vector<dist_vec>*>* all_RANGE_search(vec* ,double ,int );
		//void cube_map_print();
	
	};

//functions


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

int hamming_distance(vector<bool> bitsa,vector<bool> bitsb)
    {
    int hd=0;
    for (int i = 0; i < bitsa.size(); ++i)
        {
        if(bitsa[i]!=bitsb[i])
            hd++;
        }
    return hd;
    }
//hypercube functions
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



//elegxoume an to # twn arguments einai swsto
int argsOK(int argc, char *argv[])
{
    if (argc == 17 || argc== 7)
    {
        //printf("Error in # of args\n");
        return 1;
    }
    return 0;
}

int input_handler(int argc, char *argv[],int* k, int* M,int *probes ,int* N,double* R,char (&input_file)[256], char (&query_file)[256], char (&output_file)[256]){
    char temp[256];
    if(argsOK(argc,argv)){
        if(argc==17){
            printf("All parameters given from command line...\n");
            strcpy((input_file),argv[2]);
            strcpy((query_file),argv[4]);
            strcpy((output_file),argv[12]);
            *(k)=atoi(argv[6]);
            if(*(k)==0)//bad user safety
                *(k)=1;
            *(M)=atoi(argv[8]);
            if(*(M)==0)
                *(M)=1;
            *(probes)=atoi(argv[10]);
            if(*(probes)==0)
            	*(probes)=1;
            *(N)=atoi(argv[14]);
            *(R)=atof(argv[16]);
        }else{
            printf("Almost all parameters given from command line... Using default for k and L\n");
            strcpy((input_file),argv[2]);
            strcpy((query_file),argv[4]);
            strcpy((output_file),argv[6]);
            *(k)=14;
            *(M)=10;
            *(probes)=2;
            *(N)=1;
            *(R)=10000;

        }
    }else{
        printf("Parameters were NOT given from command line, please input them to contiue!\n");
        //default times
        *(k)=14;
        *(M)=10;
        *(probes)=2;
        *(N)=1;
        *(R)=10000;
        printf("Type the path to the input_file: ");
        scanf("%s",(input_file));
        //printf("\n");
        printf("Type the path to the query_file: ");
        scanf("%s",(query_file));
        //printf("\n");
        printf("Type the path to the output_file: ");
        scanf("%s",(output_file));

        printf("Type the value of k or type d for default value(14) ");
        scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(k)=14;
        else{
            if(isdigit(temp[0]))
                *(k)=atoi(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");
                return(1);
            }
        }
        strcpy(temp,"");

        printf("Type the value of M or type d for default value(10) ");
        scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(M)=10;
        else{
            if(isdigit(temp[0]))
                *(M)=atoi(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");
                return(1);
            }
        }
        strcpy(temp,"");

        printf("Type the value of probes or type d for default value(2) ");
        scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(probes)=2;
        else{
            if(isdigit(temp[0]))
                *(probes)=atoi(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");
                return(1);
            }
        }
        strcpy(temp,"");

        printf("Type the value of N or type d for default value(1) ");
         scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(N)=1;
        else{
             if(isdigit(temp[0]))
                *(N)=atoi(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");
                return(1);
            }
        }
        strcpy(temp,"");
        printf("Type the value of R or type d for default value(10000) ");
         scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(R)=10000;
        else{
             if(isdigit(temp[0]))
                *(R)=atof(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");
                return(1);
            }
        }
        strcpy(temp,"");
        //printf("\n");
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


int repeat_handler(vec* nvectors, vec* qvectors,char* input_file,char*query_file,char* output_file,hypercube *cube){//,Lhashtables *lht
    cout<<"To end programm type 0, To repeat with new query_file and input_file type 1, To repeat with new input_file type 2, To repeat with new query_file type 3 : "<<endl;
    int input;
    char temp[256];
    char scanret[256];
    while(1){
        strcpy(temp,"");
        strcpy(scanret,"");
        scanf("%s",scanret);
        if(isdigit(scanret[0]))
            input=stoi(scanret);
        else
            input=-5;
        if(input==0){
            return -1;
        }
        else if(input==1){
            
            cout<<"Please enter input_file"<<endl;
            scanf("%s",temp);
            strcpy(input_file,temp);
            cout<<"Please enter query_file"<<endl;
            strcpy(temp,"");
            scanf("%s",temp);
            strcpy(query_file,temp);
            cout<<"Please enter output_file"<<endl;
            strcpy(temp,"");
            scanf("%s",temp);
            strcpy(output_file,temp);
            delete [] nvectors;
            delete [] qvectors;
            delete cube;
            return 0;
        }else if(input ==2){
            cout<<"Please enter input_file"<<endl;
            scanf("%s",temp);
            strcpy(input_file,temp);
            cout<<"Please enter output_file"<<endl;
            strcpy(temp,"");
            scanf("%s",temp);
            strcpy(output_file,temp);
            delete [] nvectors;
            delete cube;
            return 1;
        }else if(input==3){
            cout<<"Please enter query_file"<<endl;
            scanf("%s",temp);
            strcpy(query_file,temp);
            cout<<"Please enter output_file"<<endl;
            strcpy(temp,"");
            scanf("%s",temp);
            strcpy(output_file,temp);
            delete [] qvectors;
            return 2;
        }else cout<<"You typed something wrong, please try again!"<<endl;
    }

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

int print_to_file(char output_file[256],string lsh_or_hypercube,vector<vector<dist_vec>*>* dsvec2,int queries_no_of_vectors,vec* qvectors,double time_lsh,double time_true,vector<vector<dist_vec>*>* dsvec3,vector<vector<dist_vec>*>* dsvec4,double R){
    ofstream outfile;
    outfile.open(output_file);

    for(int i=0;i<queries_no_of_vectors;i++){
        outfile<<"Query: ";
        outfile<<qvectors[i].name;
        outfile<<endl;
        vector<dist_vec>* dstemp2 =(*dsvec2)[i];
        vector<dist_vec>* dstemp3 =(*dsvec3)[i];
        
        int size2=dstemp2->size();

        for(int j=0;j<size2;j++){
        	dist_vec ds2=(*dstemp2)[j];
        	dist_vec ds3=(*dstemp3)[j];
        	

            outfile<<"Nearest neighbour-";
            outfile<<j+1<<": "<<ds2.vect->name<<endl;
            outfile<<lsh_or_hypercube;
            outfile<<ds2.dist<<endl;
            outfile<<"distanceTrue: "<<ds3.dist<<endl;
        }
        
    
        outfile<<"tLSH: ";
        outfile<<time_lsh<<endl;
        outfile<<"tTrue: "<<time_true<<endl;
        outfile<<R<<"-near neighbours:"<<endl;//den kserw an thelei R h thelei to noumero tou R
        
        vector<dist_vec>* dstemp4 =(*dsvec4)[i];
        int size4=dstemp4->size();
        for(int r=0;r<size4;r++)
        	{
        	dist_vec ds4=(*dstemp4)[r];
            outfile<<ds4.vect->name<<endl;
        	}
        
    }
    outfile.close();

    return 0;
}




int main(int argc, char *argv[]){
	char input_file[256],query_file[256],output_file[256];
	int k,M,probes,N;
    double R;
    int no_of_vectors,no_of_coordinates;
    int queries_no_of_vectors,queries_no_of_coordinates;
    int ret;
    vec* nvectors;
    vec* qvectors;
    hypercube *cube;
    string lsh_or_hypercube="distanceHypercube: ";	

    if(input_handler(argc,argv,&k,&M,&probes,&N,&R,(input_file), query_file, output_file))
        return -1;

    printf("input_file: %s, query_file: %s, output_file: %s,k:%d,M:%d,probes:%d,N:%d,R:%f\n",input_file,query_file,output_file,k,M,probes,N,R);

    int flag=0;
    while(flag!=-1){

        if(flag==0 || flag==1){
            nvectors=open_and_create_vectors(input_file,&no_of_coordinates,&no_of_vectors);
            if(nvectors==NULL)
                return -1;
            printf("Input:: no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);
        }

        if(flag==0 || flag==2){
            qvectors=open_and_create_vectors(query_file,&queries_no_of_coordinates,&queries_no_of_vectors);  
            if(nvectors==NULL)
                return -1;
            printf("Queries:: queries_no_of_vectors: %d, queries_no_of_coordinates: %d\n",queries_no_of_vectors,queries_no_of_coordinates);
        }
        cout<<"Now using hypercube and KNN"<<endl;
        auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        if(flag==0 || flag==1){
            cube=new hypercube(M,probes,no_of_coordinates,k,no_of_vectors);
            cube->cube_start(no_of_vectors,nvectors);
            //->cube_map_print();
        }
        vector<vector<dist_vec>*>* dsvec2;
        dsvec2=cube->all_NN_search(qvectors,N,queries_no_of_vectors);
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        double time1=((double)duration1.count()/1000000);
        if(dsvec2==NULL){
            delete cube;
            delete [] nvectors;
            delete [] qvectors;
            //cout<<"No function for metric:"<<metric<<endl;
            return -1;
        }


        cout<<"Now using brute calculation"<<endl;
        auto start2 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        vector<vector<dist_vec>*>* dsvec3;
        dsvec3=brute_calculate_all(qvectors,nvectors,no_of_vectors,no_of_coordinates,queries_no_of_vectors,N);
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        double time2=((double)duration2.count()/1000000);


        cout<<"Now using Radius Search"<<endl;
        vector<vector<dist_vec>*>* dsvec4;
        dsvec4=cube->all_RANGE_search(qvectors,R,queries_no_of_vectors);
        print_to_file(output_file,lsh_or_hypercube,dsvec2,queries_no_of_vectors,qvectors,time1,time2,dsvec3,dsvec4,R);

        cout<<"Output File Created!!"<<endl;
        flag=repeat_handler(nvectors,qvectors,input_file,query_file,output_file,cube);//,lht
    	
    	for(int i=0;i<queries_no_of_vectors;i++){

        vector<dist_vec>* dstemp2 =(*dsvec2)[i];
        vector<dist_vec>* dstemp3 =(*dsvec3)[i];
        vector<dist_vec>* dstemp4 =(*dsvec4)[i];

        dstemp2->clear();delete dstemp2;
        dstemp3->clear();delete dstemp3;
        dstemp4->clear();delete dstemp4;
    	}
    

	    dsvec2->clear();delete dsvec2;
	    dsvec3->clear();delete dsvec3;
	    dsvec4->clear();delete dsvec4;



    }
    delete cube;
    delete [] nvectors;
    delete [] qvectors;
}