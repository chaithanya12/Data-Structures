/*	distance2.cpp
 *	Author: Patrick Drumm
 *	Class: cse30311
 *
 *	Included files:
 *		Distance.h - Distance is a class that is templated to allow the storage of locations in either a vector or a list (Distance.loc_store is the vector/list).
 *			> add_loc(double,double,string) is a function within Distance that creates a new Location and pushes it onto the back of loc_store
 *	
 *	distance2.cpp
 *		> reads in all of the locations from a location database textfile and stores them into a Distance.
 *		> calculates and prints the distance from the user-inputted latitude and longitude to each location stored in the vector/list
 */


#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "Distance.h"
#include <cmath>
#include <stdlib.h>
using namespace std;

template <typename T>
void read_in_locs(char*,Distance<T>*);

int main(int argc, char**argv){
	// create Distance object and store all locations from user-inputted location database
	Distance<vector<Location> > my_Distance;
	read_in_locs(argv[1],&my_Distance);
	// enter latitude,longitude arguments as a Location object
	Location user_loc = Location(atof(argv[2]),atof(argv[3]),"User location");
	
	// calculate the distance from each point in vector/list to the user location
	my_Distance.calc_dist_from(user_loc);
	// print the distances from each location
	my_Distance.print_dists();
}

template <typename T>
void read_in_locs(char* file_name, Distance<T>* my_dist){
	double t_latd, t_longd;
	string t_loc_name;

	ifstream inFile;
	inFile.open(file_name, ios::in);
	// note that the inFile.ignore() ensures that the space between the longitude and the location name is not stored as the first char of the location name
	while(inFile>>t_latd && inFile>>t_longd && inFile.ignore() && getline(inFile,t_loc_name) ){
//cout<<t_latd<<endl;
		(*my_dist).add_loc(t_latd,t_longd,t_loc_name);
	}

}
