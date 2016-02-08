/*	distance1.cpp
 *	Author: Patrick Drumm
 *	Class: cse30311
 *
 *	Included files:
 *		Distance.h - Distance is a class that is templated to allow the storage of locations in either a vector or a list (Distance.loc_store is the vector/list).
 *			> add_loc(double,double,string) is a function within Distance that creates a new Location and pushes it onto the back of loc_store
 *	
 *	distance1.cpp simply reads in all of the locations from a location database textfile and stores them into a Distance
 */


#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "Distance.h"
using namespace std;

template <typename T>
void read_in_locs(char*,Distance<T>*);

int main(int argc, char**argv){
	Distance<vector<Location> > my_Distance;
	read_in_locs(argv[1],&my_Distance);
}

template <typename T>
void read_in_locs(char* file_name, Distance<T>* my_dist){
	double t_latd, t_longd;
	string t_loc_name;

	ifstream inFile;
	inFile.open(file_name, ios::in);
	// note that the inFile.ignore() ensures that the space between the longitude and the location name is not stored as the first char of the location name
	while(inFile>>t_latd && inFile>>t_longd && inFile.ignore() && getline(inFile,t_loc_name) ){
		(*my_dist).add_loc(t_latd,t_longd,t_loc_name);
	}

}
