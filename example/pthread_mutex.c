#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

pthread_mutex_t mutex;

const char filename[] = "hello";

void* thread(void *id){

        int num = *(int *)id;
        // 加锁

        if (pthread_mutex_lock(&mutex) != 0){
                fprintf(stdout, "lock error!\n");
        }
        // 写文件的操作
        FILE *fp = fopen(filename, "a+");
        int start = *((int *)id);
        int end = start + 1;
        setbuf(fp, NULL);// 设置缓冲区的大小
        fprintf(stdout, "%d\n", start);
        for (int i = (start * 10); i < (end * 10); i ++){
                fprintf(fp, "%d\t", i);
        }
        fprintf(fp, "\n");
        fclose(fp);

        // 解锁
        pthread_mutex_unlock(&mutex);

        return NULL;
}

int main(){
        int num_thread = 5;
        pthread_t *pt = (pthread_t *)malloc(sizeof(pthread_t) * num_thread);
        int * id = (int *)malloc(sizeof(int) * num_thread);

        // 初始化互斥锁
        if (pthread_mutex_init(&mutex, NULL) != 0){
                // 互斥锁初始化失败
                free(pt);
                free(id);
                return 1;
        }

        for (int i = 0; i < num_thread; i++){
                id[i] = i;
                if (pthread_create(&pt[i], NULL, thread, &id[i]) != 0){
                        printf("thread create failed!\n");
                        return 1;
                }
        }
        for (int i = 0; i < num_thread; i++){
                pthread_join(pt[i], NULL);
        }
        pthread_mutex_destroy(&mutex);

        // 释放资源
        free(pt);
        free(id);
        return 0;
}
