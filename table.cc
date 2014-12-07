#include "table.h"

Table::Table() {
    topo.clear();
}

Table::Table(const Table & rhs) {
    *this = rhs;
}

Table & Table::operator=(const Table & rhs) {
    /* For now,  Change if you add more data members to the class */
    topo = rhs.topo;

    return *this;
}

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  os << "Generic Table()";
  return os;
}
#endif

#if defined(LINKSTATE)
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
ostream & Table::Print(ostream &os) const
{
  os << "DistanceVector Table()";
  return os;
}
#endif
