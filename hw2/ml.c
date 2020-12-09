#include <stdlib.h>
#include <stdio.h>

double * w;
double * y;
double ** real;
double ** transpose;
double ** first;
double ** second;
double ** inversemx;
void create (int ex,int att){
    att++;
    y=(double *)malloc(ex * sizeof(double));

    real=(double **)malloc(ex * sizeof(double *));
    for (int i = 0; i < ex; i++)
    {
        real[i]=(double *)calloc(att, sizeof(double));
    }

    transpose=(double **)malloc(att * sizeof(double *));
    for (int i = 0; i < att; i++)
    {
        transpose[i]=(double *)calloc(ex, sizeof(double));
    }
    
    first=(double **)malloc(att * sizeof(double *));
    for (int i = 0; i < att; i++)
    {
        first[i]=(double *)calloc(att, sizeof(double));
    }

    second=(double **)malloc(att * sizeof(double *));
    for (int i = 0; i < att; i++)
    {
        second[i]=(double *)calloc(ex, sizeof(double));
    }

    w=(double *)malloc(att * sizeof(double ));

    inversemx=(double **)malloc(att * sizeof(double *));
    for (int i = 0; i < att; i++)
    {
        inversemx[i]=(double *)calloc(att, sizeof(double));
    }
    for (int i=0;i<att;i++){
        for(int j=0;j<att;j++){
            if (i==j){
                inversemx[i][j]=(double)1;
            }
            else{
                inversemx[i][j]=(double)0;
            }
        }
    }

}

void multiply(int row,int column){
    for (int i=0;i<column;i++){
        for (int j=0;j<column;j++){
            first[i][j]=0.0;
            for (int k=0;k<row;k++){
                first[i][j]+=(transpose[i][k]*(real[k][j]));
            }
        }
    }
}
void inverse(int num){
    for (int p=0;p<num;p++){
        double temp=first[p][p];
        for (int q=0;q<num;q++){
            first[p][q]=first[p][q]/temp;
            inversemx[p][q]=(inversemx[p][q]/temp);
        }
        for (int i=p+1;i<num;i++){
            double temp2=first[i][p];
            for (int j=0;j<num;j++){
                first[i][j]=first[i][j]-(temp2*first[p][j]);
                inversemx[i][j]=inversemx[i][j]-(temp2*inversemx[p][j]);
            }
        }
    }
    
    for (int p=num-1;p>=0;p--){
        for (int i=p-1;i>=0;i--){
            double temp2=first[i][p];
            for (int j=0;j<num;j++){
                first[i][j]=first[i][j]-(temp2*first[p][j]);
                inversemx[i][j]=inversemx[i][j]-(temp2*inversemx[p][j]);
            }
        }
    }
}
void nextm(int row, int column){  //5,3
    for (int i=0;i<row;i++){
        for (int j=0;j<column;j++){
            second[i][j]=0;
            for (int k=0;k<row;k++){
                second[i][j]+=(inversemx[i][k]*(transpose[k][j]));
            }
        }
    }
}

void nextm2(int row,int column){  //3
    for (int i=0;i<row;i++){
        w[i]=0;
        for (int k=0;k<column;k++){
            w[i]+=(second[i][k]*(y[k]));
        }
    }
}

void frees(int ex,int att){ 
    for (int i=0;i<att;i++){
        free(transpose[i]);
        free(first[i]);
        free(second[i]);
        free(inversemx[i]);
    }
    free(transpose);
    free(first);
    free(second);
    free(inversemx);
    for (int i=0; i<ex; i++){ 
        free(real[i]);
    }
    free(real);
    free(y);
    free(w);
}
int main(int argc,char **argv){
    FILE * filepointer=fopen(argv[1],"r");
    int column;
    int row;
    fscanf(filepointer,"%d",&column);
    fscanf(filepointer,"%d",&row);
    if (column ==0 || row==0){
        return 0;
    }
    int ycount=0;
    create(row,column);
    int idk=column+1;
    for (int i=0;i<row;i++){
        for (int j=0;j<idk;j++){
            if(j==0){
                real[i][j]=1;
                transpose[j][i]=1;
            }
            else{
                double temp;
                fscanf(filepointer,"%lf,",&temp);
                real[i][j]=temp;
                transpose[j][i]=temp;
            }
        }
        fscanf(filepointer,"%lf\n",&y[ycount]);
        ycount++;
    }
    fclose(filepointer);
    multiply(row,(idk));   
    inverse((idk));
    nextm((idk),row);
    nextm2(idk,row);
    FILE *test=fopen(argv[2],"r");
    int amount;
    fscanf(test,"%d",&amount);
    double sum=0;
    for (int j=0;j<amount;j++){
        for (int i=0;i<idk;i++){
            if (i==0){
                sum+=w[i];
            }
            else{
                double temp;
                fscanf(test,"%lf,",&temp);
                sum+=temp*w[i];
            }
        }
        printf("%0.0lf\n",sum);
        sum=0;
    }
    fclose(test);
    frees(row,idk);
    return 0;
}