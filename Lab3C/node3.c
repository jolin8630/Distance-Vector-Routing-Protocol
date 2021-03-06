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


struct distance_table table3;
struct rtpkt send_to_0;
struct rtpkt send_to_2;
float clocktime;

void printdt3(struct distance_table *dtptr);

struct distance_table 
{
  int costs[4][4];
} dt3;

/* students to write the following two routines, and maybe some others */

int find_row_min3(int row) {
  int min = 999;
  for (int j = 0; j < 4; j++) {
    if (table3.costs[row][j] < min)
      min = table3.costs[row][j];
  }
  return min;
}

void send_neighbors3() {
  send_to_0.sourceid = 3;
  send_to_0.destid = 0;
  send_to_0.mincost[0] = find_row_min3(0);
  send_to_0.mincost[1] = find_row_min3(1);
  send_to_0.mincost[2] = find_row_min3(2);
  send_to_0.mincost[3] = 0;

  //send to 3
  send_to_2.sourceid = 3;
  send_to_2.destid = 2;
  send_to_2.mincost[0] = find_row_min3(0);
  send_to_2.mincost[1] = find_row_min3(1);
  send_to_2.mincost[2] = find_row_min3(2);
  send_to_2.mincost[3] = 0;

  printf ("rtinit3 calling the routine tolayer2\n");
  tolayer2(send_to_0);
  tolayer2(send_to_2);
}

void rtinit3() 
{

  printf("rtinit3 is called at: %f\n", clocktime);
  for (int i = 0; i < 4; i++){
    for (int j= 0; j < 4; j++){
      table3.costs[i][j] = 999;
    }
  }
  table3.costs[3][0] = 0;
  table3.costs[3][1] = 0;
  table3.costs[3][2] = 0;
  table3.costs[3][3] = 0;

  table3.costs[0][3] = 7;
  table3.costs[0][0] = 7;
  table3.costs[2][3] = 2;
  table3.costs[2][2] = 2;


  send_neighbors3();
}


void rtupdate3(struct rtpkt *rcvdpkt)
{
  printf("rtupdate3 called at: %f\n", clocktime);

  // update rt
  int changed = 0;
  int j = rcvdpkt->sourceid;
  for (int i = 0; i < 4; i++) {
    int new_cost = find_row_min3(j) + rcvdpkt->mincost[i];
    if (table3.costs[i][j] > new_cost) {
      table3.costs[i][j] = new_cost;
      changed = 1;
    }
  }
  // send
  if (changed){
    send_neighbors3();
    printf("The table change\n");}
  else{
      printf("The table did not change\n");}
printdt3(&table3); 
changed =0;
 
}



  


void printdt3(struct distance_table *dtptr)
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}







