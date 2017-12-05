#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <climits>
#include <ctime>
using namespace std;
  
//Class to hold all info for each graph
class Graph
{
        int vertice; // No. of vertices
        int** table; // Pointer to an array containing adjacency lists
        int** solution; //pointer containing solutions to each alg; overwritten on each alg.
        string type; //what kind of graph was generated or from file
    public:
        Graph(int vertex, string graphtype);		// Constructor
		void writeGraph(string filename);	//write graph to file
		void writeTime(double time, string c, string alg);	//write the time and other info to file
		void writeSolution(string c, string alg);	//write shortest path to file
		int minDistance(int distance[], bool set[]);	//find min distance for Dijkstra
		void dijkstra(string c);	//Dijkstra alg
		void floydWarshall(string c);	//floyd's alg
};

//contructor. Takes in # of verticies and what type of graph to generate. 
//3 types of graph generation:
//sequential, reverse, random
Graph::Graph(int vertex, string graphtype)
{
	
	int count = 0;
	if (graphtype == "sequential" || graphtype == "reverse" || graphtype == "random") //check if we're not reading from file.
	{
		this->vertice = vertex; //set vertice
		table = new int*[vertex];	//create dynamic 2d array
		for (int i = 0; i < vertex; i++)
		{
			table[i] = new int[vertex];
		}
		
	}
	else //we are reading from file!
	{
		this->type = "file"; //get the type
		
		//open file
		ifstream graphfile;	
		graphfile.open(graphtype.c_str());
		
		//read # of verticies
		graphfile >> vertex;
		this->vertice = vertex;
		
		//create dynamic 2d array
		table = new int*[vertice];
		for (int i = 0; i < vertice; i++)
		{
			table[i] = new int[vertice];
		}
		
		//set values to table[][] from file
		for (int i = 0; i < vertice; i++)
		{
			for (int j = 0; j < vertice; j++)
			{
				graphfile >> count;
				table[i][j] = count;					
			}
		} 
		//close file
		graphfile.close();
	}
	
	//sequential graph generation
	if (graphtype == "sequential")
	{
		this->type = "sequential"; //set type
		for (int i = 0; i < vertex; i++)
		{
			for (int j = i; j < vertex; j++)
			{
				if (i == j)
				{
					table[i][j] = 0; //if same vertex, vertex cost is always 0
					if (count == 0)  //should only append count on first vertex
					{
						count++;
					}
				}
				else
				{
					table[i][j] = count; //set forward edge
					table[j][i] = count; //set backwards edge
					count++;					
				}
			}
		}
	}
	
	//reverse graoh generation
	else if (graphtype == "reverse")
	{
		this->type = "reverse"; //set type
		
		//algorithm for finding the max cost. from count -> 1
		count = vertex*((vertex*.5)-.5); 
		
		for (int i = 0; i < vertex; i++)
		{
			for (int j = i; j < vertex; j++)
			{
				if (i == j)
				{
					table[i][j] = 0; //if same vertex, vertex cost is always 0
					if (count == 0)  //should only append count on first vertex
					{
						count--;
					}
				}
				else
				{
					table[i][j] = count; //set forward edge
					table[j][i] = count; //set backwards edge
					count--;					
				}
			}
		}
	}
	
	//random graph generation
	else if(graphtype == "random")
	{
		this->type = "random"; //set type
		count = rand(); //get a random number
		for (int i = 0; i < vertex; i++)
		{
			for (int j = i; j < vertex; j++)
			{
				if (i == j)
				{
					table[i][j] = 0; //if same vertex, vertex cost is always 0
					if (count == 0)  //should only append count on first vertex
					{
						count = rand(); //get a random # again
					}
				}
				else
				{
					table[i][j] = count; //set forward edge
					table[j][i] = count; //set backwards edge
					count = rand(); //get a random # again
				}
			}
		}
	}
	
	//all graphs need solution set, dynamically allocate it here
	solution = new int*[vertice];
	for (int i = 0; i < vertice; i++)
	{
		solution[i] = new int[vertice];
	}
	
}

