#include <iostream>
#include <cstring>
using namespace std;

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
//diavazei to plithos dianismatwn kai to plithos twn syntetagmenwn tous
int get_N_of_input(int* no_of_vectors,int* no_of_coordinates,char input_file[256]){
    //printf("input_file: %s",input_file);

    (*no_of_vectors)=0;//
    (*no_of_coordinates)=-1;//ksekiname apo -1 giati metrame kai to item_id san sydetagmenh
    FILE *fd;//anoigoume to arxeio, an den yparxei epistrefei -1 alliws  synexizei
    fd=fopen(input_file,"r");
    if(fd==NULL)
        return -1;
    char getch;

    getch=fgetc(fd);//diavazoume xarakthra xarakthra to arxeio, afou aplws mas endiaferei to plithos kai oxi ta dedomena
    while(1){
        if(getch==EOF)
            break;
        if(getch=='\n')//an exoume allagh grammhs tote pame sto epomeno dianisma
            (*no_of_vectors)++;
        else if (getch=='\t'&& (*no_of_vectors)==0)//afou ola ta dianismata exoun idio plithos syntetagmenwn, metrame mono tou prwtou
            (*no_of_coordinates)++;
        getch=fgetc(fd);
    }
    

    return 0;

}

int main(int argc, char *argv[]){
    char input_file[256],query_file[256],output_file[256];
    int k,L,N,R;//allagi tou R se dekadiko arithmo
    int no_of_vectors,no_of_coordinates;
    int ret;
	if(argsOK(argc,argv)){
        printf("All parameters given from command line...\n");
        strcpy(input_file,argv[2]);
        strcpy(query_file,argv[4]);
        strcpy(output_file,argv[10]);
        k=atoi(argv[6]);
        L=atoi(argv[8]);
        N=atoi(argv[12]);
        R=atoi(argv[14]);
    }else{
        printf("Parameters were NOT given from command line, please input them to contiue!\n");
        //default times
        k=4;
        L=5;
        N=1;
        R=10000;
        printf("Type the path to the input_file: ");
        scanf("%s",input_file);
        //printf("\n");
        printf("Type the path to the query_file: ");
        scanf("%s",query_file);
        //printf("\n");
        printf("Type the path to the output_file: ");
        scanf("%s",output_file);
        //printf("\n");
    }
    printf("input_file: %s, query_file: %s, output_file: %s,k:%d,L:%d,N:%d,R:%d\n",input_file,query_file,output_file,k,L,N,R);
    ret=get_N_of_input(&no_of_vectors,&no_of_coordinates,input_file);
    if(ret==-1){
        printf("Something terrible happened while opening the file... Exiting...");
        return -1;
    }
    printf("no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);

		
}