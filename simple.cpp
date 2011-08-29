#include<cmath>
#include<stdlib.h>
#include<fstream>
#include<cmath>
#include<string>
#include<iostream>
#include<climits>
#include<cmath>
#include<stdlib.h>
#include<list>
#include<map>
#include<queue>
#include<vector>
#include<cstring>

//#define DEBUG

using namespace std;

class Edge;

class Node
{
public:
	string    name;
	bool      visited;
	double       cost;
	double		 lat;
	double		 lng;
	class Node      *back_pointer;
	list<class Edge*> neighbors;

	Node(string const &n)
	{ 
		name = n;
		visited = false;
		cost = 0.0;
		lat=0.0;
		lng=0.0;
	}
};

class Tracker
{
public:
	  vector<string> id;
	  double cost;
	  void printTrakcer()
	   {
		cout << cost << " ";
		if (cost != -1)
		{
			for (int i=id.size()-1; i>=0; i--)
			{
				cout << id[i] << " ";
			}
		}
		cout << endl;
	   }
};

vector<Tracker*> tp;

class Edge 
{
public:
	double weight;
	class Node * dest;

	Edge(double c,Node *d = NULL):weight(c),dest(d){}

	~Edge(){if(dest)delete dest;}
};

class NodeMap{
public:
	Tracker track;	
	map<string,Node*> nodemap;
	
	Node* find_in_nodemap(string const &name)
	{
		Node* result = nodemap[name];
		if ( result == 0 )
			result = nodemap[name] = new Node(name);
		return result;
	}
	#if 0
	void setTracker(string src, string dst)
	{
		class Tracker *mytracker = new Trakcer();
        map<string,Node*>::iterator im;
        for(im = nm.nodemap.begin(); im != nm.nodemap.end(); im++)
        {
			if (src.compare((*im).second->name)) continue;
			mytracker->id.push_back(src.compare((*im).second->name))

            list<Edge*> neighbors = (*im).second->neighbors;
            list<Edge*>::iterator e;
            for(e = neighbors.begin(); e != neighbors.end(); e++)
			{
                cout << "   -> " << (*e)->dest->name << " weight " << (*e)->weight <<endl;
            }

        }
	}
	#endif

	friend ostream& operator<<(ostream& o, NodeMap nm){
		map<string,Node*>::iterator im;
		for(im = nm.nodemap.begin(); im != nm.nodemap.end(); im++)
		{
			o << (*im).second->name << endl;
	        list<Edge*> neighbors = (*im).second->neighbors;
			list<Edge*>::iterator e;
			for(e = neighbors.begin(); e != neighbors.end(); e++){
				cout << "   -> " << (*e)->dest->name << " weight " << (*e)->weight <<endl;
			}

		}
		return o;
	}
};

struct compare
{  
   bool operator()(Node * &a,Node * &b)const{
      return b->cost < a->cost;      
   }
};

void reset_nodes(NodeMap &nm)
{
	map<string,Node*>::iterator im;
	for(im = nm.nodemap.begin(); im != nm.nodemap.end(); im++){
		(*im).second->cost = 0; 
		(*im).second->back_pointer = 0;
		(*im).second->visited = false;
	}
}

void dijkstra(string s,string t,NodeMap &nodes){

	Node* source = nodes.nodemap[s];
	Node* target = nodes.nodemap[t];

	reset_nodes(nodes);

	priority_queue<Node *, deque<Node*>, compare>pq;
	pq.push(source);
	while(!pq.empty()){

		Node* curr = pq.top(); 
		pq.pop();
		curr->visited = true;


		list<Edge*>::iterator edge;
		for(edge = curr->neighbors.begin(); edge != curr->neighbors.end(); edge++){
			Node *ne = (*edge)->dest;
			if(!ne->visited){
				ne->cost += (*edge)->weight + curr->cost;
				ne->visited = true;
				ne->back_pointer = curr;
				pq.push(ne);
			}
			else{
			} 
		}

	}

	//find short path
	Tracker *tk = new Tracker();
	Node *ne = target;	
	
	if (ne->back_pointer==NULL)
	{
		tk->cost = -1;
		tp.push_back(tk);
		return;
	}

	int flag=0;	
	while (ne->back_pointer!=NULL)
	{
		//cout << "id:" << ne->name << "," << ne->cost << endl;
		tk->id.push_back(ne->name);
		if (!flag)  tk->cost = ne->cost,flag=1;
		ne = ne->back_pointer;
	}

	tk->id.push_back(source->name);
	tp.push_back(tk);
	//cout << "id:" << source->name << "," << source->cost << endl;
	
}

double ConvertDegreeToRadians(double degrees)
{
	return (M_PI/180) * degrees;
}

double GetDistance(double Lat1, double Long1, double Lat2, double Long2)
{
	double lat1r = ConvertDegreeToRadians(Lat1);
	double lat2r = ConvertDegreeToRadians(Lat2);
	double long1r = ConvertDegreeToRadians(Long1);
	double long2r = ConvertDegreeToRadians(Long2);

	double a = sin(ConvertDegreeToRadians(Lat2 - Lat1)/2) * sin(ConvertDegreeToRadians(Lat2 - Lat1)/2) + cos(lat1r) * cos(lat2r) * sin(ConvertDegreeToRadians(Long2 - Long1)/2) * sin(ConvertDegreeToRadians(Long2 - Long1)/2); 
	double R = 6367;
	double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
	double d = R * c;

	return round(d);
}