//write graph to file.
//format is: vertex is on first line by itself
//each vertex cost seperated by a space
void Graph::writeGraph(string filename)
{
	//open file for writing
	ofstream graphfile;
	graphfile.open(filename.c_str());
	//write # of verticies
	graphfile << this->vertice << '\n';
	
	//write all edge costs to file
	for (int i = 0; i < vertice; i++)
	{
		for (int j = 0; j < vertice; j++)
		{
			graphfile << this->table[i][j] << ' ';					
		}
		graphfile << '\n'; //next line for next vertex
	} 
	//close file
	graphfile.close();
}

//write the Graph variable name, execution time (in CPU ticks / ticks per second), Graph type, algorithm used 
void Graph::writeTime(double time, string c, string alg)
{
	//open file for writing
    ofstream output;
    output.open("results.txt", ofstream::app); //append values
    
    //set precision for our time (decimals place)
    output.precision(15);
    output << "Graph: " << c << endl << "Time: " << time << endl << "Type: " << this->type << endl << "Algorithm type: " << alg << '\n';
    output << "# of verticies: " << vertice << '\n';
    output << "--------------------------------------------\n\n";
    
   	//close file
    output.close();
}

//write solution to file, writes Graph variable name, type of graph, and algorithm type, and the patht the algorithm computed
void Graph::writeSolution(string c, string alg)
{
	//open file for writing
    ofstream output;
    output.open("paths.txt", ofstream::app); //append values
    
    output <<  "Graph: " << c << endl << "Type: " << this->type << endl << "Algorithm type: " << alg << '\n';
    output << "# of verticies: " << vertice << '\n';
    
    //write solution cost on each vertex
    for (int i = 0; i < vertice; i++)
    {
   		output << "Vertex: " << i << "\n";
		for (int j = 0; j < vertice; j++)
   		{
   			output << "\t" << solution[i][j];
		}
		output << '\n';
    }
    output << "--------------------------------------------\n\n";
    
    //close file
    output.close();
}

//find min distance for Dijkstra's alg
int Graph::minDistance(int distance[], bool set[])
{
   // Initialize min value
   int min = INT_MAX;
   int min_index;
  
   for (int i = 0; i < vertice; i++)
   {
	   if (set[i] == false && distance[i] <= min)
	   {
			min = distance[i];
			min_index = i;
	   }
   } 
   //return minDistance index of value
   return min_index;
}

//Dijkstra's algorithm, takes in string for variable name of graph for writing into solution files
void Graph::dijkstra(string c)
{	
	string alg = "Dijkstra"; //for writing 
    int distance[vertice];
	bool set[vertice];
	
	//timer for execution time
	clock_t timer;
	double diff = 0.0;
	
	for (int i = 0; i < vertice; i++)
	{
		//set all values to max for comparison
		for (int x = 0; x < vertice; x++)
		{
			distance[x] = INT_MAX; 
			set[x] = false;
		}
		
		//set our starting vertex to 0, since the starting point is always 0
		distance[i] = 0;
		
		//BEGIN DIJKSTRA
		//start clock
		timer = clock();
		
		//this algorithm runs one vertex at a time, and we want to start once at all vertexes available. Runs n times for each vertex
		for (int count = 0; count < vertice-1; count++)
		{
			int min = minDistance(distance,set);
			set[min] = true;
			
			for (int j = 0; j < vertice; j++)
			{
				if (!set[j] && table[min][j] && distance[min] != INT_MAX && distance[min]+table[min][j] < distance[j])
				{
					distance[j] = distance[min] + table[min][j];
					
				}
			}
		}
		timer = clock() - timer;
		//END DIJKSTRA
		//stop clock
		
		//pull path from distance[] into the solution set
		for (int j = 0; j < vertice; j++)
		{
			solution[i][j] = distance[j];
		}
		
		//add up the time.
		diff += ((double)timer)/CLOCKS_PER_SEC;
	}
	//write time solution to file
	writeTime(diff, c, alg);
	//write path solution to file
	writeSolution(c, alg);
	cout << "Graph " << c << " with dijkstra's done!" << endl; 
}

