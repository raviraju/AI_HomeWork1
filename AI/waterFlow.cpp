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
}node;

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

void bfs_srch(string source, set<string>& destinationNodes, map<string, set<string> >& adjacenyMap, int startTime, ofstream& ofs)
{
	int bfs_stepCost = 1;
	cout << "Run BFS Search on source " << source << endl;
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
	queue<node> frontier;
	set<string> visited;	
	frontier.push(srcNode);visited.insert(srcNode.state);
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
		explored.insert(parentNode.state);
		map<string, set<string> >::iterator adjacenyMapItr = adjacenyMap.find(parentNode.state);
		if(adjacenyMapItr != adjacenyMap.end())
		{
			set<string>& children = adjacenyMapItr->second;
			for(set<string>::iterator it = children.begin(); it!= children.end(); ++ it)
			{
				string child = *it;
				if ((explored.find(child) == explored.end()) && !(found_in_frontier(child, visited)) ) //not in explored and not in frontier
				{
					if(goal_test(child, destinationNodes))
					{
						parentMap[child]=parentNode.state;
						//as the current goal child is not added to queue
						int total_cost = parentNode.path_cost + bfs_stepCost + startTime;
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
						return;
					}
					parentMap[child]=parentNode.state;
					node childNode(child, &parentNode, parentNode.path_cost + bfs_stepCost);
					//cout << "Address :::: (" << child << " , " << &childNode << ") its parent is : (" << parentNode.state << " , " << &parentNode << ")" << endl;					
					frontier.push(childNode);visited.insert(child);
				}
			}
		}
		
	}
}

void dfs_srch(string source, set<string>& destinationNodes, map<string, set<string> >& adjacenyMap, int startTime, ofstream& ofs)
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
		map<string, set<string> >::iterator adjacenyMapItr = adjacenyMap.find(parentNode.state);
		if(adjacenyMapItr != adjacenyMap.end())
		{
			set<string>& children = adjacenyMapItr->second;
			for(set<string>::iterator it = children.begin(); it!= children.end(); ++ it)
			{
				string child = *it;
				if ((explored.find(child) == explored.end()) && !(found_in_frontier(child, visited)) ) //not in explored and not in frontier
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
		
		typedef pair<string, string> pipeKey;
		map< pipeKey, pipeProperty> pipes;// EDGES INFO of GRAPH
		typedef map< pipeKey, pipeProperty>::iterator pipesItr;
		
		//EDGES of GRAPH
		map<string, set<string> 				  > adjacenyMap;
		typedef map<string, set<string> >::iterator adjacenyMapItr;
		
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
		
		if(bfs)
			bfs_srch(src, destNodes, adjacenyMap, startTime, ofs);
		
		if(dfs)
			dfs_srch(src, destNodes, adjacenyMap, startTime, ofs);
		
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