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
    
    int lat = l->GetLatency();
    int src = l->GetSrc();
    int dest = l->GetDst();
       
    this->routing_table.topo[src][dest].cost = lat;
    this->routing_table.topo[src][dest].age = this->routing_table.top[src][dest].age + 1;
    SendToNeighbors(new RoutingMessage(lat, this->routing_table.top[src][dest].age, src, dest));
}

void LinkState::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;
    
    /
    int lat = m->latency;
    int age = m->age;
    
    //Check whether it's new (no entry exists) OR whether it's newer
    if (routing_table.topo[src][dest].age == -1 || (routing_table.topo[src][dest].age > -1 && routing_table.topo[src][dest].age < age))
    {
    SendToNeighbors(m);
    }
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
    
    //Initialiation:
          
    //First, need a pair of queues to hold unvisited, visited nodes
    set <int> unvisited;
    set <int> visited;
    
    //Also need a map to hold the distances once found, a map to keep track of the paths, and a temp to get hold the vectors from topo 
    //The latter map will be searched recursively by previous node until reaching this node to reconstruct the path
    //Construct iterators for the maps as well
    
    map<int,int> distances; 
    map<int,int>::const_iterator dist_it;
    map<int,int> prev_paths;
    map<int,int>::const_iterator paths_it;
    map<int, TopoLink> temp;
    map<int, TopoLink>::const_iterator temp_it;
    
    //Set node's distance to itself to 0
    distances[0] = 0;
    
    //Set other distances to +INF. Set the previous paths to -(INT_MAX). 
    for (int i=1; i<this.routing_table.topo.size(); i++)
    {
        distances[i] = INT_MAX;
        prev_paths[i] = -(INT_MAX);
    }
    
    //Start with this node 
    temp = this.routing_table.topo[this.number];
    visited.insert(this.number);
    
    //Iterate through this node's neighbors
    for (temp_it = temp.begin(); temp_it != temp.end(); temp++)
    {
        //Iterator will return the key_value pair. Check whether we're looking 
        if (visited.count(temp_it->first) < 1)
        {
        int v = temp_it->first; //Get the key (i.e. node number)
            //D(v) = min( D(w) + c(w,v) , D(v) )
            if ( (distances[this.number] + this.distances[this.number][v].cost) < distances[v])
            {
                distances[v] = (distances[this.number] + this.distances[this.number][v].cost);
                previous[v] = this.number;
                //Add this neighbor to our queue
                unvisited.insert(v);
            }
                    
        }
    
    }
    
    //Initialization should be done; we have a queue of things to test. Start with the nearest.
    
    //Loop:
    
    
   
    

    return NULL;
}

int findPreviousNode (

Table* LinkState::GetRoutingTable() {
    return (new Table(routing_table));
}

ostream & LinkState::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
