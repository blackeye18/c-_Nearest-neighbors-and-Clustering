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
#include <chrono>
#include <queue>

using namespace std;
using namespace std::chrono;

#define RLIMIT 1000
#define PNUM 4294967291
#define W 4//meta3i 0 kai 6

double normal_dist_generator(void);
long int euclidean_remainder(long int a,long int b);
int h_function(vector<double> p,vector<double> v,double t);
long int g_function(int h[],vector<int> r,int k);
long double vect_dist(vector<double> vecA,vector<double> vecB,int d);//euclydian apostasi meta3i  2 vectors

class vec
{
public:
    string name;//to id ths grammhs-dianismatos
    vector <double> coord;
};
/*
struct LinkedList{
    int dist;
    vec* vect=NULL;
    LinkedList *next=NULL;
};

 insert_in_list(LinkedList*head,int dist,vec* vect){
    LinkedList* newnode= new LinkedList;
    LinkedList * currnode=head;
    newnode->dist=dist;
    newnode->vec=vect;
    newnode->next=NULL;

    if(head==NULL){
        head=newnode;
        return;}
    while(currnode->next!=NULL){
        currnode=currnode->next;
    }
    currnode->next=newnode;
    return;
}

*/

class dist_vec
    {public:
    long double dist;
    vec* vect;
    dist_vec(long double dist, vec* vect)
       : dist(dist), vect(vect)
        {};
    ~dist_vec(void);
    };
dist_vec::~dist_vec()
	{

	};
struct pqcompare
    {
    bool operator()(dist_vec const& NN1, dist_vec const& NN2)
        {
        return NN1.dist > NN2.dist;
        }
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
        vec* vect=NULL;
        node* next=NULL;
        //~node(void);
    };
    /*
node::~node()
    {
    //if(vect!=NULL)
      //  {
        //delete vect;
        //vect=NULL;
        //}
    if(next!=NULL)
        {
        delete next;
        next=NULL;
        }
    }
    */

