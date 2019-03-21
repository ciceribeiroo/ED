#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;


int main()
{
	int v[9];
	for ( int i = 0; i < 9; i++ )
	{
		cin >> v[i];
	}
	bool primos[9];
	for ( int i = 0; i < 9; i++ )
	{
		primos[i] = true;
	}
	for ( int i = 0; i < 9; i++ )
	{
		for ( int j = 2; j < v[i]; j++ )
		{
			if ( v[i] != j)
			{
				if ( v[i] % j == 0 )
				{
					primos[i] = false;
				}
			}
		}
	}
	for ( int i = 0; i < 9; i++ )
	{
		if ( primos[i] == true )
		{
			cout << v[i] << " " << i << endl;
		}
	}
return 0;
}
