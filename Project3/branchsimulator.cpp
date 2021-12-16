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
			state = 3;
		}
		bool isTaken(){
			return state > 1;
		}
		void update(bool isT){
			state = ((state << 1) + isT) % 4;
		}
		int getState() {
			return state;
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

	unsigned int size = pow(2, m);
	string line;
	while (getline(trace, line)) {
		unsigned int inst = strtol(line.substr(0, 8).c_str(), NULL, 16);
		bool isTaken = strtol(line.substr(9, 10).c_str(), NULL, 2) > 0;
		out << (saturatList[inst % size].isTaken() ? "1" : "0") << endl;
		saturatList[inst % size].update(isTaken);
	}

	trace.close();	
	out.close();
}
