#define _USE_MATH_DEFINES // for M_PI

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <stack>
#include <queue>
#include <sstream>
#include <bitset>
#include <iomanip>
#include <unordered_map>
#include <math.h>
#include <array>
#include "ConsoleColor.h"

using namespace std;

#define N_ATTRIBUTES 6
#define N_RESTAURANTS 19
#define EARTH_RADIUS 3959 // miles

ostream& (*cstream[])(ostream&) =
{
	blue, green, red, yellow, white
};

///    printVector
// Template which prints contents of a vector of any type.
template <typename T>
void printVector(const T& inputVector)
{
	for (auto i = inputVector.begin(); i != inputVector.end(); ++i)
		cout << *i << " ";
	cout << "\n";
}

class InputFile
{
private:
	string filename;
public:
	ifstream inFile;
	InputFile(string filename);
	~InputFile();
	ifstream & getIfstream();
};

InputFile::InputFile(string filename)
{
	cout << "Opening input file: " << filename << "\n\n";
	inFile.open(filename);
}

ifstream & InputFile::getIfstream()
{
	return inFile;
}

InputFile::~InputFile()
{
	cout << "\nClosing " << filename << "\n";
	inFile.close();
}

class XMLParser
{
private:
	ifstream &inFileRef;
	string line;
	string data;
	vector<vector<string>> xmlDataList;
	vector<string> dataSet;
	string masterNode;
	stack<string> tagStack;
public:
	XMLParser(ifstream &);
	void setLine();
	string closeTag(string line);
	void parseFile();
	void printData();
	vector<vector<string>> getXMLDataList();
	string trim(string text, string whitespace);
	void checkPhoneNumber();
};

XMLParser::XMLParser(ifstream &inFile) : inFileRef(inFile)
{
	cout << "Parsing XML file...\n";
	setLine();
	line.clear(); // delete first line
	setLine();
	masterNode = line;
	tagStack.push(masterNode); // set master node
	parseFile();
}

void XMLParser::setLine()
{
	getline(inFileRef, line);
}

void XMLParser::parseFile()
{
	while (getline(inFileRef, line))
	{
		line = trim(line, "\040\t"); // \040 = space. Trims space from outside <> tags
		//cout << "[" << line << "]\n";
		regex begin_element_pattern("^<\/?\\w+>$"); // matches: one <tag> or </tag>
		bool is_single_tag = regex_search(line.begin(), line.end(), begin_element_pattern);
		if (is_single_tag)
		{
			if (line == closeTag(tagStack.top()) && line != closeTag(masterNode))
			{
				checkPhoneNumber();
				xmlDataList.push_back(dataSet);
				tagStack.pop();
				dataSet.clear();
			}
			else
				tagStack.push(line);
		}
		else
		{
			regex pattern("[<>]");
			sregex_token_iterator end;
			sregex_token_iterator temp(line.begin(), line.end(), pattern, -1);
			temp++;
			temp++;
			line = temp->str();
			line = trim(line, "\040\t"); // trims space from data ex: [ CityName]
			//cout << "Pushing back: [" << line << "]\n";
			dataSet.push_back(line);
		}
	}
}

string XMLParser::trim(string text, string space)
{
	auto front = text.find_first_not_of(space);
	auto back = text.find_last_not_of(space);
	auto length = back - front + 1;
	return text.substr(front, length);
}

string XMLParser::closeTag(string line)
{
	string sresult = "";
	regex frontPattern("^<\\w");
	regex replacePattern("^<");
	if (regex_search(line.begin(), line.end(), frontPattern))
	{
		sresult = regex_replace(line, replacePattern, "</");
	}
	return sresult;
}

bool regexMatch(string text, string expr);

void XMLParser::checkPhoneNumber() // fixes placement of phone # in one of the restaurants of input file.
{
	string text = dataSet[N_ATTRIBUTES - 1];
	bool found = regexMatch(text, "\\(\\d{3}\\)\\d{3}-\\d{4}");
	if (found)
	{
		dataSet.insert(dataSet.begin() + 3, text);
		dataSet.pop_back();
	}
}

bool regexMatch(string text, string expr)
{
	regex pattern(expr);
	bool bvalue = regex_match(text, pattern);
	return bvalue;
}

void XMLParser::printData()
{
	for (int i = 0; i < xmlDataList.size(); i++)
	{
		for (int j = 0; j < N_ATTRIBUTES; j++)
		{
			cout << xmlDataList[i][j] << "\t";
		}
		cout << "\n";
	}
}

vector<vector<string>> XMLParser::getXMLDataList()
{
	return xmlDataList;
}

class Restaurant
{
private:
	pair<double, double> longlat;
	string address;
	string city;
	string state;
	string phone;
public:
	Restaurant();
	Restaurant(vector<string> xmlDataSet);
	pair<double, double> getLonglat();
	friend ostream &operator<< (ostream& os, const Restaurant& r);
};

Restaurant::Restaurant()
{
	longlat.first = 0;
	longlat.second = 0;
	address = city = state = phone = "";
}

