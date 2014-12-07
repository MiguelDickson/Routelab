#include "table.h"

Table::Table(const Table & rhs) {
    *this = rhs;
}

Table & Table::operator=(const Table & rhs) {
    /* For now,  Change if you add more data members to the class */
    topo = rhs.topo;

    return *this;
}

#if defined(GENERIC)

Table::Table() {
    topo.clear();
}

ostream & Table::Print(ostream &os) const
{
  os << "Generic Table()";
  return os;
}
#endif

#if defined(LINKSTATE)

Table::Table() {
    topo.clear();
}

ostream & Table::Print(ostream &os) const
{
  map<int, map<int, TopoLink> >::const_iterator row = topo.begin();

    while (row != topo.end()) {
        map<int, TopoLink>::const_iterator col = (*row).second.begin();
        os << "Row=" << (*row).first;

        while (col != (*row).second.end()) {
            os << " Col=" << (*col).first << " cost=" << (*col).second.cost << " age=" << (*col).second.age << endl;
            col++;
        }
        row++;
    }

   
  return os;
}
#endif

#if defined(DISTANCEVECTOR)

Table::Table()
{
    topo.clear();
    routing_table.clear();
    neighbor_table.clear();
    hops_map.clear();
}

// an outgoing link latency has changed for the node
void Table::UpdateSingleLink(int src, int dest, int latency)
{
	cout << "UPPDATE LINK: \nsrc= "<<src<<"\ndest= "<<dest<<"\nlatency= "<<latency<<"\n\n"<<endl;
	
	// update the distance to the neighbor and default dv
	neighbor_table[dest].cost = latency;
	routing_table[dest].cost = -1;
	
	// set overall topo of node to node = 0
	topo[dest][dest].cost = 0;
}

ostream & Table::Print(ostream &os) const
{
	os << "DistanceVector Table \n";
	
	/*my_topo::const_iterator row = neighbor_table.begin();
    while (row != neighbor_table.end()) {
        map<int, TopoLink>::const_iterator col = (*row).second.begin();
        os << "Row=" << (*row).first;

        while (col != (*row).second.end()) {
            os << " Col=" << (*col).first << " latency=" << (*col).second.cost << endl;
            col++;
        }
        row++;
    }*/
	
	return os;
}
#endif
