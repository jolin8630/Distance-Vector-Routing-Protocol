#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;



struct distance_table table2;
struct rtpkt send_to_0;
struct rtpkt send_to_1;
struct rtpkt send_to_3;
float clocktime;
void printdt2(struct distance_table *dtptr);


struct distance_table 
{
  int costs[4][4];
} dt2;


/* students to write the following two routines, and maybe some others */

int find_row_min2(int row) {
  int min = 999;
  for (int j = 0; j < 4; j++) {
    if (table2.costs[row][j] < min)
      min = table2.costs[row][j];
  }
  return min;
}
void send_neighbors2() {
  send_to_0.sourceid = 2;
  send_to_0.destid = 0;
  send_to_0.mincost[0] = find_row_min2(0);
  send_to_0.mincost[1] = find_row_min2(1);
  send_to_0.mincost[2] = 0;
  send_to_0.mincost[3] = find_row_min2(3);

  
  send_to_1.sourceid = 2;
  send_to_1.destid = 1;
  send_to_1.mincost[0] = find_row_min2(0);
  send_to_1.mincost[1] = find_row_min2(1);
  send_to_1.mincost[2] = 0;
  send_to_1.mincost[3] = find_row_min2(3);

  send_to_3.sourceid = 2;
  send_to_3.destid = 3;
  send_to_3.mincost[0] = find_row_min2(0);
  send_to_3.mincost[1] = find_row_min2(1);
  send_to_3.mincost[2] = 0;
  send_to_3.mincost[3] = find_row_min2(3);
  

  printf ("rtinit2 calling the routine tolayer2\n");
  tolayer2(send_to_0);
  tolayer2(send_to_1);
  tolayer2(send_to_3);

}


void rtinit2() 
{
  printf("rtinit3 is called at: %f\n", clocktime);
  for (int i = 0; i < 4; i++){
    for (int j= 0; j < 4; j++){
      table2.costs[i][j] = 999;
    }
  }
  table2.costs[2][0] = 0;
  table2.costs[2][1] = 0;
  table2.costs[2][2] = 0;
  table2.costs[2][3] = 0;

  table2.costs[0][2] = 3;
  table2.costs[0][0] = 3;
  table2.costs[1][2] = 1;
  table2.costs[1][1] = 1;
  table2.costs[3][2] = 2;
  table2.costs[3][3] = 2;


  send_neighbors2();


}


void rtupdate2(struct rtpkt *rcvdpkt)
{printf("rtupdate2 called at: %f\n", clocktime);

  // update rt
  int changed = 0;
  int j = rcvdpkt->sourceid;
  for (int i = 0; i < 4; i++) {
    int new_cost = find_row_min2(j) + rcvdpkt->mincost[i];
    if (table2.costs[i][j] > new_cost) {
      table2.costs[i][j] = new_cost;
      changed = 1;
    }
  }
  // send
  if (changed){
    send_neighbors2();
    printf("The table change\n");
  }
  else{
      printf("The table did not change\n");}
printdt2(&table2); 
changed =0;
 
}



void printdt2(struct distance_table *dtptr)
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}







