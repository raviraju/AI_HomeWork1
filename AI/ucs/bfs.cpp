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

		explored.insert(parentNode.state);
		map<string, set<string> >::iterator adjacenyMapItr = adjacenyMap.find(parentNode.state);
		if(adjacenyMapItr != adjacenyMap.end())
		{
			set<string>& children = adjacenyMapItr->second;
			for(set<string>::iterator it = children.begin(); it!= children.end(); ++ it)
			{
				string child = *it;
				if ((explored.find(child) == explored.end()) && !(found_in_frontier(child, frontier_states)) ) //not in explored and not in frontier
				{
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
						return;
					}
					parentMap[child]=parentNode.state;
					node childNode(child, &parentNode, parentNode.path_cost + bfs_stepCost);
					//cout << "Address :::: (" << child << " , " << &childNode << ") its parent is : (" << parentNode.state << " , " << &parentNode << ")" << endl;					
					frontier.push(childNode);frontier_states.insert(child);
				}
			}
		}
		
	}