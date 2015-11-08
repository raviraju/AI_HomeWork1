//See if this line is added
#include <iostream>
#include <fstream>
#include <cstring> //strncmp
#include <string>
#include <cstdlib>
#include <set>
#include <sstream>
#include <list>
#include <map>
#include <utility>
#include <queue>
#include <stack>
#include <functional>

using namespace std;

typedef struct {
	int length;
	int no_offPeriods;
	set< pair<int,int> >offPeriods;
	typedef set< pair<int,int> >::iterator offPeriodsItr;
	
	void display()
	{
		cout << "Length of Pipe : " << length << " No of OffPeriods : " << no_offPeriods << endl;
		for(offPeriodsItr it = offPeriods.begin(); it!= offPeriods.end(); ++ it)
			cout << " (" << (*it).first << ", " << (*it).second << " ) ";
		cout<< endl;
	}
}pipeProperty;

typedef pair<string, string> pipeKey;

typedef struct node{
	string state;
	node* parent;
	int path_cost;
	
	node(string s, node* p, int cost)
	{
		state = s;
		parent = p;
		path_cost = cost;
	}
	
	void display()
	{
		cout << "Ravi : NODE state : " << state << " parent : " ;
		if(parent)
			cout << parent->state;
		else
			cout << "NULL";
		cout << " path_cost : " << path_cost << endl;
	}
}node;

struct CompareNode : public std::binary_function<node*, node*, bool>                                                                                       
{  
  bool operator()(const node* lhs, const node* rhs) const  
  {  
     return lhs->path_cost > rhs->path_cost;  
  }  
};

bool goal_test(string node_state, set<string>& destinationNodes)
{
	set<string>::iterator it = destinationNodes.find(node_state);
	if(it!=destinationNodes.end())
		return true;
	else
		return false;
}

bool found_in_frontier(string child, set<string>& visited)
{
	if(visited.find(child) != visited.end())
		return true;
	else
		return false;
}

bool found_in_frontier_states_path(string child, map<string, int>& frontier_states_path)
{
	map<string, int>::iterator it = frontier_states_path.find(child);
	if(it != frontier_states_path.end())
		return true;
	else
		return false;
}


