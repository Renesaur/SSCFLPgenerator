#include "generator.h"

/*************************************************************************************************************/
generator::generator():
    n(0),
    m(0),
    TD(0){
    try{

    }catch(std::exception &e){
        std::cerr << "Exception in constructor of the data class : " << e.what() << std::endl;
        exit(1);
    }
}


/*************************************************************************************************************/
void generator::generateStidsenAndersenDammannInstance ( int nn, int mm, double BoxLB, double BoxUB, int dLB, int dUB, int sLB, int sUB, int seed )
{
    try
    {
        double  TotalDemand   = 0.0, // The total demand of the customers
                TotalCapacity = 0.0, // The total capacity of the facilities
                TheRatio      = 0.0, // The TotalCapacity / TotalDemand ratio we should end up  with
                Scale         = 0.0; // The scaling factor needed to reach TheRatio ( s[i] = s[i]*Scale )
        n = nn;
        m = mm;
        std::vector<std::pair<double,double>> FacPos ( n ); // Position of each facility
        std::vector<std::pair<double,double>> CustPos ( m );// Position of each customer
        std::vector<int> MinC ( n );                        // The minimum distance to a customer for each facility
        std::pair<double,double> p;                         // Pair used to hold the position of each facility/customer

        /*
         * Set up the distributions
         */
        std::mt19937_64::result_type TheSeed = seed;
        std::mt19937_64 G ( TheSeed );
        UniReal Position    = UniReal ( BoxLB , BoxUB );
        UniReal Ratio       = UniReal ( 1.5, 4.0 );
        UniInt  Demand      = UniInt ( dLB , dUB );
        UniInt  Capacity    = UniInt ( sLB , sUB );

        /*
         * Generate the coordinates for the facilities
         */
        for ( int i=0; i<n; ++i )
        {
            p.first = Position( G );
            p.second = Position ( G );
            FacPos[i] = p;
            MinC[i] = INT_MAX;
        }

        /*
         * Generate the coordinates for the customers
         */
        for ( int j=0; j<m; ++j )
        {
            p.first = Position( G );
            p.second = Position ( G );
            CustPos[j] = p;
        }

        /*
         * Allocate memmory for the data
         */
        c = std::vector< std::vector< int > > ( n );
        f = std::vector< int > ( n );
        s = std::vector< int > ( n );
        d = std::vector< int > ( m );

        /*
         * Generate the assignment costs, capacities and fixed opening costs
         */
        for ( int i=0; i<n; ++i )
        {
            c[i] = std::vector< int > ( m );
            for ( int j=0; j<m; ++j )
            {
                c[i][j] = Euclid( FacPos[i] , CustPos[j] );
                if ( c[i][j] < MinC[i] ) MinC[i] = c[i][j];
            }
            f[i] = ( MinC[i] + 1 ) * 10;
            s[i] = Capacity ( G );
            TotalCapacity += double ( s[i] );
        }

        /*
         * Generate the demands
         */
        for ( int j = 0; j<m; ++j )
        {
            d[j] = Demand ( G );
            TD += d[j];
            TotalDemand += double ( d[j] );
        }

        /*
         * Generate the ratio sum_i s_i/sum_j d_j
         */
        TheRatio = Ratio ( G );

        /*
         * Scale the capacities such that sum_i s_i/sum_j d_j = Ratio
         */
        Scale = TheRatio * ( TotalDemand / TotalCapacity );
        for( int i=0; i<n; ++i )
        {
            s[i] = int ( Scale * s[i] + 0.5 );
        }
    }catch(std::exception &e)
    {
        std::cerr << "Exception in the generateStidsenAndersenDammannInstance of the data class : " << e.what ( ) << std::endl;
        exit ( EXIT_FAILURE );
    }
}

