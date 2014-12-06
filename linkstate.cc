#include "linkstate.h"
#include "context.h"
#include <map>
#include <set>


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
    int dest = l->GetDest();
       
    this->routing_table.topo[src][dest].cost = lat;
    this->routing_table.topo[src][dest].age = this->routing_table.topo[src][dest].age + 1;
    SendToNeighbors(new RoutingMessage(lat, this->routing_table.topo[src][dest].age, src, dest));
}

void LinkState::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;
    
    int lat = m->latency;
    int age = m->age;
    int src = m->source;
    int dest = m->destination;
    
    //Check whether it's new (no entry exists) OR whether it's newer
    if (routing_table.topo[src][dest].age == -1 || (routing_table.topo[src][dest].age > -1 && routing_table.topo[src][dest].age < age))
    {
    routing_table.topo[src][dest].age = age;
    routing_table.topo[src][dest].cost = lat;
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
          
    //First, need a pair of queues to hold nodes to potentially visit, and visited nodes
    set<int> nodes;
    set<int> visited;
    
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
    //Set this node's previous to itself
    prev_paths[0] = 0;
    
    //Set other distances to +INF. Set the previous paths to -(99999). 
    for (int i=1; i<routing_table.topo.size(); i++)
    {
        distances[i] = 99999;
        prev_paths[i] = -(99999);
    }
    
    //Start with this node 
    temp = routing_table.topo[number];
    visited.insert(number);
    
    //Iterate through this node's neighbors
    for (temp_it = temp.begin(); temp_it != temp.end(); temp_it++)
    {
        //Iterator will return the key_value pair. Check whether the key is visited
        if (visited.count(temp_it->first) < 1)
        {
        int v = temp_it->first; //Get the key (i.e. node number)
            //D(v) = min( D(w) + c(w,v) , D(v) )
            if ( (distances[number] + routing_table.topo[number][v].cost) < distances[v])
            {
                distances[v] = (distances[number] + routing_table.topo[number][v].cost);
                prev_paths[v] = number;
                //Add this neighbor to our queue
                nodes.insert(v);
            }
                    
        }
    
    }
    
    //Initialization should be done; we have a queue of things to test. Start with the nearest.
    
    //Loop:
        while (! nodes.empty() )
        {
            int x = getCurrentSmallest(nodes, visited, distances);
            nodes.erase(x);
            visited.insert(x);
            temp = routing_table.topo[x];
            //Repeat the loop that we ran in the initialization step basically
               for (temp_it = temp.begin(); temp_it != temp.end(); temp_it++)
                {
                    //Iterator will return the key_value pair. Check the key is visited
                    if (visited.count(temp_it->first) < 1)
                    {
                    int v = temp_it->first; //Get the key (i.e. node number)
                        //D(v) = min( D(w) + c(w,v) , D(v) )
                        if ( (distances[number] + routing_table.topo[number][v].cost) < distances[v])
                        {
                            distances[v] = (distances[number] + routing_table.topo[number][v].cost);
                            prev_paths[v] = number;
                            //Add this neighbor to our queue
                            nodes.insert(v);
                        }
                                
                    }
    
                }
                   
        }
    
        //Djikstra's is done. Distances should hold the distances from this node. Now need to recursively go through previous to figure out proper routing.
        //Make a new copy of local_map
        map<int, int> new_map;
        new_map = make_map(prev_paths, number);
        routing_table.local_map = new_map;
        int y= destination->GetNumber();
        
        Node* target = new Node(new_map[y], NULL, 0, 0);
        Node* result = context->FindMatchingNode(const_cast<Node *>(target));
        return result;        
}


map<int, int> LinkState::make_map(map<int,int> prevpaths, int node_number)
{
    map<int, int> result;
    map<int, int>::const_iterator prevpaths_it;
    int destination_node;
    int current_previous;
    int old_previous;
    for (prevpaths_it = prevpaths.begin(); prevpaths_it != prevpaths.end(); prevpaths_it++)
    {
       //The key to the previous vector is the node which has the value as its previous 
       destination_node = prevpaths_it->first; 
       //The value is the last previous node calculated during Djikstra's
       old_previous = prevpaths_it->second;
     
       if (old_previous == node_number)
       result[destination_node] = destination_node; //Directly routing there is fastest
       
       else
       {
            current_previous = prevpaths[old_previous];
            while (current_previous != node_number)
            {
            old_previous = current_previous;
            current_previous = prevpaths[current_previous];            
            }
            result[destination_node] = old_previous;
       }
    
    }

    return result;
}

int LinkState::getCurrentSmallest(set<int> visited, set<int> nodes, map<int,int> distances)
{
    int min_dist = 99999;
    int min_node = -1;
    for (int i=0; i< routing_table.topo.size(); i++)
    {        
        set<int>::const_iterator nodes_it;    
        set<int>::const_iterator visit_it;
        nodes_it = nodes.find(i);      
        //If the current i is in the node queue
        if (nodes_it != nodes.end())
        {
            //If the current i is unvisited
            visit_it = visited.find(i);
            if (visit_it == nodes.end())
            {
                if (distances[i] < min_dist)
                {
                min_dist = distances[i];
                min_node = i;
                }
            
            }
        
        }
    
    }
    if (min_dist == 99999) 
        return -1; //An error has occurred; this shouldn't happen
    else
        return min_node;
}


Table* LinkState::GetRoutingTable() {
    Table *copytable = new Table(routing_table);
   return copytable;
}

ostream & LinkState::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