void bfs_srch(string source, set<string>& destinationNodes, map<string, set<string> >& adjacenyMap, int startTime, ofstream& ofs)
{
	int bfs_stepCost = 1;
	cout << "Run BFS Search on source " << source << endl;
	map<string, string> parentMap;
	typedef map<string, string>::iterator parentMapItr;

	parentMap[source]=string("NONE");
	node srcNode(source, NULL, startTime);
	//cout << "Address of " << source << " : " << &srcNode << endl;
	if (goal_test(srcNode.state, destinationNodes))
	{
		cout << "Goal is found at srcNode " << srcNode.state << endl;		
		//write result to output file
		ofs << srcNode.state << " " << srcNode.path_cost << endl;
		return;
	}
	queue<node> frontier;
	set<string> frontier_states;//ease of track of frontier elements
	frontier.push(srcNode);frontier_states.insert(srcNode.state);
	cout<< "Ravi : pushing to frontier : " << srcNode.state << endl;
	//srcNode.display();
	set<string> explored;
	while(1)
	{
		if(frontier.empty())
		{
			//failure
			cout << "None of Destination can be reached" << endl;
			//write "None" to output file
			ofs << "None" << endl;
			return;			
		}
		node parentNode = frontier.front();frontier.pop();
		set<string>::iterator frontier_statesItr = frontier_states.find(parentNode.state);
		if ( frontier_statesItr != frontier_states.end())
			frontier_states.erase(frontier_statesItr);
		cout<< "********************************" << endl;
		cout<< "Ravi : popped from frontier : " << parentNode.state << endl;
		//parentNode.display();

		explored.insert(parentNode.state);
		cout<< "Ravi : Added to explored set : { ";
		for(set<string>::iterator it = explored.begin(); it!= explored.end(); ++ it)
		{
			cout << *it << " , ";
		}
		cout<< " } " << endl;
		
		
		map<string, set<string> >::iterator adjacenyMapItr = adjacenyMap.find(parentNode.state);
		if(adjacenyMapItr != adjacenyMap.end())
		{
			set<string>& children = adjacenyMapItr->second;
			for(set<string>::iterator it = children.begin(); it!= children.end(); ++ it)
			{
				string child = *it;
				cout<< "Ravi : processing " << parentNode.state << "'s child : " << child << endl;
				if ((explored.find(child) == explored.end()) && !(found_in_frontier(child, frontier_states)) ) //not in explored and not in frontier
				{
					cout<< "Ravi : child : " << child << " not in explored and not in frontier "<< endl;
					if(goal_test(child, destinationNodes))
					{
						parentMap[child]=parentNode.state;
						//as the current goal child is not added to queue
						int total_cost = parentNode.path_cost + bfs_stepCost;
						cout << "goal found : " << child << " with cost : " << total_cost << " (with startTime = " << startTime << " )"<< endl;
						ofs << child << " " << total_cost << endl;
						//Display Parents
						//for(parentMapItr parentItr = parentMap.begin(); parentItr != parentMap.end(); ++parentItr)
						//	cout << "Parent[ " << parentItr->first << " ] = " << parentItr->second << endl;
						cout << "path to goal : " << child ;
						string traceParent = parentMap[child];
						while(traceParent != string("NONE"))
						{
							cout << " <- " << traceParent;
							traceParent = parentMap[traceParent];
						}
						cout << endl;
						return;
					}
					parentMap[child]=parentNode.state;
					node childNode(child, &parentNode, parentNode.path_cost + bfs_stepCost);
					//cout << "Address :::: (" << child << " , " << &childNode << ") its parent is : (" << parentNode.state << " , " << &parentNode << ")" << endl;					
					frontier.push(childNode);frontier_states.insert(child);
					cout<< "Ravi : pushing to frontier : " << childNode.state << endl;
				}
			}
		}
		
	}
}

void dfs_srch(string source, set<string>& destinationNodes, map<string, set<string, greater<string> > >& dfsAdjacenyMap, int startTime, ofstream& ofs)
{
	int dfs_stepCost = 1;
	cout << "Run DFS Search on source " << source << endl;
	map<string, string> parentMap;
	typedef map<string, string>::iterator parentMapItr;

	parentMap[source]=string("NONE");
	node srcNode(source, NULL, 0);
	//cout << "Address of " << source << " : " << &srcNode << endl;
	if (goal_test(srcNode.state, destinationNodes))
	{
		cout << "Goal is found at srcNode " << srcNode.state << endl;		
		//write result to output file
		ofs << srcNode.state << " " << startTime << endl;
		return;
	}
	stack<node> frontier;
	set<string> visited;	
	frontier.push(srcNode);visited.insert(srcNode.state);
	cout << "stack push : " << srcNode.state << endl;
	set<string> explored;
	while(1)
	{
		if(frontier.empty())
		{
			//failure
			cout << "None of Destination can be reached" << endl;
			//write "None" to output file
			ofs << "None" << endl;
			return;			
		}
		node parentNode = frontier.top();frontier.pop();
		cout << "stack pop : " << parentNode.state << endl;
		explored.insert(parentNode.state);
		cout<< "Added to explored set : { ";
		for(set<string>::iterator it = explored.begin(); it!= explored.end(); ++ it)
		{
			cout << *it << " , ";
		}
		cout<< " } " << endl;
		map<string, set<string, greater<string> > >::iterator dfsAdjacenyMapItr = dfsAdjacenyMap.find(parentNode.state);
		if(dfsAdjacenyMapItr != dfsAdjacenyMap.end())
		{
			set<string, greater<string> >& children = dfsAdjacenyMapItr->second;
			for(set<string, greater<string> >::iterator it = children.begin(); it!= children.end(); ++ it)
			{
				string child = *it;
				if ((explored.find(child) == explored.end()))// && !(found_in_frontier(child, visited)) ) //not in explored and not in frontier
				{
					if(goal_test(child, destinationNodes))
					{
						parentMap[child]=parentNode.state;
						//as the current goal child is not added to queue
						int total_cost = parentNode.path_cost + dfs_stepCost + startTime;
						cout << "goal found : " << child << " with cost : " << total_cost << " (with startTime = " << startTime << " )"<< endl;
						ofs << child << " " << total_cost << endl;
						//Display Parents
						//for(parentMapItr parentItr = parentMap.begin(); parentItr != parentMap.end(); ++parentItr)
						//	cout << "Parent[ " << parentItr->first << " ] = " << parentItr->second << endl;
						cout << "path to goal : " << child ;
						string traceParent = parentMap[child];
						while(traceParent != string("NONE"))
						{
							cout << " <- " << traceParent;
							traceParent = parentMap[traceParent];
						}
						cout<<endl;
						return;
					}
					parentMap[child]=parentNode.state;
					node childNode(child, &parentNode, parentNode.path_cost + dfs_stepCost);
					//cout << "Address :::: (" << child << " , " << &childNode << ") its parent is : (" << parentNode.state << " , " << &parentNode << ")" << endl;					
					frontier.push(childNode);visited.insert(child);
					cout << "stack push : " << childNode.state << endl;
				}
			}
		}
		
	}
}