class hashtable
    {
    protected:
        node** buckets=NULL;//array me ta buckets
        int bucket_count;//ari8mos buckets
        int total_nodes;
        int initialized;
    public:
        hashtable(void); // plithos bucket
        void hashtable_init(int);
        int hashtable_insert(vec* ,long );//eisagwgh 
        void hashtable_print();
        node* search_nd(long);
        ~hashtable(void);
    };

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
node* hashtable::search_nd(long hvalue)
    {
    int id=hvalue%bucket_count;
    if(buckets!=NULL)
        {
        if(buckets[id]!=NULL)
            return buckets[id];
        }
    return NULL;
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

class Lhashtables
    {
    protected:
        hashtable* Lhtables=NULL;//array me ta hashtables
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
        vector<dist_vec>* NN_search(vec*,int);
        vector<vector<dist_vec>*>* find_k_nearest(vec* ,int ,int );
        vector<dist_vec>* LRadius_search(vec* ,int );
        vector<vector<dist_vec>*>* find_in_LRadius(vec* ,int ,int );
        ~Lhashtables(void);

    };

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
            cout<<endl;
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

int Lhashtables:: lsh_continue(int no_of_ht,int no_of_vectors, vec* nvectors){
    int h_return;
    int h[this->k];
    int tablesize=no_of_vectors/16;//apo diafaneies
    this->Lhtables[no_of_ht].hashtable_init(tablesize);
    long int g_notablesize;
    for(int i=0;i<no_of_vectors;i++){
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

int input_handler(int argc, char *argv[],int* k, int* L, int* N,double* R,char (&input_file)[256], char (&query_file)[256], char (&output_file)[256]){
    char temp[256];
    if(argsOK(argc,argv)){
        printf("All parameters given from command line...\n");
        strcpy((input_file),argv[2]);
        strcpy((query_file),argv[4]);
        strcpy((output_file),argv[10]);
        *(k)=atoi(argv[6]);
        *(L)=atoi(argv[8]);
        *(N)=atoi(argv[12]);
        *(R)=atof(argv[14]);
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

vector<dist_vec>* brute_calculate(vec* qvector,vec* nvectors,int no_of_vectors,int no_of_coordinates,int N,int pos){
    long double dist;
    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;
    for(int i=0;i<no_of_vectors;i++){
        dist=vect_dist(qvector[pos].coord,nvectors[i].coord,no_of_coordinates);
        Q.push(dist_vec(dist,&(nvectors[i])));
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

   
    
   // cout<<"Time taken to brute_calculate_all:"<<<<" seconds"<<endl;
    return dsvec2;

}

vector<dist_vec>* Lhashtables::NN_search(vec* nvector,int N)
    {
        int counter=0;
    int element_count=0;
    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;
    //int g_notablesize=0;
    long int g_notablesize[this->L];
    int h_return;
    int h[k];

    for (int li = 0; li < L; li++)
        {
        for(int ki=0;ki<this->k;ki++)
            {

            h_return=h_function(nvector->coord,this->v[li][ki],this->t[li][ki]);
            h[ki]=h_return;
            //cout<<"H Function Return:"<<h[ki]<<endl;
            }
        //cout<<"calling g function"<<endl;
        g_notablesize[li]=g_function(h,this->r[li],this->k);

        node* currnode=Lhtables[li].search_nd(g_notablesize[li]);

        while(currnode!=NULL)
            {
            if(currnode->vect!=NULL)
            	{	
            	//if(currnode->hashvalue==NULL){break;}
            	if(currnode->hashvalue == g_notablesize[li])
                	{
                    counter++;
                	long double dist=vect_dist(nvector->coord,currnode->vect->coord,d);
                	Q.push(dist_vec(dist,currnode->vect));
                	}
                }
              else
              	cout<<"NULL vect found"<<endl;
            currnode=currnode->next;    
            }

        }
    if(counter<N){
        for(int li=0;li<L;li++){
            node* currnode=Lhtables[li].search_nd(g_notablesize[li]);

            while(currnode!=NULL)
                {
                if(currnode->vect==NULL){break;}
                //if(currnode->hashvalue==NULL){break;}
                if(currnode->hashvalue != g_notablesize[li])
                    {
                        counter++;
                    long double dist=vect_dist(nvector->coord,currnode->vect->coord,d);
                    Q.push(dist_vec(dist,currnode->vect));
                    }
                currnode=currnode->next;    
                }

        }
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


vector<vector<dist_vec>*>* Lhashtables:: find_k_nearest(vec* qvectors,int N,int queries_no_of_vectors){
    //vector<dist_vec> dsvec;
    vector<vector<dist_vec>*>* dsvec2;
    dsvec2=new vector<vector<dist_vec>*>;

    for(int i=0;i<queries_no_of_vectors;i++){
        dsvec2->push_back(NN_search(&(qvectors[i]),N));

        /*
        for(int j=0;j<dsvec2[i].size();j++ )
            if(dsvec2[i][j].vect!=NULL)
            cout<<"For query:"<<i<<"vec id "<<dsvec2[i][j].vect->name<<"dist "<<dsvec2[i][j].dist<<endl;
        */

    }
    
    return dsvec2;

}

vector<dist_vec>* Lhashtables::LRadius_search(vec* nvector,int R)
    {
        int counter=0;
    int element_count=0;
    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;
    //int g_notablesize=0;
    long int g_notablesize[this->L];
    int h_return;
    int h[k];

    for (int li = 0; li < L; li++)
        {
        for(int ki=0;ki<this->k;ki++)
            {

            h_return=h_function(nvector->coord,this->v[li][ki],this->t[li][ki]);
            h[ki]=h_return;
            //cout<<"H Function Return:"<<h[ki]<<endl;
            }
        //cout<<"calling g function"<<endl;
        g_notablesize[li]=g_function(h,this->r[li],this->k);

        node* currnode=Lhtables[li].search_nd(g_notablesize[li]);
        int nn=0;
        while(currnode!=NULL)
            {
            nn++;
            if(currnode->vect!=NULL)
          		{
            	counter++;
            	long double dist=vect_dist(nvector->coord,currnode->vect->coord,d);
            	if(dist<R)
                	Q.push(dist_vec(dist,currnode->vect));
            	}
            else
              	cout<<"NULL vect found"<<endl;
            currnode=currnode->next;    
            }
        //cout<<"BUcket of "<<li<<" hashtable "<<"num of nodes "<<nn<<endl;;
        }
    
    vector<dist_vec>* dsvec=new vector<dist_vec>;//=new vector<dist_vec>;
    
    
        while(!Q.empty())
            {
            dist_vec tempdv(Q.top().dist,Q.top().vect);
            dsvec->push_back(tempdv);
            Q.pop();
            }
        
        
    return dsvec;      
    }




vector<vector<dist_vec>*>* Lhashtables:: find_in_LRadius(vec* qvectors,int R,int queries_no_of_vectors){
    //vector<dist_vec> dsvec;
    vector<vector<dist_vec>*>* dsvec2;
    dsvec2= new vector<vector<dist_vec>*>;
    for(int i=0;i<queries_no_of_vectors;i++){
        dsvec2->push_back(LRadius_search(&(qvectors[i]),R));
        /*
        for(int j=0;j<dsvec2[i].size();j++ )
            if(dsvec2[i][j].vect!=NULL)
            cout<<"For query:"<<i<<"vec id "<<dsvec2[i][j].vect->name<<"dist "<<dsvec2[i][j].dist<<endl;
        */


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

int repeat_handler(vec* nvectors, vec* qvectors,char* input_file,char*query_file,char* output_file,Lhashtables *lht){
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
            delete lht;
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
            delete lht;
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

int main(int argc, char *argv[]){
    char input_file[256],query_file[256],output_file[256],temp[256];
    int k,L,N;
    double R;
    int no_of_vectors,no_of_coordinates;
    int queries_no_of_vectors,queries_no_of_coordinates;
    int ret;
    vec* nvectors;
    vec* qvectors;
    Lhashtables *lht;
    string lsh_or_hypercube="distanceLSH: ";
    if(input_handler(argc,argv,&k,&L,&N,&R,(input_file), query_file, output_file))
        return -1;

    printf("input_file: %s, query_file: %s, output_file: %s,k:%d,L:%d,N:%d,R:%f\n",input_file,query_file,output_file,k,L,N,R);
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


        auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        if(flag==0 || flag==1){
            lht=new Lhashtables(L,no_of_coordinates,k);
            lht->lsh_start(no_of_vectors,nvectors);
        }
        vector<vector<dist_vec>*>* dsvec2;
        dsvec2=lht->find_k_nearest(qvectors,N,queries_no_of_vectors);
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        double time1=((double)duration1.count()/1000000);



        auto start2 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
        vector<vector<dist_vec>*>* dsvec3;
        dsvec3=brute_calculate_all(qvectors,nvectors,no_of_vectors,no_of_coordinates,queries_no_of_vectors,N);
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        double time2=((double)duration2.count()/1000000);
        

        vector<vector<dist_vec>*>* dsvec4;
        dsvec4=lht->find_in_LRadius(qvectors,R,queries_no_of_vectors);

        print_to_file(output_file,lsh_or_hypercube,dsvec2,queries_no_of_vectors,qvectors,time1,time2,dsvec3,dsvec4,R);

        cout<<"den krasaraaaa"<<endl;
  
    flag=repeat_handler(nvectors,qvectors,input_file,query_file,output_file,lht);
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
    delete lht;
    delete [] nvectors;
    delete [] qvectors;
        
}