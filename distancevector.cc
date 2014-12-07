#include "distancevector.h"
#include "table.h"

DistanceVector::DistanceVector(unsigned n, SimulationContext* c, double b, double l) :
    Node(n, c, b, l)
{}

DistanceVector::DistanceVector(const DistanceVector & rhs) :
    Node(rhs)
{
    *this = rhs;
}

DistanceVector & DistanceVector::operator=(const DistanceVector & rhs) {
    Node::operator=(rhs);
	//table = rhs.table;
    return *this;
}

DistanceVector::~DistanceVector() {}

/** Write the following functions.  They currently have dummy implementations **/

void DistanceVector::LinkHasBeenUpdated(Link *l)
{
	cerr << *this<<": Link Update: "<<*l<<endl;
	
	int updateSuccesful;
	
	// update the new link information in the table(neighbors map)
	routing_table.UpdateSingleLink(l->GetSrc(), l->GetDest(), l->GetLatency());
	
	updateSuccesful = UpdateNodeDV();
	
	// if the node's distance vector was successfully updated, send out messages
	if (updateSuccesful)
	{
		RoutingMessage *m = new RoutingMessage(GetNumber(), routing_table.routing_table);
		SendToNeighbors(m);
	}
}

void DistanceVector::ProcessIncomingRoutingMessage(RoutingMessage *m) {
	cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;
	
	int updateSuccesful;
	
	// update the overall topo to include the updated distance vector of the message
	routing_table.topo[m->source_node] = m->updated_dv_table;
	
	my_map::const_iterator init_it;
	for (init_it = m->updated_dv_table.begin(); init_it != m->updated_dv_table.end(); init_it++)
	{
		// initializing distance vectors that haven't been set yet
		if (routing_table.routing_table[init_it->first].cost == -1){}
	}
	
	updateSuccesful = UpdateNodeDV();
	
	// if the node's distance vector was successfully updated, send out messages
	if (updateSuccesful)
	{
		RoutingMessage *m = new RoutingMessage(GetNumber(), routing_table.routing_table);
		SendToNeighbors(m);
	}
}

Node* DistanceVector::GetNextHop(Node *destination)
{
	Node *ret_node = NULL;
	deque<Node*>::const_iterator neighbors_it;

	// get the node that is the next hop and return
	deque<Node*>* neighbors = GetNeighbors();
	for (neighbors_it = neighbors->begin(); neighbors_it != neighbors->end(); ++neighbors_it)
	{
		int curr_neighbor = (*neighbors_it)->GetNumber();
		if (curr_neighbor == routing_table.hops_map[destination->GetNumber()])
		{
			ret_node = new Node(*(*neighbors_it)); 
		}
	}
	
	return ret_node;
}

Table* DistanceVector::GetRoutingTable()
{
	return new Table(routing_table);
}

void DistanceVector::TimeOut() {
    cerr << *this << " got a timeout: (ignored)" << endl;
}

bool DistanceVector::UpdateNodeDV()
{
	my_map::const_iterator routing_it;
	my_map::const_iterator neighbor_it;
	
	bool success = false;
	
	// loop through distance vector table
	for (routing_it = routing_table.routing_table.begin(); routing_it != routing_table.routing_table.end(); routing_it++)
	{
		int curr_node = routing_it->first;
		int min = std::numeric_limits<int>::max();
		int next_hop = -1;
		
		// if the current node is this node, latency = 0
		if (curr_node == (int)GetNumber())
		{
			routing_table.routing_table[curr_node].cost = 0;
			continue;
		}
		
		// loop through neighbor table
		for (neighbor_it = routing_table.neighbor_table.begin(); neighbor_it != routing_table.neighbor_table.end(); neighbor_it++)
		{
			int curr_neighbor = neighbor_it->first;

			if (routing_table.neighbor_table[curr_neighbor].cost != -1 && routing_table.topo[curr_neighbor][curr_node].cost != -1)
			{
				// temp latency for next hop = neighbor + their cost
				int temp = routing_table.neighbor_table[curr_neighbor].cost + routing_table.topo[curr_neighbor][curr_node].cost;
				
				// test temp against default min (infinity) 
				if (temp < min)
				{
				   min = temp;
				   next_hop = curr_neighbor;
				}
			}
		}
		
		// if their is a new min to set
		if (min != std::numeric_limits<int>::max() && min != routing_table.routing_table[curr_node].cost)
		{
			routing_table.routing_table[curr_node].cost = min;
			routing_table.hops_map[curr_node] = next_hop;
			success = true;
		}
	}
	return success;  
}

ostream & DistanceVector::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