Restaurant::Restaurant(vector<string> xmlDataSet)
{
	//cout << "Building a new restaurant...\n";
	//for (int i = 0; i < N_ATTRIBUTES; i++)
	//	cout << "[" << xmlDataSet[i] << "]\t";
	//cout << "\n";
	address = xmlDataSet[0];
	city = xmlDataSet[1];
	state = xmlDataSet[2];
	phone = xmlDataSet[3];
	longlat.first = stod(xmlDataSet[4]); // change string to double
	longlat.second = stod(xmlDataSet[5]);
}

pair<double, double> Restaurant::getLonglat()
{
	return longlat;
}

ostream &operator<< (ostream& os, const Restaurant& r)
{
	os << white << setprecision(5) << r.address << endl;
	os << r.city << ", " << r.state << endl;
	os << r.phone << endl;
	os << r.longlat.first << " " << r.longlat.second << endl;
	return os;
}

class RestaurantGraph
{
private:
	vector<vector<int>> rawAdjList;
	unordered_map<int, vector<int>> adjacencyList;
public:
	RestaurantGraph();
	void buildRawAdjList();
	void buildAdjacencyList();
	void printAdjacencyList();
	unordered_map<int, vector<int>> getAdjacencyList();
};

RestaurantGraph::RestaurantGraph()
{
	buildRawAdjList();
	buildAdjacencyList();
}

void RestaurantGraph::buildRawAdjList()
{
	rawAdjList.resize(N_RESTAURANTS);
	rawAdjList[0] = { 3, 5, 7, 10, 12, 13, 14 };
	rawAdjList[1] = { 2, 7, 11 };
	rawAdjList[2] = { 1, 5, 6, 8, 13 };
	rawAdjList[3] = { 0, 10, 12, 14 };
	rawAdjList[4] = { 7, 11, 12 };
	rawAdjList[5] = { 0, 2, 6, 13, 14 };
	rawAdjList[6] = { 2, 8, 13 };
	rawAdjList[7] = { 0, 1, 3, 4, 11, 12 };
	rawAdjList[8] = { 2, 6, 9, 12, 13 };
	rawAdjList[9] = { 8, 13 };
	rawAdjList[10] = { 0, 3, 12, 13, 14 };
	rawAdjList[11] = { 1, 4, 7, 12, 13, 14 };
	rawAdjList[12] = { 0, 3, 7, 10, 14 };
	rawAdjList[13] = { 0, 2, 5, 6, 10, 14 };
	rawAdjList[14] = { 0, 3, 5, 10, 12, 13 };
	rawAdjList[15] = { 6, 8, 13, 16, 17, 18 };
	rawAdjList[16] = { 8, 9, 15, 17, 18 };
	rawAdjList[17] = { 8, 9, 15, 16, 18 };
	rawAdjList[18] = { 8, 9, 15, 16, 17 };
}

void RestaurantGraph::buildAdjacencyList()
{
	cout << "Building adjacency list...\n";
	for (int key = 0; key < N_RESTAURANTS; key++)
		adjacencyList.insert({key, rawAdjList[key]});
}

void RestaurantGraph::printAdjacencyList()
{
	for (int i = 0; i < N_RESTAURANTS; i++)
	{
		cout << setw(2) << i << ": ";
		printVector(adjacencyList[i]);
	}
}

unordered_map<int, vector<int>> RestaurantGraph::getAdjacencyList()
{
	return adjacencyList;
}

class DistanceCalculator
{
private:
	pair<double, double> longlatA; // first = latitude, second = longitude
	pair<double, double> longlatB;
	double radius;
	double distance;
	double radians(double degrees);
	void haversine();
public:
	DistanceCalculator();
	void setPoints(pair<double, double> pointA, pair<double, double> pointB);
	void calcDistance();
	double getDistance();
};

DistanceCalculator::DistanceCalculator()
{
	radius = EARTH_RADIUS;
	distance = 0;
	longlatA.first = longlatA.second = 0;
	longlatB.first = longlatB.second = 0;
}

void DistanceCalculator::setPoints(pair<double, double> pointA, pair<double, double> pointB)
{
	longlatA = pointA;
	longlatB = pointB;
}

void DistanceCalculator::calcDistance()
{
	haversine();
}

// haversine formula gives the distance between two points on a sphere, given the radius and their x,y coordinates
void DistanceCalculator::haversine()
{
	double difLat = radians(longlatB.first - longlatA.first);
	double difLong = radians(longlatB.second - longlatA.second);
	double nA =   pow(sin(difLat / 2.0), 2.0)  + cos(radians(longlatA.first)) 
				* cos(radians(longlatB.first)) * pow(sin(difLong / 2.0), 2.0);
	double nB = 2.0 * atan2(sqrt(nA), sqrt(1.0 - nA));
	distance = radius * nB;
}

double DistanceCalculator::radians(double degrees)
{
	return degrees * M_PI / 180.0;
}

double DistanceCalculator::getDistance()
{
	return distance;
}

