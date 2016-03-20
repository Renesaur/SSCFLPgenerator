#include <iostream>
#include"generator.h"

using namespace std;

int main()
{
    try
    {
        // Initialize a pointer to a generator
        generator* G = new generator ( );

        /*
         * Generate a Stidsen, Andersen, Dammann instance
         * Number of facilities = 5
         * Number of customers = 10
         * Lower bound on coordinate box = 0.0
         * Upper bound on coordinate box = 10.0
         * Lower bound on demands = 5
         * Upper bound on demands = 10
         * Lower bound on capacities = 10
         * Upper bound on capacities = 20
         * seed = 1
         */
        G -> generateStidsenAndersenDammannInstance( 5 , 10 , 0.0 , 10.0 , 5 , 10 , 10 , 20 , 1 );

        /*
         * Print the instance to the file instance.txt
         */
        G -> printToFile( "StidsenAndersenDammannInstance.txt" );

        /*
         * Clear the generator so that we can generate a new instance
         */
        G -> clearGenerator ( );

        /*
         * Generate a Cornuejols instance
         * Number of facilities = 5
         * Number of customers = 10
         * Lower bound on demands = 5
         * Upper bound on demands = 35
         * Lower bound on capacities = 10
         * Upper bound on capacities = 160
         * Ratio between total capacity and total demand = 3
         * Seed = 1
         */
         G -> generateCornuejolsInstance( 5 , 10 , 5 , 35 , 10 , 160 , 3 , 1 );

        /*
         * Print the Cornuejols instance to file
         */
        G -> printToFile( "CornuejolsInstance.txt" );

        delete G;
        return 0;
    }
    catch ( ... )
    {
        std::cerr << "Something went wrong!\n";
        return 1;
    }
}
