#include <iostream>
#include <cstring>
#include <vector>
#include <cctype>
#include <fstream>
#include <sstream>
using namespace std;

class vec
{
public:
    string name;
    vector <int> coord;
};




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

/*
//diavazei to plithos dianismatwn kai to plithos twn syntetagmenwn tous
int get_N_of_input(int* no_of_vectors,int* no_of_coordinates,char input_file[256]){
    //printf("input_file: %s",input_file);

    (*no_of_vectors)=0;//
    (*no_of_coordinates)=0;//ksekiname apo -1 giati metrame kai to item_id san sydetagmenh
    FILE *fd;//anoigoume to arxeio, an den yparxei epistrefei -1 alliws  synexizei
    fd=fopen(input_file,"r");
    if(fd==NULL)
        return -1;
    char getch;
    char prevchar;
    getch=fgetc(fd);//diavazoume xarakthra xarakthra to arxeio
    while(1){
        if(getch==EOF)
            break;
        if(getch=='\n'){//an exoume allagh grammhs tote pame sto epomeno dianisma
            if(isspace(prevchar)&& (*no_of_vectors)==0)
                (*no_of_coordinates)--;
            (*no_of_vectors)++;
        }
        else if ((getch=='\t'&& (*no_of_vectors)==0 )|| (getch==' '&& (*no_of_vectors)==0) ){//afou ola ta dianismata exoun idio plithos syntetagmenwn, metrame mono tou prwtou
            
            (*no_of_coordinates)++;
            
        }
        prevchar=getch;
        getch=fgetc(fd);
    }
    fclose(fd);

    return 0;

}


void create_vectors(vec* nvectors,int no_of_vectors,char input_file[256],int no_of_coordinates){
    //for(int i=0;i<no_of_vectors;i++){
      //  nvectors[i].coord.push_back(i);
    //}
    string concat="";
                    //cout<<"aaaaa"<<concat;

    char getch;
    int i,j,k,l;
    int count=0;
    int id_flag=1;
        FILE *fd;
        fd=fopen(input_file,"r");
        if(fd==NULL){
            cout<<"Problem while opening file!"<<endl;
        }

        getch=fgetc(fd);
        char prevchar;
        while(1){
        if(getch==EOF)
            break;
        if(getch=='\n'){//an exoume allagh grammhs tote pame sto epomeno dianisma
            //cout<<"prevchar:"<<prevchar<<endl;
            //if((concat.empty()))
                //nvectors[count].coord.push_back(stoi(concat));
            concat="";
            (count)++;
            id_flag=1;
        }
        else if (getch=='\t' || getch==' '){
            //if(prevchar!=' '){    
                if(id_flag==1){
                    id_flag=0;
                    nvectors[count].name=concat;
                    concat="";

                }
                else{
                    //cout<<"aaaaa"<<concat;
                    nvectors[count].coord.push_back(stoi(concat));
                    concat="";
                }
           // }       
        }else{
            concat+=getch;
            
        }
        prevchar=getch;
        getch=fgetc(fd);

        
    }


}
*/

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


    //vec vector[no_of_vectors];
  // create_vectors(vector,no_of_vectors,input_file,no_of_coordinates);
    int j;
    //cout.flush();
    /*
    for(int i=0;i<no_of_vectors;i++){
        cout<<"Dianisma :"<<i<<endl;
        for(j=0;j<no_of_coordinates;j++){
        //for(auto j: vector[i].coord){
            //cout<<j<<' ';
            if(j==0)
                cout<<"Name-id: "<<nvectors[i].name<<endl;
            
            cout<<nvectors[i].coord[j]<<" ";
        }
    }

*/
   delete [] nvectors;
		
}