//floyd warshall algorithm. takes in string for writing to file later
void Graph::floydWarshall(string c)
{
	string alg = "Floyd Warshall"; //for writing
	//timer for execution time
    clock_t timer1;
	double diff = 0.0;
	
  	//grab table and put in into working solution set
    for (int i = 0; i < vertice; i++)
    {
    	for (int j = 0; j < vertice; j++)
    	{
    		solution[i][j] = table[i][j];
		}
	}
	
    //BEGIN floyd's
    //start timer
	timer1 = clock();
    for (int k = 0; k < vertice; k++)
    {
        for (int i = 0; i < vertice; i++)
        {
            for (int j = 0; j < vertice; j++)
            {
                if (solution[i][k] + solution[k][j] < solution[i][j])
                {
                	solution[i][j] = solution[i][k] + solution[k][j];
				}
            }
        }
    }
 	timer1 = clock() - timer1;
 	//Stop timer, and grab the difference
 	//END FLOYD
	diff += ((double)timer1)/CLOCKS_PER_SEC;
	
	//write time solution to file
    writeTime(diff, c, alg);
    //write time solution to file
    writeSolution(c, alg);
    
    cout << "Graph " << c << " with floyd warshall done!" << endl; 
}

//where everything is run from. 
int main(int argc, const char * argv[])
{
	char c; //for graph variable name
	string input; //string convertion for argv[]
	srand(time(NULL)); //seed random number generator
	
	cout << "Welcome to Sara Warren's Dijkstra's and Floyd Warshall algorithm test!" << endl;

	if (argc > 1) //check if there's a file we should try and read from
	{
		input = argv[1];
		cout << "Reading from file..." << endl;
		Graph file(0, input); //construct graph
		cout << "Running Dijkstra's algorithm..." << endl;
		file.dijkstra(input); //run Dijkstra
		cout << "Running Floyd Warshall's algorithm..." << endl;
		file.floydWarshall(input); //run Floyd
		cout << "Done!" << endl << "All shortest paths calculated were written to 'paths.txt'" << endl << "runtimes for each algorithm is stored in 'results.txt'." << endl;
		return 0; //end
		
	}
	else //generate graphs
	{
		cout << "Creating Graphs!..." << endl << endl;
		
		//Construct graphs!
		Graph a(1000, "sequential");
		Graph b(1000, "reverse");
		Graph c(1000, "random");
		
		Graph a4(900, "sequential");
		Graph a5(900, "reverse");
		Graph a6(900, "random");
		
		Graph a1(800, "sequential");
	 	Graph a2(800, "reverse");
	 	Graph a3(800, "random");
		
		Graph d(750, "sequential");
		Graph e(750, "reverse");
		Graph f(750, "random");
	
		Graph g(500, "sequential");
		Graph h(500, "reverse");
		Graph i(500, "random");
	
		Graph j(250, "sequential");
		Graph k(250, "reverse");
		Graph l(250, "random");
	
		Graph m(150, "sequential");
		Graph n(150, "reverse");
		Graph o(150, "random");
		
		Graph p(125, "sequential");
		Graph q(125, "reverse");
		Graph r(125, "random");
		
		Graph s(100, "sequential");
		Graph t(100, "reverse");
		Graph u(100, "random");
		
		Graph v(75, "sequential");
		Graph w(75, "reverse");
		Graph x(75, "random");
		
		Graph y(50, "sequential");
		Graph z(50, "reverse");
		Graph aa(50, "random");

//		Graph bb(5000, "sequential");
//		Graph cc(5000, "reverse");
//		Graph dd(5000, "random");
	
		//write graphs to file if needed
		/*
		a.writeGraph("table_1.txt");
		b.writeGraph("table_2.txt");
		c.writeGraph("table_3.txt");
		d.writeGraph("table_4.txt");
		e.writeGraph("table_5.txt");
		f.writeGraph("table_6.txt");
		g.writeGraph("table_7.txt");
		h.writeGraph("table_8.txt");
		i.writeGraph("table_9.txt");
		j.writeGraph("table_10.txt");
		k.writeGraph("table_11.txt");
		l.writeGraph("table_12.txt");
		m.writeGraph("table_13.txt");
		n.writeGraph("table_14.txt");
		o.writeGraph("table_15.txt");
		p.writeGraph("table_16.txt");
		q.writeGraph("table_17.txt");
		r.writeGraph("table_18.txt");
		s.writeGraph("table_19.txt");	
		t.writeGraph("table_20.txt");
		u.writeGraph("table_21.txt");
		v.writeGraph("table_22.txt");
		w.writeGraph("table_23.txt");
		x.writeGraph("table_24.txt");
		y.writeGraph("table_25.txt");
		z.writeGraph("table_26.txt");
		aa.writeGraph("table_27.txt");
		a1.writeGraph("table_a1.txt");
        a2.writeGraph("table_a2.txt");
        a3.writeGraph("table_a3.txt");
        a4.writeGraph("table_a4.txt");
        a5.writeGraph("table_a5.txt");
        a6.writeGraph("table_a6.txt");
        
	//      bb.writeGraph("test28.txt");
	//	cc.writeGraph("test29.txt");
	//	dd.writeGraph("test30.txt");
		*/
		
		//Run Algorithms on each!
		a.dijkstra("a");
		a.floydWarshall("a");
		
		b.dijkstra("b");
		b.floydWarshall("b");
		
		c.dijkstra("c");
		c.floydWarshall("c");
		
		a4.dijkstra("a4");
		a4.floydWarshall("a4");
			
		a5.dijkstra("a5");
		a5.floydWarshall("a5");
		
		a6.dijkstra("a6");
		a6.floydWarshall("a6");
		
		a1.dijkstra("a1");
		a1.floydWarshall("a1");
		
		a2.dijkstra("a2");
		a2.floydWarshall("a2");
		
		a3.dijkstra("a3");
		a3.floydWarshall("a3");
		
		d.dijkstra("d");
		d.floydWarshall("d");
		
		e.dijkstra("e");
		e.floydWarshall("e");
		
		f.dijkstra("f");
		f.floydWarshall("f");
		
		g.dijkstra("g");
		g.floydWarshall("g");
		
		h.dijkstra("h");
		h.floydWarshall("h");
		
		i.dijkstra("i");
		i.floydWarshall("i");
		
		j.dijkstra("j");
		j.floydWarshall("j");
		
		k.dijkstra("k");
		k.floydWarshall("k");
		
		l.dijkstra("l");
		l.floydWarshall("l");
		
		m.dijkstra("m");
		m.floydWarshall("m");
		
		n.dijkstra("n");
		n.floydWarshall("n");
		
		o.dijkstra("o");
		o.floydWarshall("o");
		
		p.dijkstra("p");
		p.floydWarshall("p");
		
		q.dijkstra("q");
		q.floydWarshall("q");
		
		r.dijkstra("r");
		r.floydWarshall("r");
		
		s.dijkstra("s");
		s.floydWarshall("s");
		
		t.dijkstra("t");
		t.floydWarshall("t");
		
		u.dijkstra("u");
		u.floydWarshall("u");
		
		v.dijkstra("v");
		v.floydWarshall("v");
		
		w.dijkstra("w");
		w.floydWarshall("w");
		
		x.dijkstra("x");
		x.floydWarshall("x");
		
		y.dijkstra("y");
		y.floydWarshall("y");
		
		z.dijkstra("z");
		z.floydWarshall("z");
		
		aa.dijkstra("aa");
		aa.floydWarshall("aa");

	//5000 verticies, not needed and takes a very long time to run
		
//		bb.dijkstra("bb");
//		bb.floydWarshall("bb");
	
//		cc.dijkstra("cc");
//		cc.floydWarshall("cc");
	
//		dd.dijkstra("dd");
//		dd.floydWarshall("dd");

		cout << "Done!" << endl << "All shortest paths calculated were written to 'paths.txt'" << endl << "runtimes for each algorithm is stored in 'results.txt'." << endl;
		return 0;	
	}

}

