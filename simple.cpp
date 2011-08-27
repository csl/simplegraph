#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
#include<string>
#include<cstring>
#include<climits>
//#include<stdio.h>
#include<stdlib.h>

//#define DOUBLE_MAX  99999999999
//#define DEBUG

using namespace std;

class Node;
class Edge;
class Path;

const double PI = 4.0*atan(1.0);

vector<Node*> nodes;
vector<Edge*> edges;
vector<Path*> paths;

class Node
{
public:
	Node(char* mid)
	{
		strcpy(id, mid);
		previous = NULL;
		dStart = INT_MAX;
		nodes.push_back(this);
		lat=0;
		lng=0;
	}

	void setLat(double mlat)
	{
		lat = mlat;
	}

	void setLng(double mlng)
	{
		lng = mlng;
	}

	double getLat()
	{
		return lat;
	}

	double getLng()
	{
		return lng;
	}
	
public:
	char id[255];
	class Node* previous;
	double lat;
	double lng;
	double dStart;
};

class Edge
{
public:
	Edge(Node* node1, Node* node2, double distance)
		: node1(node1), node2(node2), distance(distance)
	{
		edges.push_back(this);
	}

	bool Connects(Node* node1, Node* node2)
	{
		return ((node1 == this->node1 && node2 == this->node2));
	}
public:
	Node* node1;
	Node* node2;
	double distance;
};

class Path
{
public:
	  Path(double th, int rep)
	  {
		if (rep == -1)
		 total = -1;
		else
		 total = th;
		 //path.push_back(this);
	  }

	  void setP(string str)
	  {
			mpath.push_back(str);
	  }

	  void pushb()
	  {
		 paths.push_back(this);
	  }

	  void print()
	  {
			if (mpath.size()==1)
			{
				cout << -1 << endl;
				return;
			}
			cout << total << " ";
			if (total != -1)
			{
				for (int i=mpath.size()-1; i>=0; i--)
				{
					cout << mpath[i] << " ";
				}
			}
			cout << endl;
      }

private:
	 double total;
	 vector<string> mpath;
};


Node* Et(vector<Node*>& nodes)
{
	int size = nodes.size();
	if (size == 0) return NULL;

	int smallestPosition = 0;
	Node* smallest = nodes.at(0);
	for (int i=1; i<size; ++i)
	{
		Node* current = nodes.at(i);
		if (current->dStart < smallest->dStart)
		{
			smallest = current;
			smallestPosition = i;
		}
	}
	nodes.erase(nodes.begin() + smallestPosition);
	return smallest;
}

bool Contains(vector<Node*>& nodes, class Node* node)
{
	const int size = nodes.size();
	for(int i=0; i<size; ++i)
	{
		if (node == nodes.at(i))
		{
			return true;
		}
	}
	return false;
}

vector<Node*>* AdjacentNodes(Node* node)
{
	vector<Node*>* adjacentNodes = new vector<Node*>();
	const int size = edges.size();
	for(int i=0; i<size; ++i)
	{
		Edge* edge = edges.at(i);
		Node* adjacent = NULL;
		if (edge->node1 == node)
		{
			adjacent = edge->node2;
		}
		else if (edge->node2 == node)
		{
			adjacent = edge->node1;
		}
		if (adjacent && Contains(nodes, adjacent))
		{
			adjacentNodes->push_back(adjacent);
		}
	}
	return adjacentNodes;
}

void PrintRoute(Node* dest, int rep)
{
	Node* p = dest;
	Path* path = NULL;

	if (dest == NULL)
	{
		return;
	}
	else
	{
		path = new Path(dest->dStart, rep);
	}

	if (dest->dStart != -1)
	{
	  if (rep != -1)
	  {
		while (p)
		{
		//cout << previous->id << " ";
			path->setP(p->id);
			p = p->previous;
		}
           }
	}
	//cout << endl;
	path->pushb();
}

double Distance(Node* node1, Node* node2)
{
	const int size = edges.size();
	for(int i=0; i<size; ++i)
	{
		Edge* edge = edges.at(i);
		if (edge->Connects(node1, node2))
		{
			return edge->distance;
		}
	}
	return -1; // should never happen
}

int Das()
{
	while (nodes.size() > 0)
	{
		Node* smallest = Et(nodes);
		vector<Node*>* adjacentNodes =
			AdjacentNodes(smallest);

		const int size = adjacentNodes->size();
		for (int i=0; i<size; ++i)
		{
			Node* adjacent = adjacentNodes->at(i);
			double ds = Distance(smallest, adjacent);
			if (ds == -1) 
			{
				//unlink
				return -1;
			}
			double distance =  ds + smallest->dStart;
			if (distance < adjacent->dStart)
			{
				adjacent->dStart = distance;
				adjacent->previous = smallest;
			}
		}
		delete adjacentNodes;
	}

	return 1;
}

class Node* findnode(char *p)
{
    const int size = nodes.size();
    for(int i=0; i<size; ++i)
    {	
        if (!strcmp(p, nodes.at(i)->id))
        {
		 	//cout << "match: " << nodes.at(i)->id << " = " << p << endl;
			return nodes.at(i);
        }
    }

	return NULL;
}

double ConvertDegreeToRadians(double degrees)
{
	return (PI/180) * degrees;
}

