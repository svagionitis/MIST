#include <mist/minimization.h>
#include <iostream>


double f1( const mist::matrix< double > &v )
{
	double x = v[ 0 ];
	double y = v[ 1 ];
	return( 4.0 * ( x - 1.0 ) * ( x - 1.0 ) + ( y - 2.0 ) * ( y - 2.0 ) + 100 );
} // �S�i��1�|�P�j2�{�i��2�|�Q�j2 

double f2( double x )
{
	return( std::sin( x ) + x / 5.0 );
}

int main( int argc, char *argv[] )
{
	using namespace std;

	{
		mist::matrix< double > p( 2, 1 );
		//p[ 0 ] = 0.1;
		//p[ 1 ] = 0.2;
		double err = mist::gradient::minimization( p, f1, 0.0 );
		cout << "f( " << p.t( ) << " ) = " << err << endl;
	}

	{
		mist::matrix< double > p( 2, 1 ), d( 2, 2 );
		//p[ 0 ] = 0.1;
		//p[ 1 ] = 0.2;
		d( 0, 0 ) = 1;
		d( 1, 1 ) = 1;
		double err = mist::powell::minimization( p, d, f1, 0.0 );
		cout << "f( " << p.t( ) << " ) = " << err << endl;
	}

	{
		double x;
		double err = mist::gold::minimization( 0, 1, x, f2, 0.00001 );
		cout << "f( " << x << " ) = " << err << endl;
	}
	{
		double x;
		double err = mist::brent::minimization( 0, 1, x, f2, 0.00001 );
		cout << "f( " << x << " ) = " << err << endl;
	}

	return( 0 );
}
