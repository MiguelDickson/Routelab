#include "messages.h"

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs) {
    *this = rhs;
}

RoutingMessage & RoutingMessage::operator=(const RoutingMessage & rhs) {
    /* For now.  Change if you add data members to the struct */
    return *this;
}

#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
    os << "Generic RoutingMessage()";
    return os;
}
#endif

#if defined(LINKSTATE)
ostream &RoutingMessage::Print(ostream &os) const
{
    os << "LinkState RoutingMessage()";
    return os;
}

RoutingMessage::RoutingMessage(int lat, int ag, int src, int dst) {
    latency = lat;
    age = ag;
    source = src;
    destination = dst;

}


#endif

#if defined(DISTANCEVECTOR)

RoutingMessage::RoutingMessage(int node, my_map dv)
{
    source_node = node;
    updated_dv_table = dv;
}

ostream &RoutingMessage::Print(ostream &os) const
{
    os << "DistanceVector RoutingMessage()";
    return os;
}
#endif
