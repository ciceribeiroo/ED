#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;


int main()
{
	int dado[20];
	for ( int i = 0; i < 20; i++ )
	{
		cin >> dado[i];
	}
	int res[6] = {0};
	for ( int i = 0; i < 20; i++ )
	{
		if ( dado[i] == 1)
		{
			res[0]++;
		}
		else if ( dado[i] == 2 )
		{
			res[1]++;
		}
		else if ( dado[i] == 3 )
		{
			res[2]++;
		}
		else if ( dado[i] == 4 )
		{
			res[3]++;
		}
		else if ( dado[i] == 5 )
		{
			res[4]++;
		}
		else if ( dado[i] == 6 )
		{
			res[5]++;
		}
	}
	float f[6] = {0};
	for ( int i = 0; i < 6; i++ )
	{
			f[i] = (float)res[i] / 20;
	}
	for ( int i = 0; i < 6; i++ )
	{
			cout << i << ": " << f[i] << endl;
	}
return 0;
}
