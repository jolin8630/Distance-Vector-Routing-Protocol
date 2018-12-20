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

struct distance_table 
{
  int costs[4][4];
} dt0;


/* students to write the following two routines, and maybe some others */

void printdt0(struct distance_table *dtptr);
void linkhandler0(int linkid, int newcost);
struct rtpkt to_neighbors;

struct distance_table table0;
struct rtpkt send_to_1;
struct rtpkt send_to_2;
struct rtpkt send_to_3;
float clocktime;
int routing[4];


int find_row_min0(int row) {
  int min = 999;
  for (int j = 0; j < 4; j++) {
    if (table0.costs[row][j] < min)
      min = table0.costs[row][j];
  }
  return min;
}

void send_neighbors0() {
  send_to_1.sourceid = 0;
  send_to_1.destid = 1;
  send_to_1.mincost[0] = 0;
  send_to_1.mincost[1] = find_row_min0(1);
  send_to_1.mincost[2] = find_row_min0(2);
  send_to_1.mincost[3] = find_row_min0(3);

  
  send_to_2.sourceid = 0;
  send_to_2.destid = 2;
  send_to_2.mincost[0] = 0;
  send_to_2.mincost[1] = find_row_min0(1);
  send_to_2.mincost[2] = find_row_min0(2);
  send_to_2.mincost[3] = find_row_min0(3);

  send_to_3.sourceid = 0;
  send_to_3.destid = 3;
  send_to_3.mincost[0] = 0;
  send_to_3.mincost[1] = find_row_min0(1);
  send_to_3.mincost[2] = find_row_min0(2);
  send_to_3.mincost[3] = find_row_min0(3);

  printf ("rtinit0 calling the routine tolayer2\n");
  tolayer2(send_to_1);
  tolayer2(send_to_2);
  tolayer2(send_to_3);

}

void To_neighbors0() {
  send_to_1.sourceid = 0;
  send_to_1.destid = 1;
  

  
  send_to_2.sourceid = 0;
  send_to_2.destid = 2;
  
  send_to_3.sourceid = 0;
  send_to_3.destid = 3;
  

  tolayer2(send_to_1);
  tolayer2(send_to_2);
  tolayer2(send_to_3);


}

void rtinit0() 
{  
  
  for (int i = 0; i <=3 ; i++){
    routing[i] = i;
  }
  //timestamp
  printf("rtinit0 is called at: %f\n", clocktime);
  for (int i = 0; i < 4; i++){
    for (int j= 0; j < 4; j++){
      table0.costs[i][j] = 999;
    }
  }
  table0.costs[0][0] = 0;
  table0.costs[0][1] = 0;
  table0.costs[0][2] = 0;
  table0.costs[0][3] = 0;

  table0.costs[1][0] = 1;
  table0.costs[1][1] = 1;
  table0.costs[2][0] = 3;
  table0.costs[2][2] = 3;
  table0.costs[3][0] = 7;
  table0.costs[3][3] = 7;


  send_neighbors0();
}
void rtupdate0(struct rtpkt *rcvdpkt)
{
   printf("rtupdate0 called at: %f\n", clocktime);

  // update rt
  int changed = 0;
  int j = rcvdpkt->sourceid;
  for (int i = 0; i < 4; i++) {
    int new_cost = table0.costs[routing[j]][j] + rcvdpkt->mincost[i];
    table0.costs[j][i]= new_cost;
    if (table0.costs[routing[i]][i] > new_cost) {
      routing[i] = j;
      changed = 1;
    }
    else if(routing[i] == j && new_cost >table0.costs[routing[i]][i]){
      int minimum = j;
      for(int k=0; k<4; k++){
        if(k!= 0 && table0.costs[k][i]< table0.costs[minimum][i]){
          minimum = k;
        }
      }
      changed =1;
      routing[i] = minimum;
    }
  
  }

  // send
  if (changed){
    send_neighbors0();
    printf("The table change\n");
  }
  else{
    printf("The table did not change\n");
  }

  printdt0(&table0);
  changed = 0;

}




void printdt0(struct distance_table *dtptr)  
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
   dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
   dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
   dtptr->costs[3][2],dtptr->costs[3][3]);
}

void linkhandler0(int linkid, int newcost)   
/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
  
{

  printf("rtlinkhandler0 called at: %f\n", clocktime);
  printf("rtlinkhandler0 linkid %d , newcosts %d\n", linkid, newcost);


  table0.costs[linkid][1] = newcost;

  int dif = newcost - table0.costs[linkid][linkid];
  for(int i = 0; i<4 ; i++){
    table0.costs[linkid][i] += dif;
  }
  for(int i = 1; i<4 ; i++){
    if(routing[i] == linkid){
      int minimum = linkid;
      for(int j=1; j<4; j++){
        if(table0.costs[j][i] < table0.costs[minimum][i]){
          minimum = j;
        }
      }
      routing[i] = minimum;
    }

  }
  

  for(int i = 0; i<=3; i++){
    if(i == linkid){
      to_neighbors.mincost[linkid] = newcost;
    }
    else{
      to_neighbors.mincost[i] = find_row_min0(i);
    }
  }
  
  To_neighbors0();


  }


  
  

