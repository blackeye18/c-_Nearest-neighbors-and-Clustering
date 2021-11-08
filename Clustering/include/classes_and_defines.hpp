#ifndef __classes_and_defines__
#define __classes_and_defines__

#define RLIMIT 1000
#define PNUM 4294967291
#define W 4//meta3i 0 kai 6
#define BUCKET_RATIO 32
const string metric = "euclidean_distance";



class vec
{
public:
    string name;//to id ths grammhs-dianismatos
    vector <double> coord;
    int clustered_flag;
};


class dist_vec
    {public:
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
        int lsh_start(int,vec*,vector<int>* );
        int lsh_continue(int,int,vec*,vector<int>* );
        int Cluster_LRadius(vec* cvector,long int g_notablesize[],double radius,int clust_num,vector<vector<dist_vec>> *curr_clust_vec,int iteration);
        vector<vector<vec*>>* ANN_lsh(vec* nvect,vector<int>* clustersvec,int no_of_vectors);
        //vector<dist_vec>* NN_search(vec*,int);
        //vector<vector<dist_vec>*>* find_k_nearest(vec* ,int ,int );
       //vector<dist_vec>* LRadius_search(vec* ,double );
        //vector<vector<dist_vec>*>* find_in_LRadius(vec* ,double ,int );
        ~Lhashtables(void);

    };



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
        void Hashfun_init_cube(void);//initializes tables used for hashing 
        unsigned int hash_calc(vec* vect);//returns the bin cube vector point
        int vec_insert(vec* vect);//inserts a vector to the multimap
        void cube_start(int ,vec*,vector<int>* );
        //vector<vector<dist_vec>*>* all_NN_search(vec* ,int ,int );
        //vector<dist_vec>* NN_search(vec*,int);
        //vector<dist_vec>* RANGE_search(vec* ,double );
        //vector<vector<dist_vec>*>* all_RANGE_search(vec* ,double ,int );
        //void cube_map_print();
    
    };

#endif