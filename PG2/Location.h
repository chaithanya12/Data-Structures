using namespace std;

class Location {
	public:
		Location(double,double,string);
		double latd;
		double longd;
		string loc_name;
		double dist;
	private:
};

Location::Location(double my_latd, double my_longd, string my_loc_name){
	latd = my_latd;
	longd = my_longd;
	loc_name = my_loc_name;
}
