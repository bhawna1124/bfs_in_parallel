/**********************************************************************
filename: cpu/baseline/bfs.cpp
author: onesuper
email: onesuperclark@gmail.com

two queues version

***********************************************************************/

#include <omp.h>
#include <stdio.h>
#include <tbb/concurrent_queue.h>
#include <sys/time.h>
#include "syncbitops.h"
#include "bitops.h"
#include <sched.h>

#define DEBUG

/*
void set_bit(unsigned int num, unsigned long* bitmap) {
     bitmap[num/32] |= ( 0x80000000 >> num%32);
}

int test_bit(unsigned int num, unsigned long* bitmap) {
     return bitmap[num/32] & (0x80000000 >> num%32);
}
*/

unsigned int determine_socket(unsigned int v) {
     return v % 4;
}

typedef struct pair_t {
     unsigned int first;
     unsigned int second;
} PAIR;


float bfs(int num_of_threads) 
{
	 struct timeval start, end;
	 float time_used;
	 tbb::concurrent_bounded_queue<unsigned int> current_a[4];
     tbb::concurrent_bounded_queue<unsigned int> current_b[4];
     tbb::concurrent_bounded_queue<PAIR> socket_queue[4];



     int map_size = num_of_nodes/ (32*4) + 1;

     unsigned long* bitmap[4];

     bitmap[0] = (unsigned long*) malloc(sizeof(unsigned long)*map_size);
     bitmap[1] = (unsigned long*) malloc(sizeof(unsigned long)*map_size);
     bitmap[2] = (unsigned long*) malloc(sizeof(unsigned long)*map_size);
     bitmap[3] = (unsigned long*) malloc(sizeof(unsigned long)*map_size);

     for (int i=0; i<4; i++)
          for (int j=0; j<map_size; j++)
               bitmap[i][j] = 0;

	 gettimeofday(&start, 0);

	 // visiting the source node now
     set_bit(source_node_no, bitmap[determine_socket(source_node_no)]);
	 current_a[determine_socket(source_node_no)].push(source_node_no);
	 cost[source_node_no] = 0;

	 // set threads number
	 omp_set_num_threads(num_of_threads);
	 int k = 0;
     bool stop = false;


     do {
          if (k%2 ==0) {
#pragma omp parallel
               {
                    int socket_no = sched_getcpu();
#ifdef DEBUG
                    printf("thread %d in socket:%d\n", omp_get_thread_num(), socket_no);
#endif 
                    while (!current_a[socket_no].empty()) {
                         unsigned int index = -1; 
                         current_a[socket_no].try_pop(index);
                         if (index != -1) {
                              Node cur_node = node_list[index];
                              for (int i = cur_node.start; i < (cur_node.start+cur_node.edge_num); i++)
                              {
                                   unsigned int id = edge_list[i].dest;
                                   int it_belongs_to = determine_socket(id);
                                   if (socket_no == it_belongs_to) {
                                        if (!test_bit(id/4, bitmap[socket_no])) {
                                             int its_color = sync_test_and_set_bit(id/4, bitmap[socket_no]);
                                             if (!its_color) {
                                                  cost[id] = cost[index] + 1;
                                                  current_b[socket_no].push(id);
                                             }
                                        }
                                   } else {
                                        PAIR a_pair;
                                        a_pair.first = id;
                                        a_pair.second = index;
                                        socket_queue[it_belongs_to].push(a_pair);
                                   }
                              }
                         }
                    }
               }
               

               
               //
               // phase 1 stops ! barrier here!!!
               //
               

#pragma omp parallel
               {
                  
                    int socket_no = sched_getcpu();
#ifdef DEBUG
                    printf("thread %d in cleaner:%d\n", omp_get_thread_num(), socket_no);
#endif
                    while (!socket_queue[socket_no].empty()) {
                         PAIR a_pair;
                         a_pair.first = -1;
                         socket_queue[socket_no].try_pop(a_pair);
                         unsigned int index = a_pair.second;
                         unsigned int id = a_pair.first;
                         if (id != -1) {
                              if(!test_bit(id/4, bitmap[socket_no])) {
                                   int its_color = sync_test_and_set_bit(id/4, bitmap[socket_no]);
                                   if (!its_color) {
                                        cost[id] = cost[index] + 1;
                                        current_b[socket_no].push(id);
                                   }
                              }
                         }
                    }
               }
               
               //
               // phase 2 ends ! barrier here!!! 
               //
               if (current_b[0].empty() && current_b[1].empty() && current_b[2].empty() && current_b[3].empty()) 
                    stop = true; 


#ifdef DEBUG
               for(int i=0; i<4; i++) {
                    printf("%d %d %d\n", current_a[i].size(), current_b[i].size(), socket_queue[i].size());
               }
               printf("--------------------------------------\n");
#endif

          } else {

             
#pragma omp parallel
               {
                    int socket_no = sched_getcpu();
#ifdef DEBUG
                    printf("thread %d in socket:%d\n", omp_get_thread_num(), socket_no);
#endif
                    while (!current_b[socket_no].empty()) {
                         unsigned int index = -1;;
                         current_b[socket_no].try_pop(index);
                         if (index != -1) {
                              Node cur_node = node_list[index];
                              for (int i = cur_node.start; i < (cur_node.start+cur_node.edge_num); i++)
                              {
                                   unsigned int id = edge_list[i].dest;
                                   int it_belongs_to = determine_socket(id);
                                   if (socket_no == it_belongs_to) {
                                        if (!test_bit(id/4, bitmap[socket_no])) {
                                             int its_color = sync_test_and_set_bit(id/4, bitmap[socket_no]);
                                             if (!its_color) {
                                                  cost[id] = cost[index] + 1;
                                                  current_a[socket_no].push(id);
                                             }
                                        }
                                   } else {
                                        PAIR a_pair;
                                        a_pair.first = id;
                                        a_pair.second = index;
                                        socket_queue[it_belongs_to].push(a_pair);
                                   }
                              }
                         }
                    }
               }
               

               
               //
               // phase 1 stops ! barrier here!!!
               //
               
         
#pragma omp parallel
               {
                  
                    int socket_no = sched_getcpu();
#ifdef DEBUG
                    printf("thread %d in cleaner:%d\n", omp_get_thread_num(), socket_no);
#endif
                    while (!socket_queue[socket_no].empty()) {
                         PAIR a_pair;
                         a_pair.first = -1;
                         socket_queue[socket_no].try_pop(a_pair);
                         if (a_pair.first != -1) {
                              unsigned int index = a_pair.second;
                              unsigned int id = a_pair.first;
                              if(!test_bit(id/4, bitmap[socket_no])) {
                                   int its_color = sync_test_and_set_bit(id/4, bitmap[socket_no]);
                                   if (!its_color) {
                                        cost[id] = cost[index] + 1;
                                        current_a[socket_no].push(id);
                                   }
                              }
                         }
                    }
               }
               
               //
               // phase 2 ends ! barrier here!!! 
               //
               if (current_a[0].empty() && current_a[1].empty() && current_a[2].empty() && current_a[3].empty()) 
                    stop = true;    

#ifdef DEBUG
               for(int i=0; i<4; i++) {
                    printf("%d %d %d\n", current_a[i].size(), current_b[i].size(), socket_queue[i].size());
               }
               printf("--------------------------------------\n");
#endif      
          }
          k++;
     } while(!stop);

	 gettimeofday(&end, 0);
	 time_used = 1000000 * (end.tv_sec - start.tv_sec) +
		  end.tv_usec - start.tv_usec;
	 time_used /= 1000000;
	 printf("used time: %f\n", time_used);

     free(bitmap[0]);
     free(bitmap[1]);
     free(bitmap[2]);
     free(bitmap[3]);
     
	 return time_used;
	 
}
