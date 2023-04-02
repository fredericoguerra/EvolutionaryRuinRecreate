#include <vector>
#include <sys/timeb.h>
#include <sys/resource.h>
#include "Data.h"


bool compareReleaseDates(Job a, Job b){
   return a.r_j < b.r_j;
}

void printSolution(vector<unsigned int> s){
   int s_size = s.size();

   for(int i = 0; i < s_size; i++){
      cout << s[i] << " ";
   }
   cout << "\n";

}

double compCost(vector<unsigned int> s, Data *data){
   int s_size = s.size();
   double c_time = 0;
   double inventory = data->initialInventory;
   bool penalize = false;

   for(int i = 0; i < s_size; i++){
      if(c_time >= data->jobs[s[i]-1].r_j)
         c_time += data->jobs[s[i]-1].p_j;
      else
         c_time = data->jobs[s[i]-1].r_j + data->jobs[s[i]-1].p_j;

      inventory += data->jobs[s[i]-1].inv_mod;
      //cout << inventory << " ";
      if(inventory < 0 || inventory > data->maxCapacity)
         penalize = true;
   }
   //cout << endl;
   c_time = (penalize) ? c_time * 10000 : c_time;

   return c_time;
}

/*double cpuTime() {
	static struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	return ((double)usage.ru_utime.tv_sec)+(((double)usage.ru_utime.tv_usec)/((double)1000000));
}*/