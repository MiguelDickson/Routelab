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
    RoutingMessage(int lat, int ag, int src, int dst);
    int age;
    int source;
    int destination;
    int latency;
    #endif 
    
    #if defined(DISTANCEVECTOR)
	RoutingMessage(int, my_map);
	my_map updated_dv_table;
	int source_node;
    #endif
};

inline ostream & operator<<(ostream &os, const RoutingMessage & m) { return m.Print(os);}

#endif