void ucs_srch(string source, set<string>& destinationNodes, map<string, set<string> >& adjacenyMap, int startTime, ofstream& ofs, map< pipeKey, pipeProperty>& pipes)
{
	cout << "Run UCS Search on source " << source << endl;
	
	map<string, string> parentMap;
	typedef map<string, string>::iterator parentMapItr;

	parentMap[source]=string("NONE");
	node* srcNodePtr = new node(source, NULL, 0);//path_cost = 0
	
	priority_queue<node*,vector<node*>, CompareNode > frontier;
	priority_queue<node*,vector<node*>, CompareNode > temp_frontier;
	map<string, int> frontier_states_path;

	frontier.push(srcNodePtr);frontier_states_path[srcNodePtr->state]=srcNodePtr->path_cost;
	cout<< "Ravi : pushing to frontier : " << srcNodePtr->state << " with "<< endl;
	srcNodePtr->display();
	set<string> explored;
	
	while(1)
	{
		if(frontier.empty())
		{
			//failure
			cout << "None of Destination can be reached" << endl;
			//write "None" to output file
			ofs << "None" << endl;
			return;			
		}
		node* poppedNodePtr = frontier.top();frontier.pop();frontier_states_path.erase(poppedNodePtr->state);
		cout<< "********************************" << endl;
		cout<< "Ravi : popped from frontier : " << poppedNodePtr->state << endl;
		poppedNodePtr->display();
		
		int pipe_length = 0;
		if(poppedNodePtr->state != source)//first node handling and check for bad_pipe: if bad_pipe:skip and go to next
		{
			string parentOfPoppedNode = (poppedNodePtr->parent)->state;//or parentMap[poppedNodePtr->state]
			cout << "Ravi : find a pipe bw : " << parentOfPoppedNode << " and " << poppedNodePtr->state << endl;
			map< pipeKey, pipeProperty>::iterator pipesItr = pipes.find(make_pair(parentOfPoppedNode, poppedNodePtr->state));
			bool badPipe = false;
			if(pipesItr != pipes.end())//found pipe
			{
				//check to see if cur_time(in 0-23 hr format) is not in the pipe off_periods
				int cur_time = (poppedNodePtr->parent)->path_cost + startTime;
				int cur_time_24_hr = cur_time % 24;
				cout << "cur_time at parent node( " <<(poppedNodePtr->parent)->state << " ) is : " << cur_time << " cur_time_24_hr : " << cur_time_24_hr << endl;				
				(pipesItr->second).display();
				pipe_length = (pipesItr->second).length;//capture pipe_length hoping its a good pipe
				if((pipesItr->second).no_offPeriods > 0)
				{
					set< pair<int,int> >offPeriods = (pipesItr->second).offPeriods;
					for(set< pair<int,int> >::iterator it = offPeriods.begin(); it!= offPeriods.end(); ++ it)
					{
						//cout << " (" << (*it).first << ", " << (*it).second << " ) ";
						int start_offPeriod = (*it).first;
						int end_offPeriod = (*it).second;
						if((start_offPeriod <= cur_time_24_hr) && (cur_time_24_hr <= end_offPeriod))
						{
							cout << "BadPipe : cur_time = " << cur_time << " cur_time_24_hr : " << cur_time_24_hr << " is in OffPeriod : (" << (*it).first << ", " << (*it).second << " ) " << endl;
							badPipe = true;
							break;//exit for
						}
					}
				}
			}
			if(badPipe)
			{
				continue;//ignore the bad pipe node and move to next, while
			}
		}
		if (goal_test(poppedNodePtr->state, destinationNodes))
		{
			//as the current goal child is not added to queue just update its parent in parentMap
			if(poppedNodePtr->state != source)
			{
				parentMap[poppedNodePtr->state]=(poppedNodePtr->parent)->state;
			}
			cout << "Goal is found at " << poppedNodePtr->state << endl;		
			//write result to output file
			int total_cost = poppedNodePtr->path_cost + startTime;
			cout << "goal found : " << poppedNodePtr->state << " with cost : " << total_cost << " (with startTime = " << startTime << " )"<< endl;
			ofs << poppedNodePtr->state << " " << total_cost%24 << endl;
			//cout<< "Display Parents : "<< endl;
			//for(parentMapItr parentItr = parentMap.begin(); parentItr != parentMap.end(); ++parentItr)
			//	cout << "Parent[ " << parentItr->first << " ] = " << parentItr->second << endl;
			cout << "path to goal : " << poppedNodePtr->state ;
			string traceParent = parentMap[poppedNodePtr->state];
			while(traceParent != string("NONE"))
			{
				cout << " <- " << traceParent;
				traceParent = parentMap[traceParent];
			}
			return;
		}
		
				
		explored.insert(poppedNodePtr->state);
		cout<< "Ravi : Added to explored set : { ";
		for(set<string>::iterator it = explored.begin(); it!= explored.end(); ++ it)
		{
			cout << *it << " , ";
		}
		cout<< " } " << endl;
		
		if(poppedNodePtr->state != source)
		{			
			parentMap[poppedNodePtr->state]=(poppedNodePtr->parent)->state;
			cout<< "Ravi : update : parentMap[" << poppedNodePtr->state << "] = " << (poppedNodePtr->parent)->state << endl; 
		}
		
		map<string, set<string> >::iterator adjacenyMapItr = adjacenyMap.find(poppedNodePtr->state);
		if(adjacenyMapItr != adjacenyMap.end())
		{
			set<string>& children = adjacenyMapItr->second;
			for(set<string>::iterator it = children.begin(); it!= children.end(); ++ it)
			{
				string child = *it;
				cout<< "Ravi : processing " << poppedNodePtr->state << "'s child : " << *it << endl;
				if ((explored.find(child) == explored.end()) && !(found_in_frontier_states_path(child, frontier_states_path)) ) //not in explored and not in frontier
				{	
					cout<< "Ravi : child : " << *it << " not in explored and not in frontier "<< endl;
					map< pipeKey, pipeProperty>::iterator pipesItr = pipes.find(make_pair(poppedNodePtr->state,child));					
					if(pipesItr != pipes.end())//found pipe
					{
						pipe_length = (pipesItr->second).length;
						cout<< "Ravi : found pipe_length : " << pipe_length << endl;
					}
					node* childNodePtr = new node(child, poppedNodePtr, poppedNodePtr->path_cost + pipe_length);					
					frontier.push(childNodePtr);frontier_states_path[childNodePtr->state]=childNodePtr->path_cost;//frontier_states.insert(child);
					cout<< "Ravi : pushing to frontier : " << childNodePtr->state << " with path_cost : " << childNodePtr->path_cost << endl;
					childNodePtr->display();
				}
				else if (found_in_frontier_states_path(child, frontier_states_path))
				{
					//iterate over priority_queue(frontier) and find the existing path_cost
					int existing_path_cost = frontier_states_path[child];
					int new_pipe_length = 0;
					int new_path_cost = 0;
					
					map< pipeKey, pipeProperty>::iterator pipesItr = pipes.find(make_pair(poppedNodePtr->state,child));					
					if(pipesItr != pipes.end())//found pipe
					{
						new_pipe_length = (pipesItr->second).length;						
					}
					new_path_cost = poppedNodePtr->path_cost + new_pipe_length;
					cout<<"Ravi Raju : existing path from : " << (poppedNodePtr->parent)->state << " to " << child << " has path_cost : " << existing_path_cost << endl;	
					cout<<"Ravi Raju : new path from : " << poppedNodePtr->state << " to " << child << " has path_cost : " << new_path_cost << endl;					
					if(existing_path_cost > new_path_cost)
					{
						
						//cout<<"Ravi : existing path from : " << (poppedNodePtr->parent)->state << " to " << child << " has path_cost : " << existing_path_cost << endl;				
						//cout<<"Ravi : frontier size before delete : " << frontier.size()<<endl;
						//pop each node in frontier and push it temp_frontier until we find the node to be replaced
						while ( !frontier.empty() )  
						{  
							node* n = frontier.top(); frontier.pop(); 
							if(n->state == child)
							{
								cout << "Ravi : found and deleted node " << child << " from frontier" << endl;
								delete n;
								break;
							}
							else
							{
								cout << "Ravi : pushing to others to temp_frontier : " << n->state << endl;
								temp_frontier.push(new node(n->state, n->parent, n->path_cost));
								delete n;
							}
						}
						//move from temp_frontier back to actual frontier
						while ( !temp_frontier.empty() )  
						{							
							node* n = temp_frontier.top(); temp_frontier.pop(); 
							cout << "Ravi : moving others from temp_frontier to frontier: " << n->state << endl;
							frontier.push( new node(n->state, n->parent, n->path_cost));
							delete n;
						}
						//cout<<"Ravi : frontier size after delete : " << frontier.size() << endl;
						node* childPtr = new node(child, poppedNodePtr, new_path_cost);
						frontier.push(childPtr);
						cout << "Ravi : Updated node " << childPtr->state <<" with optimal new_path_cost : " << (childPtr->path_cost) << endl;
						//cout<<"Ravi : new path from : " << (childPtr->parent)->state << " to " << childPtr->state << " has path_cost : " << (childPtr->path_cost) << endl;
						//cout<<"Ravi : frontier size after add : " << frontier.size()<<endl;
					}
				}
			}
		}	
	}

}


