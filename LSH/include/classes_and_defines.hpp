#ifndef __classes_and_defines__
#define __classes_and_defines__

#define RLIMIT 1000
#define PNUM 4294967291
#define W 4//meta3i 0 kai 6
const string metric = "euclidean_distance";



class vec
{
public:
    string name;//to id ths grammhs-dianismatos
    vector <double> coord;
};


class dist_vec
    {public:
    long double dist;
    vec* vect;
    dist_vec(long double dist, vec* vect)
       : dist(dist), vect(vect)
        {};
    ~dist_vec(void);
    };




struct pqcompare
    {
    bool operator()(dist_vec const& NN1, dist_vec const& NN2)
        {
        return NN1.dist > NN2.dist;
        }
    };

class node
    {
    public:
        long long hashvalue;
        int id;
        vec* vect=NULL;
        node* next=NULL;
    };


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





#endif