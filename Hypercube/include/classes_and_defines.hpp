#ifndef __classes_and_defines__
#define __classes_and_defines__

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

    #endif