/*************************************************************************************************************/
void generator::generateCornuejolsInstance ( int nn, int mm, int dLB, int dUB, int sLB, int sUB, double Ratio, int seed )
{
    try
    {
        // Set the number of facilities and the number of customers
        n = nn;
        m = mm;

        double  TotalDemand   = 0.0, // The total demand of the customers
                TotalCapacity = 0.0, // The total capacity of the facilities
                TheRatio      = Ratio, // The TotalCapacity / TotalDemand ratio we should end up  with
                Scale         = 0.0; // The scaling factor needed to reach TheRatio ( s[i] = s[i]*Scale )

        std::vector<std::pair<double,double>> FacPos ( n ); // Position of each facility
        std::vector<std::pair<double,double>> CustPos ( m );// Position of each customer
        std::vector<int> MinC ( n );                        // The minimum distance to a customer for each facility
        std::pair<double,double> p;                         // Pair used to hold the position of each facility/customer

        /*
         * Set up the distributions
         */
        std::mt19937_64::result_type TheSeed = seed;
        std::mt19937_64 G ( TheSeed );
        UniReal Position       = UniReal ( 0.0 , 1.0 );
        UniInt  Demand         = UniInt ( dLB , dUB );
        UniInt  Capacity       = UniInt ( sLB , sUB );
        UniReal  FirstOfFixed  = UniReal ( 0.0 , 90.0 );
        UniReal  SecondOfFixed = UniReal ( 100.0 , 110.0 );

        /*
         * Generate the coordinates for the facilities
         */
        for ( int i=0; i<n; ++i )
        {
            p.first = Position( G );
            p.second = Position ( G );
            FacPos[i] = p;
            MinC[i] = INT_MAX;
        }

        /*
         * Generate the coordinates for the customers
         */
        for ( int j=0; j<m; ++j )
        {
            p.first = Position( G );
            p.second = Position ( G );
            CustPos[j] = p;
        }

        /*
         * Allocate memmory for the data
         */
        c = std::vector< std::vector< int > > ( n );
        f = std::vector< int > ( n );
        s = std::vector< int > ( n );
        d = std::vector< int > ( m );

        /*
         * Generate the assignment costs and capacities
         */
        for ( int i=0; i<n; ++i )
        {
            c[i] = std::vector< int > ( m );
            for ( int j=0; j<m; ++j )
            {
                c[i][j] = Euclid( FacPos[i] , CustPos[j] );
                if ( c[i][j] < MinC[i] ) MinC[i] = c[i][j];
            }
            s[i] = Capacity ( G );
            TotalCapacity += double ( s[i] );
        }

        /*
         * Generate the fixed opening costs
         */
        for ( int i = 0; i < n; ++i )
        {
            f[i] = int ( FirstOfFixed( G ) + SecondOfFixed ( G ) * std::sqrt( s[i] ) );
        }

        /*
         * Generate the demands
         */
        for ( int j = 0; j<m; ++j )
        {
            d[j] = Demand ( G );
            TD += d[j];
            TotalDemand += double ( d[j] );
        }

        /*
         * Scale the capacities such that sum_i s_i/sum_j d_j = Ratio
         */
        Scale = TheRatio * ( TotalDemand / TotalCapacity );
        for( int i=0; i<n; ++i )
        {
            s[i] = int ( Scale * s[i] + 0.5 );
        }

    }
    catch(std::exception &e)
    {
        std::cerr << "Exception in the generateCornuejolsInstance of the data class : " << e.what ( ) << std::endl;
        exit ( EXIT_FAILURE );
    }
}

/*************************************************************************************************************/
void generator::printToFile ( const std::string& FileName )
{
    try
    {
        std::ofstream file ( FileName );

        if ( ! file ) throw std::runtime_error ( "Could not open the file!" );

        /*
         * Print number of facilities and number of customers to file!
         */
        file << n << "\t" << m << "\n\n";

        /*
         * Print capacities and fixed opening costs to file!
         */
        for ( int i=0; i<n; ++i )
        {
            file << s[i] << "\t" << f[i] << "\n";
        }
        file << "\n";

        /*
         * Print demands to file
         */
        for ( int j=0; j<m; ++j ) file << d[j] << " ";
        file << "\n";
        file << "\n";

        /*
         * Print the assignment costs to file
         */
        for ( int i=0; i<n; ++i )
        {
            for ( int j=0; j<m; ++j )
            {
                file << c[i][j] << " ";
            }
            file << "\n";
        }
    }
    catch ( std::runtime_error &re)
    {
        std::cerr << "Runtime error in printToFile in the generator class : " << re.what ( ) << std::endl;
    }
    catch ( std::exception &e )
    {
        std::cerr << "Exception in printToFile in the generator class : " << e.what ( ) << std::endl;
    }
}

/*************************************************************************************************************/
void generator::convertToUFLP ( )
{
    try{
        std::vector<int> Cmin ( n );
        for ( int i=0; i<n; ++i )
        {
            Cmin [ i ] = INT_MAX;
            for ( int j=0; j<m; ++j )
            {
                c[i][i] = d[j] * c[i][j];
                if ( c[i][j] < Cmin[i] ) Cmin[i] = c[i][j];
            }
            f[i] = Cmin[i] * 10;
            s[i] = m;
        }
        for ( int j=0; j<m; ++j ) d[j] = 1;
    }
    catch ( std::exception & e )
    {
        std::cerr << "Exception in convertToUFLP in data class : " << e.what ( ) << std::endl;
        exit ( EXIT_FAILURE );
    }
}

/*************************************************************************************************************/
int generator::Euclid(std::pair<double,double> p1, std::pair<double,double> p2){
    double x = p1.first  - p2.first;
    double y = p1.second - p2.second;
    double dist = x*x + y*y;
    dist = std::sqrt ( dist );

    return int ( 10 * dist + 0.5 ); // Add 0.5 and cast to int = round to nearest
}

/*************************************************************************************************************/
void generator::clearGenerator ( )
{
    n  = 0;
    m  = 0;
    TD = 0;
    for ( auto it = c.begin(); it != c.end (); ++it )
    {
        it->clear ();
    }
    c.clear ( );
    f.clear ( );
    d.clear ( );
    s.clear ( );
}









