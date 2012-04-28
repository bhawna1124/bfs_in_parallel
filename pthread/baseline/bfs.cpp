/**********************************************************************
filename: cpu/baseline/bfs.cpp
author: onesuper
email: onesuperclark@gmail.com

two queues version

***********************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <tbb/concurrent_queue.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sched.h>
//#define DEBUG



tbb::concurrent_queue<unsigned int> current_a;
tbb::concurrent_queue<unsigned int> current_b;

//tbb::concurrent_bounded_queue<unsigned int> current_a;
//tbb::concurrent_bounded_queue<unsigned int> current_b;
pthread_barrier_t barr;
pthread_barrier_t barr2;

int list[16] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};


void* thread_func(void*) {
     pthread_t thread_id = pthread_self();
     //printf("%d\n", sched_getcpu());
     int k = 0;
     bool stop = false;
     while(1) {
          if (k%2 ==0) {
               while (current_a.unsafe_size()) {
                    unsigned int index;
                    if (current_a.try_pop(index)) {
                         Node cur_node = node_list[index];
                         for (int i = cur_node.start; i < (cur_node.start+cur_node.edge_num); i++) {
                              unsigned int id = edge_list[i].dest;
                              bool its_color;
                              
                              its_color = __sync_lock_test_and_set(&visited[id], true);
                              if (its_color == false) {
                                   cost[id] = cost[index] + 1;
                                   current_b.push(id);
                              }
                              
                         }
                    }
               }

               pthread_barrier_wait(&barr);
               if (current_b.empty()) break;
               pthread_barrier_wait(&barr2);
          } else {
               while (current_b.unsafe_size()) {
                    unsigned int index;
                    if (current_b.try_pop(index)) {
                         Node cur_node = node_list[index];
                         for (int i = cur_node.start; i < (cur_node.start+cur_node.edge_num); i++) {
                              unsigned int id = edge_list[i].dest;
                              bool its_color;
                              its_color = __sync_lock_test_and_set(&visited[id], true);
                              if (its_color == false) {
                                   cost[id] = cost[index] + 1;
                                   current_a.push(id);
                              }
                         }
                    }
               }
               pthread_barrier_wait(&barr);
               if (current_a.empty()) break;
               pthread_barrier_wait(&barr2);
          }
          k++;
     }
}




float bfs(int num_of_threads) 
{
	 struct timeval start, end;
	 float time_used;

	 gettimeofday(&start, 0);
	 
	 visited[source_node_no] = true;
	 current_a.push(source_node_no);
	 cost[source_node_no] = 0;

     if (pthread_barrier_init(&barr, NULL, num_of_threads))
     {
          printf("could not create a barrier\n");
          return -1;
     }

     if (pthread_barrier_init(&barr2, NULL, num_of_threads))
     {
          printf("could not create a barrier\n");
          return -1;
     }
	 
	 pthread_t tid[50];
     pthread_attr_t attr[50];
     cpu_set_t cpu_info;

     for(int i=0; i<num_of_threads; i++) {
          pthread_attr_init(&attr[i]);
          CPU_ZERO(&cpu_info);
          CPU_SET(list[i], &cpu_info);
          if (0 != pthread_attr_setaffinity_np(&attr[i], sizeof(cpu_set_t), &cpu_info)) {
               printf("set affinity fail\n");
          }
          
     }

	 for (int i=0; i<num_of_threads; i++) {
          int err = pthread_create(&tid[i], &attr[i], thread_func, NULL);
          if (err != 0) {
               printf("can't create thread!");
               break;
          }
     }

     

     for (int i=0; i<num_of_threads; i++) {
          int err = pthread_join(tid[i], NULL);
          if (err != 0) {
               printf("can't join\n");
          }
     }
     


	 gettimeofday(&end, 0);
	 time_used = 1000000 * (end.tv_sec - start.tv_sec) +
		  end.tv_usec - start.tv_usec;
	 time_used /= 1000000;
	 printf("used time: %f\n", time_used);

	 return time_used;
	 
}

