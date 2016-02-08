#include <vector>
#include <list>
#include "Location.h"
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
using namespace std;
bool comp_loc_func(Location x, Location y);


template <typename T>
class Distance{
	public:
		Distance();
		T loc_store;
		void add_loc(double,double,string);
		void calc_dist_from(Location);
		void print_dists();
		void sort_by_dist();
		template <typename T_in> Distance operator=(Distance<T_in>);
		void bentley();
		void bentley(typename T::iterator, typename T::iterator);
		void loc_swap(typename T::iterator, typename T::iterator);
		void med_threes(int, typename T::iterator &, typename T::iterator &);
		void ins_sort(T&);
	private:
};

template <typename T>
Distance<T>::Distance(){
	srand(time(NULL));
}

template <typename T>
void Distance<T>::add_loc(double my_latd, double my_longd, string my_loc_name){
	loc_store.push_back(Location(my_latd,my_longd,my_loc_name) );
}

template <typename T>
void Distance<T>::calc_dist_from(Location my_loc){
	for(typename T::iterator i=loc_store.begin(); i!=loc_store.end(); ++i){
		(*i).dist = (24901./(2*M_PI))*acos( sin((M_PI*my_loc.latd)/180)*sin((M_PI*(*i).latd)/180) + cos((M_PI*my_loc.latd)/180)*cos((M_PI*(*i).latd)/180)*cos((M_PI/180)*(my_loc.longd-(*i).longd)) );
	}
}

template <typename T>
void Distance<T>::print_dists(){
	for(typename T::iterator i=loc_store.begin(); i!=loc_store.end(); ++i){
		cout << (*i).dist;
		cout << " " << (*i).loc_name << endl;
	}
}

// template specialization
// if T=vector<Location> then use std::sort
// if T=list<Location> then use list::sort
template<>
void Distance<vector<Location> >::sort_by_dist(){
	sort(loc_store.begin(), loc_store.end(), comp_loc_func);
}
template<>
void Distance<list<Location> >::sort_by_dist(){
	(*this).loc_store.sort(comp_loc_func);
}


bool comp_loc_func(Location x, Location y){
	// if the distances are equal, then sort by location name
	if (x.dist==y.dist)
		return (x.loc_name < y.loc_name);
	else
		return (x.dist < y.dist);
}

template <typename T>		//class template
template <typename T_in>	//function template
Distance<T> Distance<T>::operator=(Distance<T_in> my_Distance){
	(*this).loc_store.clear();
	for(typename T_in::iterator i=my_Distance.loc_store.begin(); i!=my_Distance.loc_store.end(); ++i){
		(*this).loc_store.push_back(*i);
	}
	return (*this);
}

template <typename T>
void Distance<T>::bentley(){
	bentley(loc_store.begin(),loc_store.end()-1);
}

template <typename T>
void Distance<T>::bentley(typename T::iterator lo, typename T::iterator hi){
if(lo>hi) exit(1);
	// handle trivial cases of size= 1 or 2
	if(hi==lo) return;
	if(hi==lo+1){
		if(lo->dist > hi->dist)
			loc_swap(lo,hi);
		else if(lo->dist == hi->dist && lo->loc_name > hi->loc_name)
			loc_swap(lo,hi);
		return;
	}
	// median of threes
	if(hi-lo>17){
		med_threes(3,lo,hi);
	}

	// Note that lo is the pivot
	typename T::iterator 	p = lo+1,
				q = hi,
				i = lo+1,
				j = hi;
	// order vector into [ pivot|<pivot|>pivot|pivot ]
	while(i!=q){
		while(i->dist < lo->dist){
			++i;
			if(i>hi) break;
		}
		while(j->dist > lo->dist){
			--j;
		}
		if(i>j) break;

		loc_swap(i,j);
		if(i->dist == lo->dist){
			loc_swap(i++,p++);
		}
		if(i>j) break;
		if(j->dist == lo->dist){
			loc_swap(j--,q--);
		}
		if(i>j) break;
	}
	// address distances of equal order and use quick insertion sort to order them alphabetically
	if((hi-q)+(p-lo)>1){
		T v_equal;
		typename T::iterator t = p;
		while(t!=lo) v_equal.push_back(*--t);
		t = q;
		while(t!=hi) v_equal.push_back(*++t);
		// v_equal now holds a vector of all equal distances
		ins_sort(v_equal);
		// v_equal is now sorted, so push back into the front and back ends
		typename T::iterator scan_equal = v_equal.begin();
		t = lo;
		while(t!=p) *(t++)=*(scan_equal++);
		t = q;
		while(t!=hi) *(++t)=*(scan_equal++);
	}

	// order vector into [ <pivot|pivot|>pivot ]
	while(p>lo){
		if(p==hi || (j==hi && i==hi)) 	loc_swap(--j,--p);
		else 				loc_swap(j--,--p);
	}
	while(q<hi){
		loc_swap(i++,++q);
	}

	// recurse
	if(j>lo)	bentley(lo,j);
	if(i<hi)	bentley(i,hi);

	return;
}

template <typename T>
void Distance<T>::loc_swap(typename T::iterator p, typename T::iterator q){
	Location temp = *p;
	*p = *q;
	*q = temp;
}

template <typename T>
void Distance<T>::med_threes(int i, typename T::iterator &p_lo, typename T::iterator &p_hi){
	typename T::iterator p = p_lo +( rand()%((p_hi-(i-1))-p_lo) );
	bentley(p,p+(i-1));//this will sort <p,p+1,p+2> so that p+1 is the median
	loc_swap(p_lo,p+(i-1)/2);
}

template <typename T>
void Distance<T>::ins_sort(T& my_v){
	int i=1;
	for(typename T::iterator t=my_v.begin()+i; t!=my_v.end(); t=my_v.begin()+i){
		while(t!=my_v.begin() && t->loc_name<(t-1)->loc_name){
			Location temp = *t;
			*t=*(t-1);
			*(t-1)=temp;
			t--;
		}
		++i;
	}
}
