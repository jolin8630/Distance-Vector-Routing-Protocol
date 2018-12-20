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
} dt1;


/* students to write the following two routines, and maybe some others */
void printdt1(struct distance_table *dtptr);
void linkhandler1(int linkid, int newcost);

struct distance_table table1;
struct rtpkt send_to_0;
struct rtpkt send_to_2;
struct rtpkt send_to_3;
struct rtpkt to_neighbor1;
int routing1[4];
float clocktime;

int find_row_min1(int row) {
  int min = 999;
  for (int j = 0; j < 4; j++) {
    if (table1.costs[row][j] < min)
      min = table1.costs[row][j];
  }
  return min;
}

void send_neighbors1() {
  send_to_0.sourceid = 1;
  send_to_0.destid = 0;
  send_to_0.mincost[0] = find_row_min1(0);
  send_to_0.mincost[1] = 0;
  send_to_0.mincost[2] = find_row_min1(2);
  send_to_0.mincost[3] = find_row_min1(3);

  
  send_to_2.sourceid = 1;
  send_to_2.destid = 2;
  send_to_2.mincost[0] = find_row_min1(0);
  send_to_2.mincost[1] = 0;
  send_to_2.mincost[2] = find_row_min1(2);
  send_to_2.mincost[3] = find_row_min1(3);

  printf ("rtinit1 calling the routine tolayer2\n");
  tolayer2(send_to_0);
  tolayer2(send_to_2);
}

void To_neighbors1(){
  send_to_0.sourceid = 1;
  send_to_0.destid = 0;

  send_to_2.sourceid = 1;
  send_to_2.destid = 2;

  tolayer2(send_to_0);
  tolayer2(send_to_2);

}

void rtinit1() 
{

  printf("rtinit1 is called at: %f\n", clocktime);
  for (int i = 0; i < 4; i++){
    for (int j= 0; j < 4; j++){
      table1.costs[i][j] = 999;
    }
  }
  table1.costs[1][0] = 0;
  table1.costs[1][1] = 0;
  table1.costs[1][2] = 0;
  table1.costs[1][3] = 0;

  table1.costs[0][1] = 1;
  table1.costs[0][0] = 1;
  table1.costs[2][1] = 1;
  table1.costs[2][2] = 1;

  send_neighbors1();
  
}
void rtupdate1(struct rtpkt *rcvdpkt)
{
  printf("rtupdate1 called at: %f\n", clocktime);
printdt1(&table1); 
  // update rt
  int changed = 0;
  int j = rcvdpkt->sourceid;
  for (int i = 0; i < 4; i++) {
    int new_cost = find_row_min1(j) + rcvdpkt->mincost[i];
    if (table1.costs[i][j] > new_cost) {
      table1.costs[i][j] = new_cost;
      changed = 1;
    }
  }
  // send
  if (changed){
    send_neighbors1();
    printf("The table change\n");

}
  else{
      printf("The table did not change\n");}
printdt1(&table1); 

}


void printdt1(struct distance_table *dtptr)
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}



void linkhandler1(int linkid, int newcost)    
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
   printf("rtlinkhandler1 called at: %f\n", clocktime);
  printf("rtlinkhandler1 linkid %d , newcosts %d\n", linkid, newcost);

  table1.costs[linkid][0] = newcost;
  
  int dif = newcost - table1.costs[linkid][linkid];
  for(int i =0 ; i<4 ; i++){
    table1.costs[linkid][i] += dif;
  }
  for(int i = 0; i<4; i++){
      if(i != 1 && routing1[i] == linkid){
        int minimum = linkid;
        for(int j=0; j<4; j++){
          if(j!=1 && table1.costs[j][i] < table1.costs[minimum][i]){
            minimum = j;
          }
        }
        routing1[i] = minimum;
      }
    }
    
    for(int k = 0; k< 4; k++){
        if(k == linkid){
            to_neighbor1.mincost[linkid] = newcost;
        }
        else{
            to_neighbor1.mincost[k] = find_row_min1(k);
        }
      }
    //send_neighbors1();
    To_neighbors1();


}


