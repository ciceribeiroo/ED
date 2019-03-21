#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;


int main()
{
	int v[10];
	for ( int i = 0; i < 10; i++ )
	{
		cin >> v[i];
	}
	bool valido[9];
	for ( int i = 0; i < 9; i++ )
	{
		valido[i] = true;
	}
	for ( int i = 0; i < 10; i++ )
	{
		if ( v[i] <= 0 )
		{
			valido[i] = false;
		}
	}
	for ( int i = 0; i < 10; i++ )
	{
		if ( valido[i] == false )
		{
			v[i] = v[i+1];
		}
	}
	for ( int i = 0; i < 10; i++ )
	{
		if ( valido[i] == true )
		{
			cout << v[i] << " ";
		}
	}
return 0;
}