int main(int argc, char *argv[])
{
	//g++ waterFlow.cpp –o waterFlow.o
	//./waterFlow.o –i inputFile
	
	ofstream ofs("output.txt", ios::out | ios::trunc);
	if(!ofs.is_open())
	{
		cout << "Cannot open output file stream" << endl;
		return -1;
	}
	if( argc != 3)
	{
		cout << "Invalid input format. Usage : ./waterFlow.o -i inputFile" << endl;
		return 0;
	}
	if ( strncmp(argv[1],"-i",sizeof("-i")) != 0)
	{
		cout << "Invalid option" << argv[1] << " . Usage : ./waterFlow.o -i inputFile" << endl;
		return 0;
	}
	
	ifstream ifs(argv[2]);
	string line;
	int noOfTestCases = 0;
	////////////////////////////NO OF TEST CASES////////////////////////////////////
	getline(ifs, line);
	noOfTestCases = atoi(line.c_str());
    cout << "No of Test Cases : " << noOfTestCases << endl;
	for ( int i =0 ; i< noOfTestCases ; i++)
	{
		
		
		map< pipeKey, pipeProperty> pipes;// EDGES INFO of GRAPH
		typedef map< pipeKey, pipeProperty>::iterator pipesItr;
		
		//EDGES of GRAPH
		map<string, set<string> 				  > adjacenyMap;
		map<string, set<string, greater<string> > > dfsAdjacenyMap;
		typedef map<string, set<string> >::iterator adjacenyMapItr;
		typedef map<string, set<string, greater<string> > >::iterator dfsAdjacenyMapItr;
		
		cout << endl << endl;
		cout << "Test Case : " << i+1 << endl; 
		////////////////////////////TEST CASE TASK////////////////////////////////////
		getline(ifs,line);
		string curTask = line;
		string bfsTask("BFS"), dfsTask("DFS"), ucsTask("UCS");
		bool bfs = false, dfs = false, ucs = false;
		//cout << curTask << endl;
		if (curTask.compare(bfsTask) == 0)
		{
			//cout << "Run BFS" << endl;
			bfs = true;
		}
		else if (curTask.compare(dfsTask) == 0)
		{
			cout << "Run DFS" << endl;
			dfs = true;
		}
		else if (curTask.compare(ucsTask) == 0)
		{
			cout << "Run UCS" << endl;
			ucs = true;
		}
		else
		{
			cout << "Invalid Search Task : " << curTask;
			return 0;
		}
		////////////////////////////SRC NODE////////////////////////////////////
		getline(ifs,line);
		string src = line;
		cout << "src : " << src << endl;
		
		pair<set<string>::iterator,bool> inserted;
		set<string>::iterator strSetItr;
		////////////////////////////DESTINATION NODES////////////////////////////////////
		getline(ifs,line);
		string destNodesString = line;
		//cout << "destNodesString :" << destNodesString;
		stringstream dss (destNodesString);
		set<string> destNodes;
		string destNode;
		while(dss >> destNode)
		{
			inserted = destNodes.insert(destNode);
			if (inserted.second == false)
				cout << "Duplicate destination : " << destNode << " not inserted" << endl;
		}
		cout << "Destination Nodes are:";
		for (strSetItr=destNodes.begin(); strSetItr!=destNodes.end(); ++strSetItr)
			cout << ' ' << *strSetItr;
		cout << endl;
		////////////////////////////MID NODES////////////////////////////////////
		getline(ifs,line);
		string midNodesString = line;
		//cout << "midNodesString :" << midNodesString;
		stringstream mss (midNodesString);
		set<string> midNodes;
		string midNode;
		while(mss >> midNode)
		{
			inserted = midNodes.insert(midNode);
			if (inserted.second == false)
				cout << "Duplicate middle : " << midNode << " not inserted" << endl;
		}
		cout << "Middle Nodes are:";
		for (strSetItr=midNodes.begin(); strSetItr!=midNodes.end(); ++strSetItr)
			cout << ' ' << *strSetItr;
		cout << endl;
		int noOfPipes = 0;
		////////////////////////////NO OF PIPES////////////////////////////////////
		getline(ifs, line);
		noOfPipes = atoi(line.c_str());
		cout << "No of Pipes : " << noOfPipes << endl;
		for ( int i =0 ; i< noOfPipes ; i++)
		{
			pipeProperty property;
			getline(ifs, line);
			cout << "Pipe " << i+1 << " : " << line << endl;
			string pipeInfo = line;
			stringstream pss (pipeInfo);
			list<string> attributes;
			string attribute;
			while(pss >> attribute)
				attributes.push_back(attribute);
			//cout << "Attributes are:";
			//for (list<string>::iterator listItr=attributes.begin(); listItr!=attributes.end(); ++listItr)
			//	cout << ' ' << *listItr;
			//cout << endl;
			
			string startPoint = attributes.front();
			//cout << "startPoint : " << startPoint;
			attributes.pop_front();
			string endPoint = attributes.front();
			//cout << " endPoint : " << endPoint << endl;
			attributes.pop_front();
			
			int length = atoi((attributes.front()).c_str());
			//cout << "length : " << length;
			property.length = length;
			attributes.pop_front();
			int noOffPeriods = atoi((attributes.front()).c_str());
			//cout << " noOffPeriods : " << noOffPeriods << endl;
			property.no_offPeriods = noOffPeriods;
			attributes.pop_front();
			//cout << attributes.size();
			if (noOffPeriods != attributes.size())
			{
				cout << "No of OffPeriods : " << noOffPeriods << " do not match set of offPeriods specified : ";
				for (list<string>::iterator listItr=attributes.begin(); listItr!=attributes.end(); ++listItr)
				cout << ' ' << *listItr;
				return 0;
			}
			if(noOffPeriods > 0)
			{
				while(!attributes.empty())
				{
					stringstream ss(attributes.front());
					attributes.pop_front();
					string tok;
					list<string> time;
					while(getline(ss, tok, '-'))
						time.push_back(tok);
					int startTime = atoi((time.front()).c_str()); time.pop_front();
					int endTime = atoi((time.front()).c_str()); time.pop_front();
					//cout << "(startTime, endTime) : (" << startTime << ", " << endTime << ")" << endl;
					property.offPeriods.insert(make_pair(startTime, endTime));
				}
			}
			//property.display();
			pipes[make_pair(startPoint, endPoint)] = property;
			if(dfs)
				(dfsAdjacenyMap[startPoint]).insert(endPoint);
			else
				(adjacenyMap[startPoint]).insert(endPoint);			
		}
		int startTime = 0;
		////////////////////////////START TIME////////////////////////////////////
		getline(ifs, line);
		startTime = atoi(line.c_str());
		cout << "Algo Start Time : " << startTime << endl;
		/*cout << "PipesMap :" << endl;
		for(pipesItr it = pipes.begin(); it != pipes.end(); ++it)
		{
			//it->first=======pair<string, string> pipeKey
			//it->second=======pipeProperty
			cout << "Pipe (" << (it->first).first << ", " << (it->first).second << " )" << endl ;
			(it->second).display();
		}*/
		if(dfs)
		{
			cout << "DfsAdjacencyMap :" << endl;
			for(dfsAdjacenyMapItr it = dfsAdjacenyMap.begin(); it != dfsAdjacenyMap.end(); ++it)
			{
				//it->first=======string
				//it->second=======set<string>
				cout << (it->first) << " : " ;
				for(strSetItr = (it->second).begin(); strSetItr != (it->second).end(); ++strSetItr)
					cout << ' ' << *strSetItr;
				cout << endl;
			}
		}
		else
		{
			cout << "AdjacencyMap :" << endl;
			for(adjacenyMapItr it = adjacenyMap.begin(); it != adjacenyMap.end(); ++it)
			{
				//it->first=======string
				//it->second=======set<string>
				cout << (it->first) << " : " ;
				for(strSetItr = (it->second).begin(); strSetItr != (it->second).end(); ++strSetItr)
					cout << ' ' << *strSetItr;
				cout << endl;
			}
		}
		
		if(bfs)
			bfs_srch(src, destNodes, adjacenyMap, startTime, ofs);
		
		if(dfs)
			dfs_srch(src, destNodes, dfsAdjacenyMap, startTime, ofs);
		
		if(ucs)
			ucs_srch(src, destNodes, adjacenyMap, startTime, ofs, pipes);
		
		while(ifs)
		{
			getline(ifs,line);
			if (line.length() == 0)
				break;
			cout << line << endl;
		}
	}
    ifs.close();
		
	
	return 0;
}