string trim(const string& str)
{
	if (!str.compare("")) return str;
	string s1 = str.substr(0,str.find_last_not_of(" \n\r\t")+1);
	string s2 = s1.substr(s1.find_first_not_of(" \n\r\t"));

	return s2;
	
}
 
int main(int argc, char **argv)
{
	char filename[30];
	string datain;
	string fsrc, fdst;
	class NodeMap node_map;
	class Node *mnode;
	class Node *src, *dst;
	class Edge *connector;
	//Source->neighbors.push_back(connector);
	char cdata[33]="", mdata[33]="";
	int mydata=0;
	int r=0, link=0;

   	cin >> filename;
	if (!strcmp(filename,"-1")) exit(1);

	while (1)
	{
		ifstream openfile(filename);

		if(!openfile)
		{    
			cout << "can't get file" << endl;   
			exit(1);
		}

	    while (!openfile.eof())
	    {
			getline(openfile, datain);
			strcpy( cdata, datain.c_str() );
			datain = trim(datain);
			//geometry tag
			if (datain.compare("<geometry>"))
			{
                char *dp = strtok (cdata," ");
				int dcounter=0;
				
                while (dp != NULL)
                {
                    if  (dcounter == 0)
                    {
						string from(dp);
						src = node_map.find_in_nodemap(from);
					}
					else if (dcounter == 1)
					{
						string to(dp);
						dst = node_map.find_in_nodemap(to);

					}
					else if (dcounter == 2)
					{
                            			int weight = atoi(dp);
						//printf("weight = %d\n",weight);
						connector = new Edge(weight, dst);
						src->neighbors.push_back(connector);
                   			 }
                    			dp = strtok (NULL, " ");
					dcounter++;
            	}          
			}
			else
			{
				while (1)
				{
					getline(openfile, datain);
					datain = trim(datain);
					//Location tag
					if (!datain.compare("<location>"))
					{
		       	      			int toDolatlng=0;
						getline(openfile, datain);
						datain = trim(datain);
						strcpy( cdata, datain.c_str() );

						mydata=0;
						r=4;
						while (1)
						{
							if (cdata[r] == '<') break;
							mdata[mydata] = cdata[r];
							mydata++;
							r++;
						}
						mdata[mydata]='\0';

						string from(mdata);
						mnode = node_map.find_in_nodemap(from);

					      getline(openfile, datain);
					      datain = trim(datain);
					      strcpy( cdata, datain.c_str() );

					      mydata=0;
					      char blatlng[100]="";
					      r=0;
					      while (1)
					      {
						 if (cdata[r] == '>') break;
						 blatlng[mydata] = cdata[r];
						 mydata++;
						 r++;
					      }
					      blatlng[mydata]='\0';

					      mydata=0;
					      r=5;
					      while (1)
					      {
						 if (cdata[r] == '<') break;
						 mdata[mydata] = cdata[r];
						 mydata++;
						 r++;
					      }
					      mdata[mydata]='\0';

					      if (!strcmp(blatlng, "<lat"))
					       {
					      	mnode->lat=atof(mdata);
						toDolatlng=1;
					       }
					      else
					      	mnode->lng=atof(mdata);

					      getline(openfile, datain);
					      datain = trim(datain);
					      strcpy( cdata, datain.c_str() );

					      mydata=0;
					      r=5;
				              while (1)
					        {
						 if (cdata[r] == '<') break;
						 mdata[mydata] = cdata[r];
						 mydata++;
						 r++;
					        }
                                          mdata[mydata]='\0';
					
					      if (toDolatlng == 0)
					      	mnode->lat=atof(mdata);
					      else
					      	mnode->lng=atof(mdata);
						
						continue;
					}
					else if (!datain.compare("</location>"))
					{
						continue;
					}

					break;
				}
				
				for (;;)
				{
					link=0;
					strcpy( cdata, datain.c_str() );

					while (cdata[link] != '>') link++;
					link++;

					mydata=0;
					while (cdata[link] != '<')
					{
						if (cdata[link] == ' ')
						{
							mdata[mydata]='\0';
							string id(mdata);
							src = node_map.find_in_nodemap(id);
							mydata=0;
							link++;
							continue;
						}
						
						mdata[mydata] = cdata[link];
						mydata++;
						link++;
					}
					mdata[mydata]='\0';
					string id(mdata);
					dst = node_map.find_in_nodemap(id);
					#ifdef DEBUG
						cout << "id: " << id << endl;
						cout << src->lat << "," << src->lng << endl;
						cout << dst->lat << "," << dst->lng << endl;
					#endif
					double weight = GetDistance(src->lat, src->lng, 
							dst->lat, dst->lng);
					
					#ifdef DEBUG
						cout << "weight: " <<  weight << endl;
					#endif

					connector = new Edge(weight, dst);
					src->neighbors.push_back(connector);
					
					getline(openfile, datain);
					datain = trim(datain);
					if (!datain.compare("</geometry>")) break;
				}
			}
		}

		#ifdef DEBUG
			//cout << node_map;
		#endif

		while (1)
		{
			cin >> fsrc;
			if (!strcmp(fsrc.c_str(), "-1"))
				break;
			cin >> fdst;
			dijkstra(fsrc, fdst, node_map);
			//cout << node_map;
		}


		for (int i=0; i<tp.size(); i++)
		{
			Tracker *trp = tp[i];
			trp->printTrakcer();
		}

		break;
	}

	return 0;
}
