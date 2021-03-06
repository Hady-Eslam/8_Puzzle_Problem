#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<ctime>
using namespace std;

typedef vector <vector<int>> v2;
v2 Goal( 3, vector<int>(3) );
typedef pair<int,int>Pair;
map< v2, bool > Visited;
vector< v2 > Path;
typedef pair< int, Pair > PPair;

int Max;
void TracePath();
void Show( v2 );


bool isSolvable(int a[]){
	
	int count = 0;
	for (int i=0;i<8;i++)
		for (int j=i+1;j<8;j++)
			if ( a[i]>a[j] )
				count++;
	return count%2 == 0;
}


bool isGoal( v2 puzzle ){

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			if ( puzzle[i][j] != Goal[i][j] )
				return false;
	return true;
}


bool isValid( int i, int j, v2 puzzle, Pair Zero ){
	
	if ( i>-1 && i<3 && j>-1 && j<3 ){
		puzzle[Zero.first][Zero.second] = puzzle[i][j];
		puzzle[i][j] = 0;
		if ( !Visited[puzzle] )
			return true;
	}
	return false;
}


int hamming( int i, int j, v2 puzzle, Pair Zero ){

	puzzle[Zero.first][Zero.second] = puzzle[i][j];
	puzzle[i][j] = 0;

	int Steps = 0;
	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++)
			if ( puzzle[i][j] != Goal[i][j] )
				Steps++;
	return Steps;
}


Pair FindZero( v2 puzzle ){

	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++)
			if ( puzzle[i][j] == 0 )
				return make_pair(i,j);
	
	return make_pair(0,0);
}


v2 ChangePuzzle( v2 puzzle, Pair Node, Pair Zero ){

	puzzle[Zero.first][Zero.second] = puzzle[Node.first][Node.second];
	puzzle[Node.first][Node.second] = 0;
	return puzzle;
}


vector< PPair > RemoveUslessSolutions( vector<PPair> solutions ){
	
	for (int i=0; i<solutions.size(); i++)
		if ( solutions[i].first != solutions[0].first )
			solutions.erase( solutions.begin()+i );

	return solutions;
}


int manhatin ( v2 puzzle, int i, int j, Pair Zero ){
	
	int i1 = ( puzzle[i][j]-1 )/3;
	int j1 = ( puzzle[i][j]-1 )%3;
	return max( i1-Zero.first, Zero.first-i1 ) + max( j1-Zero.second, Zero.second-j1 );
}


Pair improveDecision( v2 puzzle, Pair Zero, vector<PPair>solutions ){

	solutions = RemoveUslessSolutions( solutions );
	for ( int i=0; i<solutions.size(); i++ )
		solutions[i].first = manhatin( puzzle, solutions[i].second.first, solutions[i].second.second, Zero );

	sort (solutions.begin(), solutions.end() );
	return make_pair( solutions[0].second.first, solutions[0].second.second );
}


bool Solve( v2 puzzle ){

	Path.push_back( puzzle );
	int Steps = 1;
	Max = 1;
	while ( !isGoal( puzzle ) ){

		vector< PPair >solutions;
		Visited[ puzzle ] = true;
		Pair Zero = FindZero( puzzle ),Node;

		if ( isValid( Zero.first-1, Zero.second, puzzle, Zero ) )
			solutions.push_back( make_pair( hamming( Zero.first-1, Zero.second, puzzle, Zero ), make_pair( Zero.first-1, Zero.second ) ) );

		if ( isValid( Zero.first+1, Zero.second, puzzle, Zero ) )
			solutions.push_back( make_pair( hamming( Zero.first+1, Zero.second, puzzle, Zero ), make_pair( Zero.first+1, Zero.second ) ) );
		
		if ( isValid( Zero.first, Zero.second-1, puzzle, Zero ) )
			solutions.push_back( make_pair( hamming( Zero.first, Zero.second-1, puzzle, Zero ), make_pair( Zero.first, Zero.second-1 ) ) );
		
		if ( isValid( Zero.first, Zero.second+1, puzzle, Zero ) )
			solutions.push_back( make_pair( hamming( Zero.first, Zero.second+1, puzzle, Zero ), make_pair( Zero.first, Zero.second+1 ) ) );
		
		sort ( solutions.begin(), solutions.end() );
		
		if ( solutions.size() == 0 && Path.size() != 0 ){
			Path.pop_back(),puzzle = Path.back();
			Steps--;
		}
		else if ( solutions.size() == 0 && Path.size() == 0 )
			return false;
		else{
			
			if ( solutions.size() == 1 || solutions.size() > 1 && solutions[0].first != solutions[1].first )
				Node = solutions[0].second;
			else
				Node = improveDecision( puzzle, Zero, solutions );
			
			puzzle = ChangePuzzle ( puzzle, Node, Zero );
			Path.push_back( puzzle );
			Steps++;
			if ( Steps > Max )
				Max = Steps;
		}
	}
	return true;
}


int main(){

	v2 puzzle( 3,vector<int>(3) );
	int arr[8],count=0;

	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++){
			cin>>puzzle[i][j];
			if ( puzzle[i][j] != 0 )
				arr[count] = puzzle[i][j],count++;
		}

	Goal[0][0] = 1;
	Goal[0][1] = 2;
	Goal[0][2] = 3;
	Goal[1][0] = 4;
	Goal[1][1] = 5;
	Goal[1][2] = 6;
	Goal[2][0] = 7;
	Goal[2][1] = 8;
	Goal[2][2] = 0;
	long Start = clock();
	if ( isSolvable( arr ) ){
		if ( Solve( puzzle ) ){
			long End = clock();
			cout<<"Time in MilliSecond = "<<(End-Start)<<"   Time in Seconds = "<<(End-Start)/1000<<endl;
			cout<<endl<<"Max = "<<Max<<"    Size = "<<Path.size()<<endl;
			cout<<"Enter Number : "<<endl;
			int x;
			cin>>x;
			TracePath();
		}
		else
			cout<<"Can't Find Solution"<<endl;
	}
	else
		cout<<"This puzzle is unsolvable"<<endl;
	
	return 0;
}


void Show( v2 puzzle ){

	cout<<endl<<endl<<"The Next Step"<<endl;
	for (int i=0;i<3;i++,cout<<endl)
		for (int j = 0;j<3;j++)
			cout<<puzzle[i][j]<<" ";
	cout<<endl;
}


void TracePath(){

	cout<<endl<<"The Path Of The Puzzle is "<<endl<<endl;
	for (int i=0;i<Path.size();i++)
		Show( Path[i] );
}
