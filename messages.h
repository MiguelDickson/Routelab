#ifndef _messages
#define _messages

#include <iostream>
#include "node.h"
#include "link.h"

struct RoutingMessage {
    RoutingMessage();
    RoutingMessage(const RoutingMessage &rhs);
    RoutingMessage &operator=(const RoutingMessage &rhs);

    ostream & Print(ostream &os) const;

    // Anything else you need

    #if defined(LINKSTATE)
    RoutingMessage(int latency, int age, int source, int destination)
    int age;
    int source;
    int destination;
    int latency;
    #endif 
    
    
    
    #endif
    #if defined(DISTANCEVECTOR)
    #endif
};

inline ostream & operator<<(ostream &os, const RoutingMessage & m) { return m.Print(os);}

#endif
