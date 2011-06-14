#include<stdio.h>
#include<stdlib.h>
#include "graph_file.h"
#include "core.h"
#include"path_labeling.h"


void count_size(Graph_Loader& gl,int&l_size,int&f_size,int&g_size){
    Graph* graph;
    l_size=0;
    f_size=0;
    g_size=0;
    int nc=0;
    int* pfid;
    int fid;
    while(gl.load(graph)){
        g_size++;
        nc=graph->node_count;
        pfid=graph->features;
        for(int i=0;i<nc;i++){
            if(l_size<graph->labels[i])
                l_size=graph->labels[i];
            while((fid=*(pfid++))>=0){
                if(f_size<fid)f_size=fid;
            }
        }
        //delete graph;
    }
    l_size++;f_size++;
};

void test(char* modelfile="model.bin",char*testfile="toy.bin",char*resultfile="result.txt"){
    Model* model=new Model(modelfile);
    PERMM permm(model,3);
    Graph_Loader* gl;
    FILE* pFile=fopen(resultfile,"w");
    Graph* graph=NULL;
    
    permm.eval_reset();
    gl=new Graph_Loader(testfile);
    printf("begin\n");
    while(gl->load(graph)){
        
        //permm.decode(graph);
        permm.nb_decode(graph);
        
        //permm.cal_betas(graph);//cal betas, so you can output them
        for(int i=0;i<graph->node_count;i++){
            fprintf(pFile,"%d %d\n",graph->labels[i],permm.result[i]);
        };
        fprintf(pFile,"\n");
    }
    permm.eval_print();
    fclose(pFile);
}
void train(char* trainingfile="toy.bin",char* modelfile="model.bin",int iterations=10){
    Graph_Loader* gl=new Graph_Loader(trainingfile);
    int l_size;
    int f_size;
    int g_size;
    count_size(*gl,l_size,f_size,g_size);
    printf("l_size: %d\n",l_size);
    printf("f_size: %d\n",f_size);
    Model* model=new Model(l_size,f_size);
    
    PERMM permm(model);
    
    Graph* graph=NULL;
    for(int t=0;t<iterations;t++){
        printf("iter %d\n",(t+1));
        permm.eval_reset();
        //gl=new Graph_Loader(trainingfile);
        while(gl->load(graph)){
            
            permm.decode(graph);
            
            /*for(int i=0;i<graph->node_count;i++){
                printf("%d ",permm.result[i]);
            }printf("\n");
            for(int i=0;i<graph->node_count;i++){
                printf("%d ",graph->labels[i]);
            }printf("\n");*/
            //getchar();
            permm.update(graph);
            //delete graph;
        }
        permm.eval_print();


    }
    model->average(permm.steps);
    model->save(modelfile);
}
int main(){
    
    //train();
    //test();
    
    //train("training.bin","model.bin",5);
    test("model.bin","test.bin","result.txt");
    
    /*
    Alpha_Beta list[10];
    for(int i=0;i<10;i++){
        printf("%d\n",list[i].value);
    }printf("\n");
    int ms=4;
    int count=0;
    Alpha_Beta a={1,0,0};
    nb_heap_insert(list,ms,count,a);
    
    nb_heap_insert(list,ms,count,a);
    a.value=3;
    nb_heap_insert(list,ms,count,a);
    
    nb_heap_insert(list,ms,count,a);
    
    nb_heap_insert(list,ms,count,a);
    
    nb_heap_insert(list,ms,count,a);
    
    for(int i=0;i<10;i++){
        printf("%d\n",list[i].value);
    }*/
    return 0;

}

