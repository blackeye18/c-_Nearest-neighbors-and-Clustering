#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;



class vec
{
public:
    string name;//to id ths grammhs-dianismatos
    vector <double> coord;
};

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
	        

	return 0;
}