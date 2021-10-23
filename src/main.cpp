#include <iostream>
#include <cstring>
#include <vector>
#include <cctype>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <map>

using namespace std;

#define RLIMIT 100
#define PNUM 4294967291
#define W 4//meta3i 0 kai 6


class vec
{
public:
    string name;//to id ths grammhs-dianismatos
    vector <int> coord;
};


double normal_dist_generator(void)
    {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<float> d{0.0,1.0};
    map<int, int> hist{};
    return round(d(gen));
    }


class node
    {
    public:
        long long hashvalue;
        int id;
        vec* vect;
        node* next;
        ~node(void);
    };
node::~node()
    {
    if(vect!=NULL)
        {
        delete vect;
        vect=NULL;
        }
    if(next!=NULL)
        {
        delete next;
        next=NULL;
        }
    }

class hashtable
    {
    protected:
        node* buckets;//array me ta buckets
        int bucket_count;//ari8mos buckets
        int total_nodes;
        int initialized;
    public:
        hashtable(void); // plithos bucket
        void hashtable_init(int);
        int hashtable_insert(vec* , long long );//eisagwgh 
        ~hashtable(void);
    };

hashtable::hashtable()
    {
    initialized=0;
    }
void hashtable::hashtable_init(int bnum)
    {
    buckets= new node[bnum];
    bucket_count=bnum;
    total_nodes=0;
    initialized=1;
    }

int hashtable:: hashtable_insert(vec* nvec, long long hvalue)
    {
    if(not initialized){return 1;}
    total_nodes++;
    int id=hvalue % bucket_count;
    if (buckets[id].vect==NULL)
        {
        buckets[id].hashvalue=hvalue;
        buckets[id].id=id;
        buckets[id].vect=nvec;
        buckets[id].next=NULL;
        }
    else
        {
        node* tempn= new node;
        tempn->hashvalue=hvalue;
        tempn->id=id;
        tempn->vect=nvec;
        tempn->next=buckets[id].next;
        buckets[id].next=tempn;
        }
    return 0;
    }

hashtable::~hashtable()
    {
    delete[] buckets;
    buckets=NULL;
    }
class Lhashtables
    {
    protected:
        hashtable* Lhtables;//array me ta hashtables
        int L;//number of hashtables;
        int d;//dimensions
        int k;
        vector<vector<vector<double>>> v;
        vector<vector<double>> t;
        vector<vector<int>> r;
    public:
        Lhashtables(int,int,int);
        void Hashfun_init(void);
        int lsh_start(int,vec*);
        int lsh_continue(int,int,vec*);
        //~Lhashtables(void);

    };

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
            cout<<endl;
            }
        }

    }

int h_function(vector<int> p,vector<double> v,double t,int d){
    double in_prod;
    in_prod=inner_product(p.begin(),p.end(),v.begin(),0);
    in_prod+=t;
    double sum=in_prod/W;
    int h=floor(sum);
    return h;

}
int Lhashtables:: lsh_continue(int no_of_ht,int no_of_vectors, vec* nvectors){
    int h_return;
    int h[this->k];
    for(int i=0;i<no_of_vectors;i++){
        for(int ki=0;ki<this->k;ki++){

            h_return=h_function(nvectors[i].coord,this->v[no_of_ht][ki],this->t[no_of_ht][ki],this->d);
            h[ki]=h_return;
            cout<<"H Function Return:"<<h[ki]<<endl;
        }
    }
    return 0;
}

int Lhashtables:: lsh_start(int no_of_vectors,vec *nvectors){
    this->Hashfun_init();
    int ret=0;
    for(int i=0;i<L;i++){
        ret=this->lsh_continue(i,no_of_vectors,nvectors);
        if(ret!=0){
            cout<<"Something went wrong with lsh_continue"<<endl;
            return-1;
        }

    }


    return 0;

}

//elegxoume an to # twn arguments einai swsto
int argsOK(int argc, char *argv[])
{
    if (argc != 15)
    {
        //printf("Error in # of args\n");
        return 0;
    }
    return 1;
}

int input_handler(int argc, char *argv[],int* k, int* L, int* N,int* R,char (&input_file)[256], char (&query_file)[256], char (&output_file)[256]){
    char temp[256];
    if(argsOK(argc,argv)){
        printf("All parameters given from command line...\n");
        strcpy((input_file),argv[2]);
        strcpy((query_file),argv[4]);
        strcpy((output_file),argv[10]);
        *(k)=atoi(argv[6]);
        *(L)=atoi(argv[8]);
        *(N)=atoi(argv[12]);
        *(R)=atoi(argv[14]);
    }else{
        printf("Parameters were NOT given from command line, please input them to contiue!\n");
        //default times
        *(k)=4;
        *(L)=5;
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

        printf("Type the value of k or type d for default value(4) ");
        scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(k)=4;
        else{
            if(isdigit(temp[0]))
                *(k)=atoi(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");
                return(1);
            }
        }
        strcpy(temp,"");

        printf("Type the value of L or type d for default value(5) ");
        scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(L)=5;
        else{
            if(isdigit(temp[0]))
                *(L)=atoi(temp);
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
                *(R)=atoi(temp);
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
                    nvectors[counter].coord.push_back(stoi(tok));
                    
            }
        counter++;
    }
    //cout<<"NEW FUNCTION: no_of_vectors:"<<(*no_of_vectors)<<"no_of_coordinates:"<<(*no_of_coordinates)<<endl;
    return nvectors;
            
}

void print_vectors(vec *nvectors,int no_of_vectors,int no_of_coordinates){
    for(int i=0;i<no_of_vectors;i++){
        cout<<"Dianisma :"<<i<<endl;
        for(int j=0;j<no_of_coordinates;j++){
        //for(auto j: vector[i].coord){
            //cout<<j<<' ';
            if(j==0)
                cout<<"Name-id: "<<nvectors[i].name<<endl;
            
            cout<<nvectors[i].coord[j]<<" ";
        }
    }
    cout<<endl;
}




int main(int argc, char *argv[]){
    char input_file[256],query_file[256],output_file[256],temp[256];
    int k,L,N,R;//allagi tou R se dekadiko arithmo
    int no_of_vectors,no_of_coordinates;
    int ret;

	if(input_handler(argc,argv,&k,&L,&N,&R,(input_file), query_file, output_file))
        return -1;

    printf("input_file: %s, query_file: %s, output_file: %s,k:%d,L:%d,N:%d,R:%d\n",input_file,query_file,output_file,k,L,N,R);

/*
    ret=get_N_of_input(&no_of_vectors,&no_of_coordinates,input_file);
    if(ret==-1){
        printf("Something terrible happened while opening the file... Exiting...\n");
        return -1;
    }
    printf("no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);*/
    vec* nvectors;

    nvectors=open_and_create_vectors(input_file,&no_of_coordinates,&no_of_vectors);
    if(nvectors==NULL)
        return -1;
    printf("no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);
   // print_vectors(nvectors,no_of_vectors,no_of_coordinates);
    Lhashtables lht(L,no_of_coordinates,k);
    lht.lsh_start(no_of_vectors,nvectors);
   
cout<<"den krasaraaaa"<<endl;

   delete [] nvectors;
		
}