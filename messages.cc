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

RoutingMessage::RoutingMessage(int latency, int age, int source, int destination) {
    this.latency = latency;
    this.age = age;
    this.source = source;
    this.destination = destination;

}


#endif

#if defined(DISTANCEVECTOR)
ostream &RoutingMessage::Print(ostream &os) const
{
    os << "DistanceVector RoutingMessage()";
    return os;
}
#endif
