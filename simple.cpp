#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
#include<string>
#include<list>
#include<map>
#include<cstring>
#include<queue>
#include<cstdlib>
#include<climits>

using namespace std;

class Node;
class Edge;
class Path;

//#define DEBUG
#define PI 3.14159265358979323846

vector<Path*> mp;

class Node
{

public:
	Node(string mname)
	{
		name = mname;
		visited = false;
		weight = lat = lng = 0.0;
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
	string    name;
	bool      visited;
	double       weight;
	double		 lat;
	double		 lng;
	class Node      *back_pointer;
	list<class Edge*> neighbors;
};

class Edge
{
public:
	class Node * mdest;
	double eweight;

	Edge(double c,Node *md = NULL)
	{
		eweight = c;
		mdest = md;
	}
	~Edge(){
		if(mdest)  delete mdest;
	}


};

map<string,class Node*> nodes;

class Path
{
public:

	void setweight(double thr)
	{
		total = thr;
	}


	void setP(string str)
	{
		mpath.push_back(str);
	}

	void pushb()
	{
		mp.push_back(this);
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

struct ca
{  
   bool operator()(class Node * &a, class Node * &b)const{
      return b->weight < a->weight;      
   }
};


Node* findnode(string name)
{
	Node* cc = nodes[name];
	if ( cc == 0 )
	{
		nodes[name] = new Node(name);
		cc = nodes[name];
	}

	return cc;
}

void rnodes()
{
	map<string,class Node*>::iterator count;
	for(count = nodes.begin(); count != nodes.end(); count++){
		(*count).second->weight = 0; 
		(*count).second->back_pointer = 0;
		(*count).second->visited = false;
	}
}

void dan(string s,string t)
{
	int flag=0;	
	Node* source = nodes[s];
	Node* target = nodes[t];

	rnodes();

	priority_queue<Node *, deque<Node*>, ca> queue_r;
	queue_r.push(source);
	while (!queue_r.empty()){

		Node* curr = queue_r.top(); 
		queue_r.pop();
		curr->visited = true;

		list<Edge*>::iterator edge;
		for(edge = curr->neighbors.begin(); edge != curr->neighbors.end(); edge++){
			Node *ne = (*edge)->mdest;
			if(!ne->visited){
				ne->weight += (*edge)->eweight + curr->weight;
				ne->visited = true;
				ne->back_pointer = curr;
				queue_r.push(ne);
			}
			else{
			} 
		}

	}

	Path *pathr = new Path();
	Node *ner = target;		
	if (ner->back_pointer==NULL)
	{
		pathr->setweight(-1);
		mp.push_back(pathr);
		return;
	}

	while (ner->back_pointer!=NULL)
	{
		//cout << "id:" << ne->name << "," << ne->weight << endl;
		pathr->setP(ner->name);
		if (!flag)  pathr->setweight(ner->weight),flag=1;
		ner = ner->back_pointer;
	}

	pathr->setP(ner->name);
	mp.push_back(pathr);
	//cout << "id:" << source->name << "," << source->weight << endl;
	
}

double ConvertDegreeToRadians(double degrees)
{
	return (degrees/180.0)*PI;
}

double ConvertRadiansToDegree(double radians)
{
	return (180.0/PI) * radians;
}

double GetDistance(double Lat1, double Long1, double Lat2, double Long2)
{
#if 0
	double Lat1r = ConvertDegreeToRadians(Lat1);
	double Lat2r = ConvertDegreeToRadians(Lat2);
	double Long1r = ConvertDegreeToRadians(Long1);
	double Long2r = ConvertDegreeToRadians(Long2);

	cout << Lat1 << "," << Lat2 << "," << Long1 << "," << Long2 << endl;
	cout << Lat1r << "," << Lat2r << "," << Long1r << "," << Long2r << endl;

	double R = 6371;
	double d = acos(sin(Lat1r) * sin(Lat2r) + 
			        cos(Lat1r) * cos(Lat2r) * cos(Long2-Long1))* R;

	return d;
#endif

#if 1
	double dLat = ConvertDegreeToRadians(Lat2 - Lat1);
	double dLon = ConvertDegreeToRadians(Long2 - Long1);

	double lat1r = ConvertDegreeToRadians(Lat1);
	double lat2r = ConvertDegreeToRadians(Lat2);
	double long1r = ConvertDegreeToRadians(Long1);
	double long2r = ConvertDegreeToRadians(Long2);

	double a = sin(dLat/2) * sin(dLat/2) +
        	    cos(lat1r) * cos(lat2r) * sin(dLon/2) * sin(dLon/2); 
	double R = 6367;
	double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
	double d = R * c;

	return round(d);
#endif
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
	class Node *rnode;
	class Node *srcnode, *dstnode;
	class Edge *medge;

	int weight=0;
	char * pch;
	char buffer[255]="";
	char data[255]="";
	int count=0;
	int loop=0;
	string dstart,dend;
	int format=0;
    	cin >> file_path;

	loop=1;
	//FILE *file = fopen(file_path, "r");
	if (!strcmp(file_path,"-1")) exit(1);
	
	ifstream fin(file_path);
	//FILE *file = fopen(file_path, "r");

	if(!fin)
	{    
		cout << file_path << "file error" << endl;   
		exit(1);
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

						string ko(data);
						rnode = findnode(ko);
						
						//fetch lat
						getline(fin, sp);
						sp = trim(sp);
						strcpy( buffer, sp.c_str() );
						ct=0;

						char cdata[255]="";
						int inputlng=0;
						for (int j=0; buffer[j] != '>' ;j++,ct++)
							cdata[ct] = buffer[j];
						cdata[ct]='\0';
						ct=0;
						for (int j=5; buffer[j] != '<' ;j++,ct++)
							data[ct] = buffer[j];
						data[ct]='\0';

						if (!strcmp(cdata, "<lat"))
						{
							inputlng=1;
							rnode->setLat(atof(data));
						}
						else
						{
							inputlng=0;
							rnode->setLng(atof(data));
						}

						//fetch lng
						getline(fin, sp);
						sp = trim(sp);
						strcpy( buffer, sp.c_str() );
						ct=0;
						for (int j=5; buffer[j] != '<' ;j++,ct++)
							data[ct] = buffer[j];
						data[ct]='\0';

						if (inputlng == 0)
						{
							rnode->setLat(atof(data));
						}
						else
						{
							rnode->setLng(atof(data));
						}
						
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
							string ok(data);
							srcnode = findnode(ok);

					        	//a = findnode(data);
							ct=0;
							continue;
						}

						data[ct] = buffer[j];
						ct++;
					}
					data[ct]='\0';
					string ok(data);
					dstnode = findnode(ok);
					#ifdef DEBUG
						cout << srcnode->name << " -> " << dstnode->name << endl;
						cout << srcnode->getLat() << "," << srcnode->getLng() << endl;
						cout << dstnode->getLat() << "," << dstnode->getLng() << endl;
					#endif

					double weight = GetDistance(srcnode->getLat(), srcnode->getLng(), dstnode->getLat(), dstnode->getLng());
					
					#ifdef DEBUG
						cout << weight << endl;
					#endif

					medge = new Edge(weight, dstnode);
					srcnode->neighbors.push_back(medge);

					getline(fin, sp);
					sp = trim(sp);
					if (!sp.compare("</geometry>")) break;
				}
			}
			else
			{
				format=1;
			}
		}

		if (format == 1)
		{
			ifstream other(file_path);
			//FILE *file = fopen(file_path, "r");

			if(!other)
			{    
				cout << file_path << "file error" << endl;   
				exit(1);
			}
			string aid,bid;

			while (other >> aid >> bid >> weight)
			{
				srcnode = findnode(aid);
				dstnode = findnode(bid);
				medge = new Edge(weight, dstnode);
				srcnode->neighbors.push_back(medge);
				
			} 
		}

		while (1)
		{
			cin >> dstart;
			if (dstart == "-1")
					break;

			cin >> dend;
			dan(dstart, dend);
			//cout << node_map;
		}
		for (int i=0; i<mp.size(); i++)
		{
			Path *mpr = mp[i];
			mpr->print();
		}


	return 0;
}
