#include <iostream>  
#include <queue>  
using namespace std;  
  
class Node  
{  
private:  
    int value;  
  
public:  
    Node( int v )  : value( v ) {}  
    int Val() const { return value; }  
};  
  
struct CompareNode : public std::binary_function<Node*, Node*, bool>                                                                                       
{  
  bool operator()(const Node* lhs, const Node* rhs) const  
  {  
     return lhs->Val() > rhs->Val();  
  }  
};  
  
int main()  
{     
    priority_queue<Node*,vector<Node*>, CompareNode > pq;  
	priority_queue<Node*,vector<Node*>, CompareNode > temp_pq;  
  
    pq.push( new Node( 111 ) );  
    pq.push( new Node( 1111 ) );  
    pq.push( new Node( 1011 ) );  
    pq.push( new Node( 100 ) );  
    pq.push( new Node( 1110 ) );  
    pq.push( new Node( 101 ) );  

	
	int existing_val, new_value;
	cout << "Enter the node value to be replaced : ";
	cin >> existing_val;
	cout << "Enter the new value for the node : ";
	cin >> new_value;
	
    while ( !pq.empty() )  
    {  
        Node* n = pq.top(); pq.pop(); 
		if(n->Val() == existing_val)
		{
			cout << "found and replacing..." << endl;
			delete n;
			pq.push( new Node(new_value));
			break;
		}
		else
		{
			cout << "pushing to temp_pq : " << n->Val() << endl;
			temp_pq.push(new Node(n->Val()));
			delete n;
		}  
    }
	
	cout << "temp pq:";
	while ( !temp_pq.empty() )  
    {
		Node* n = temp_pq.top(); temp_pq.pop(); 
        cout << " " << n->Val();
		pq.push( new Node(n->Val()));
		delete n;
	}
	cout<<endl;
	
	cout << "After pq:";
	while ( !pq.empty() )  
    {
		Node* n = pq.top(); pq.pop(); 
        cout << " " << n->Val();
		delete n;
	}
	cout<<endl;
	
 
}  