double GetDistance(double Lat1, double Long1, double Lat2, double Long2)
{
	double Lat1r = ConvertDegreeToRadians(Lat1);
	double Lat2r = ConvertDegreeToRadians(Lat2);
	double Long1r = ConvertDegreeToRadians(Long1);
	double Long2r = ConvertDegreeToRadians(Long2);

	cout << Lat1 << "," << Lat2 << "," << Long1 << "," << Long2 << endl;

	double R = 6371;
	double d = acos(sin(Lat1r) * sin(Lat2r) + 
			        cos(Lat1r) * cos(Lat2r) * cos(Long2r-Long1r))* R;
	return d;
}


string LTrim(const string& str)
{
	return str.substr(str.find_first_not_of(" \n\r\t"));
}

string RTrim(const string& str)
{
	return str.substr(0,str.find_last_not_of(" \n\r\t")+1);
}

string trim(const string& str)
{
	if (!str.compare("")) return str;
	return LTrim(RTrim(str));
} 
int main(int argc, char **argv)
{
	string sp;
	char file_path[255]="";
	class Node* a = NULL;
    	class Node* b = NULL;
	int weight=0;
	char * pch;
	char buffer[255]="";
	char data[255]="";
	int count=0;
	int loop=0;
	char dstart[255]="",dend[255]="";

    	cin >> file_path;

	while (1)
	{
		nodes.clear();
		edges.clear();
		//FILE *file = fopen(file_path, "r");
		if (!strcmp(file_path,"-1")) break;
		
		ifstream fin(file_path);
		//FILE *file = fopen(file_path, "r");

		if(!fin)
		{    
			cout << file_path << "file error" << endl;   
			break;
		}

	    while (!fin.eof())
	    {
			getline(fin, sp);
			strcpy( buffer, sp.c_str() );
			sp = trim(sp);
			if (!sp.compare("")) 
					break;
			else if (!sp.compare("<geometry>"))
			{
				//handler: loaction
				while (1)
				{
					getline(fin, sp);
					sp = trim(sp);
				
					if (!sp.compare("<location>"))
					{
						//printf("location handling...\n");
						//fetch id
						getline(fin, sp);
						sp = trim(sp);
						strcpy( buffer, sp.c_str() );

						int ct=0;
						for (int j=4; buffer[j] != '<' ;j++,ct++)
							data[ct] = buffer[j];
						data[ct]='\0';

						a = findnode(data);
						if (a == NULL)  a = new Node(data);
						
						//fetch lat
						getline(fin, sp);
						sp = trim(sp);
						strcpy( buffer, sp.c_str() );
						ct=0;
						for (int j=5; buffer[j] != '<' ;j++,ct++)
							data[ct] = buffer[j];
						data[ct]='\0';

						a->setLat(atof(data));

						//fetch lng
						getline(fin, sp);
						sp = trim(sp);
						strcpy( buffer, sp.c_str() );
						ct=0;
						for (int j=5; buffer[j] != '<' ;j++,ct++)
							data[ct] = buffer[j];
						data[ct]='\0';
						a->setLng(atof(data));
						
						continue;
					}
					else if (!sp.compare("</location>"))
					{
						continue;
					}

					break;
				}
				
				//handler: link
				while (1)
				{
					strcpy( buffer, sp.c_str() );
					int ct=0;
					int j=0;

					for (j=0; buffer[j] != '>' ;j++) ;
					j++;
					for (; buffer[j] != '<' ;j++)
					{
						if (buffer[j] == ' ')
						{
							data[ct]='\0';
					        	a = findnode(data);
							ct=0;
							continue;
						}

						data[ct] = buffer[j];
						ct++;
					}
					data[ct]='\0';
				       b = findnode(data);
					#ifdef DEBUG
						cout << a->id << " -> " << b->id << endl;
						cout << a->getLat() << "," << a->getLng() << endl;
						cout << b->getLat() << "," << b->getLng() << endl;
					#endif

					double weight = GetDistance(a->getLat(), a->getLng(), 
							b->getLat(), b->getLng());
					
					#ifdef DEBUG
						cout << weight << endl;
					#endif

					Edge* e1 = new Edge(a, b, weight);

					getline(fin, sp);
					if (!sp.compare("</geometry>\r")) break;
				}
			}
			else
			{
	  			pch = strtok (buffer," ");			

				while (pch != NULL)
				{
			    	switch (count)
		        	{
						case 0:
							a = findnode(pch);
							if (a == NULL)  a = new Node(pch);
			    				count++;
							break;
						case 1:
							b = findnode(pch);
							if (b == NULL)  b = new Node(pch);
			    				count++;
							break;
						case 2:
							weight= atoi(pch);
							//printf("%s -> %s, w=%d\n", a->id, b->id, weight);
							Edge* e1 = new Edge(a, b, weight);
							count=0;
					    	break;
			     	}
			    	pch = strtok (NULL, " ");
				}
			}
		}

		cin >> dstart;
		if (!strcmp(dstart, "-1"))
				break;

		cin >> dend;

		//Cal ShortPath
		a = findnode(dstart);
		b = findnode(dend);
		a->dStart = 0.0; // set start node
		int rep = Das();
		PrintRoute(b, rep);
	}

	for (int i=0; i<paths.size(); i++)
	{
		Path *sp = paths[i];
		sp->print();
	}

	return 0;
}
