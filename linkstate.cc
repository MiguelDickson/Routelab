#include "linkstate.h"

LinkState::LinkState(unsigned n, SimulationContext* c, double b, double l) :
    Node(n, c, b, l)
{}

LinkState::LinkState(const LinkState & rhs) :
    Node(rhs)
{
    *this = rhs;
}

LinkState & LinkState::operator=(const LinkState & rhs) {
    Node::operator=(rhs);
    return *this;
}

LinkState::~LinkState() {}


/** Write the following functions.  They currently have dummy implementations **/
void LinkState::LinkHasBeenUpdated(Link* l) {
    cerr << *this << ": Link Update: " << *l << endl;
    
    int src = l->GetSrc();
    int dest = l->GetDst();
    int lat = l->GetLatency();
    
    this->routing_table.topo[src][dest].cost = lat;
       
    SendToNeighbors(new RoutingMessage());
}

void LinkState::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;
}

void LinkState::TimeOut() {
    cerr << *this << " got a timeout: (ignored)" << endl;
}

Node* LinkState::GetNextHop(Node *destination) { 

    //Run Djikstra's to get the next hop: 
    
    //1 Initialization
    //List of nodes N' = {this node, U} 
        //for every node v
        //if v is neighbor of {U}
        //D(v) = c(u,v) 
        //else
        //D(v) = +INF
        
    //2 Loop
        //Find node w not in N' such that D(this node, W) is min.
        //add w to N'
        //update D(v) for each neighbor v of w and not in N':
            //D(v) = min( D(v), D(w) + c(w,v) )
    //until N'=N
    
    int size = this.routing_table.
    
    //First, need a list of nodes
    set <int> list_of_nodes;
    
    //Ne
   
    

    return NULL;
}

Table* LinkState::GetRoutingTable() {
    return (new Table(routing_table));
}

ostream & LinkState::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
