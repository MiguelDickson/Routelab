#ifndef _table
#define _table

#include <iostream>
#include <map>

using namespace std;

struct TopoLink {
    TopoLink(): cost(-1), age(0) {}

    TopoLink(const TopoLink & rhs) {
        *this = rhs;
    }

    TopoLink & operator=(const TopoLink & rhs) {
        this->cost = rhs.cost;
        this->age = rhs.age;

        return *this;
    }

    int cost;
    int age;
};

typedef std::map<int, TopoLink> my_map;
typedef std::map <int, my_map> my_topo;

// Students should write this class
class Table {
 
    public:
        Table();
        Table(const Table &);
        Table & operator=(const Table &);

        map < int, map < int, TopoLink > > topo;
            
        ostream & Print(ostream &os) const;
        
        // Anything else you need

        #if defined(LINKSTATE)
        
        map<int, int> local_map;
        
        #endif

        #if defined(DISTANCEVECTOR)
		
		void UpdateSingleLink(int, int, int);
		
		my_map routing_table;
		my_map neighbor_table;
		map <int, int> hops_map;
		
        #endif
};

inline ostream & operator<<(ostream &os, const Table & t) { return t.Print(os);}


#endif
