#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <bitset>
#include <math.h>
#define StrongTaken 3
#define WeakTaken 2
#define WeakNtaken 1
#define StrongNtaken 0

using namespace std;

class entry{
	private:
		int state;
	public:
		entry(){
			this.state = 11;
			
		}
		bool isTaken(){
			if (this.state == 11 or this.state == 10){
				return 1;
			}
			else{
				return 0;
			}

		}
		void update(bool isT){
			bit s
			state << 1 + 1 
			int a [-2:]
			
			
		}
		int getState() {
			return this.state; 
			
			
			
		}
};

int main (int argc, char** argv) {
	ifstream config;
	config.open("config.txt");

	int m, k;
	config >> m >> k;
 
	config.close();

	ofstream out;
	string out_file_name = string("trace.txt") + ".out";
	out.open(out_file_name.c_str());
	
	ifstream trace;
	trace.open("trace.txt");

	vector<entry> saturatList(pow(2, m));
	
	
	while (getline(trace,line))
                {      
                    int pc = bitset<8>(line.substr<0,8>, 16  );
                	findpc = pc.su<-m:>
                	satiralist[finpc] = state;
                	//²»ÊÇÈÃÄãĞ´×´Ì¬ ÊÇĞ´ 0 1  
                	out<< state << 
                	bool t = line[-1]
                	satiralist[finpc].update(t)
                    i++;
                    
                    

	trace.close();	
	out.close();
}