class AdjacencyMatrix
{
private:
	RestaurantGraph *restaurantGraph;
	unordered_map<int, vector<int>> adjacencyList;
	vector<Restaurant> *restaurants;
	DistanceCalculator calculator;
	array<array<double, N_RESTAURANTS>, N_RESTAURANTS> distances;
	double totalDistance;
	vector<int> path;
	array<double, N_RESTAURANTS> pathDistances;
	bitset<N_RESTAURANTS> visited;
	double min;
	void clearPathVariables();
	int findClosest();
	bool pathExists(int a, int b);
	bool pathIsShorter(int a, int b);
	int start;
	int destination;
	vector<double> steps;
public:
	AdjacencyMatrix(RestaurantGraph *inputGraph, vector<Restaurant> *restaurants);
	void calculateDistances();
	void printMatrix();
	void calcPath(int start);
	void printPath();
};

AdjacencyMatrix::AdjacencyMatrix(RestaurantGraph *inputGraph, vector<Restaurant> *r)
{
	cout << "Building adjacency matrix...\n";
	restaurantGraph = inputGraph;
	adjacencyList = restaurantGraph->getAdjacencyList();
	restaurants = r;
	totalDistance = 0;
	calculateDistances();
}

void AdjacencyMatrix::calculateDistances()
{
	vector<int> travelList;
	for (int i = 0; i < N_RESTAURANTS; i++)
	{
		travelList = adjacencyList[i];
		int restA = i;
		for (int j = 0; j < travelList.size(); j++)
		{
			int restB = travelList[j];
			calculator.setPoints(restaurants->at(restA).getLonglat(), restaurants->at(restB).getLonglat());
			calculator.calcDistance();
			distances[restA][restB] = calculator.getDistance();
		}
	}
}

void AdjacencyMatrix::printMatrix()
{
	cout << "Printing Matrix:\n";
	int i = 0, j = 0;
	for (i = 0; i < N_RESTAURANTS; i++)
	{
		for (j = 0; j < N_RESTAURANTS; j++)
			cout << setprecision(3) << setw(6) <<  distances[i][j]; // temp formatting.
		cout << "\n";
	}
}

void AdjacencyMatrix::clearPathVariables()
{
	path.clear(); // delete previous path
	fill_n(pathDistances.begin(), N_RESTAURANTS, DBL_MAX); // fill distances list with high value
	totalDistance = 0;
	min = DBL_MAX;
visited.reset(); // set all bits to 0
destination = 0;
}
/// next: use Djikstra's Algorithm to calculate shortest path
/// starting with restaurant #start, go to the next minimum distance restaurant
/// **which hasn't been visited yet**.
/// at that restaurant, repeat. until no non-visited restaurant is in the adjacency list (row).
// make_pair for each non-zero item in distances array?
// 
void AdjacencyMatrix::calcPath(int s)
{
	cout << "Calculating shortest path...\n";
	start = s;
	int d = -1;
	clearPathVariables();
	pathDistances[start] = 0;
	for (int i = 0; i < N_RESTAURANTS; i++)
	{
		d = findClosest();
		if (d == -1)
			break;
		{
			path.push_back(d);
			visited.set(d);
			for (int j = 0; j < N_RESTAURANTS; j++)
			{
				if (!visited[j] && pathExists(d, j) && pathIsShorter(d, j))
				{
					pathDistances[j] = pathDistances[d] + distances[d][j];
					steps.push_back(distances[d][j]);
				}					
			}
		}
	}
	destination = path.back();
}

bool AdjacencyMatrix::pathExists(int a, int b)
{
	return distances[a][b];
}

bool AdjacencyMatrix::pathIsShorter(int a, int b)
{
	if (pathDistances[a] + distances[a][b] < pathDistances[b])
		return true;
	return false;
}


int AdjacencyMatrix::findClosest()
{
	min = DBL_MAX;
	int target = -1;
	for (int i = 0; i < N_RESTAURANTS; i++)
	{
		if (!visited[i] && pathDistances[i] < min)
		{
			min = pathDistances[i];
			target = i;
		}
	}
	return target;
}

void AdjacencyMatrix::printPath()
{
	cout << "\nThe shortest path to the most restaurants is the path traveled to restaurant ";
	cout << destination << ":\n";
	printVector(path);
	double total = 0;
	for (int i = 0; i < path.size(); i++)
	{
		cout << blue << "\nRestaurant " << path[i] << ":\n";
		cout << restaurants->at(path[i]);
		if (i < path.size() - 1)
			cout << "Traveling to restaurant " << path[i + 1] << ", " << steps[i] << " miles away.\n";
		cout << "Total distance traveled: " << total << "\n";
		if (i < path.size() - 1)
			total += steps[i];
	}
}

int main()
{
	InputFile file("Restaurants.XML");
	XMLParser parser(file.getIfstream());
	vector<vector<string>> output = parser.getXMLDataList();
	vector<Restaurant> restaurants;
	cout << "Building restaurant list...\n";
	for (int i = 0; i < output.size(); i++)
	{
		Restaurant temp(output[i]);
		restaurants.push_back(temp);
	}	

	RestaurantGraph graph;
	//graph.printAdjacencyList();

	AdjacencyMatrix matrix(&graph, &restaurants);
	matrix.calculateDistances();
	//matrix.printMatrix();
	matrix.calcPath(0);
	matrix.printPath();

	system("pause");
	return 0;
}