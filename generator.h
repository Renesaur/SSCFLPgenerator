#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include<vector>
#include<stdexcept>
#include<iostream>
#include<fstream>
#include<sstream>
#include<limits.h>
#include<random>
#include<chrono>

typedef std::uniform_int_distribution<> UniInt; //! Uniform distribution on integers
typedef std::uniform_real_distribution<> UniReal;
typedef std::mt19937_64 G; //! Random number generator based on the 64-bit Mersenne Twister by Matsumoto and Nishimura, 2000
typedef std::chrono::high_resolution_clock myclock;

class generator{
    private:
        /**
         * @name Data
         * This section contains all the data for describing the SSCFLP.
         */
        ///@{
            int n; //! Number of facility sites
            int m; //! Number of customers
            int TD; //! Total demand. TD=sum_{j=1}^n d[j]
            std::vector<std::vector<int>> c; //! Assignment cost. c[i][j] is the cost of assigning customer j to facility i
            std::vector<int> f; //! Fixed opening cost. f[i] is the cost of opening facility i
            std::vector<int> d; //! Demand. d[j] is the demand at customer j
            std::vector<int> s; //! Capacity. s[i] is the capacity of facility i
        ///@}

    public:

        /*! \brief Constructor of the data class
         *
         * Constructor of the generator class.
         */
        generator();

        /*! \brief Generates an instance of the SSCFLP according to Stidsen, Andersen, and Dammann
         *
         * Function generating an instance of the SSCFLP according to the method specified in Stidsen, Andersen, Dammann Management Science (2014),
         * "A Branch and Bound Algorithm for a Class of Biobjective Mixed Integer Programs".
         * The method needs the number of facilities and the number of customers in the instance.
         * For each customer and facility a coordinate in the Euclidean plane is generate in a box [BoxLB , BoxUB]x[BoxLB , BoxUB].
         * Then the Euclidean distance between each ( facility, customer)-pair is calculated and the cost of assigning customer j to facility i
         * is set to c[i][j] = round ( dist(i,j) * 10 ). The fixed costs are set to f[i] = ( min_j { c[i][j]+1 } )  * 10.
         * Next, the demand of each customer is generated from a uniform distribution on {dLB,...,dUB}. Like wise each facility capacity is generated
         * from {sLB,...,sUB}. Finally a number, r, between 1.5 and 4 is drawn and the capacities are scaled such that \f$\sum_i s[i] / \sum_j d[j] \approx r\f$
         * \param n integer. Number of facilities.
         * \param m integer. Number of customers.
         * \param BoxLB double. Lower bound on the square box from which coordinates are drawn.
         * \param BoxUB double. Upper bound on the square box from which coordinates are drawn.
         * \param dLB integer. Lower bound on the demands.
         * \param dUB integer. Upper bound on the demands.
         * \param sLB integer. Lower bound on the capacities.
         * \param sUB integer. Upper bound on the capacities.
         * \param seed integer. The seed used to for the random number generator.
         */
        void generateStidsenAndersenDammannInstance ( int n, int m, double BoxLB, double BoxUB, int dLB, int dUB, int sLB, int sUB, int seed );

        /*! \brief Generates instance of SSCFLP according to Cornuejols
         *
         * Similar to generateStidsenAndersenDammannInstance. Generates coordinates for facilities and customers in unit square. Multipliers distances by
         * 10 and round an use that as unit assignment costs. Capacities and demands are drawn from U(sLB,sUB) and U(dLB,dUB), respectively. Here U(a,b)
         * is a uniform distribution on [a,b]. The capacities are scaled such that total capacity/total demand = Ratio. Finally, the fixed costs are generated
         * as f[i] = U[0,90] + U[100,110]*\sqrt ( s[i] ) to reflect economics of scale.
         */
        void generateCornuejolsInstance ( int n, int m, int dLB, int dUB, int sLB, int sUB, double Ratio, int seed );

        /*!
         * Converts the data to an uncapacitated facility location problem by setting c[i][j] = d[j] * c[i][j], s[i] = m, and d[j] = 1.
         */
        void convertToUFLP ();

        /*!
         * Function printing the instance to a file. The format is as follows
         * n m
         * s[1]   f[1]
         * s[2]   f[2]
         * ...
         * s[n]   f[n]
         * d[1] d[2] ... d[m]
         * c[1][1] ... c[n][1]
         * c[2][1] ... c[n][2]
         * ...
         * c[n][1] ... c[n][m]
         * \param FileName constant reference to string. Contains the name for the file the instance is printed to.
         */
        void printToFile ( const std::string& FileName );


        /*!
         * Function returning a scaled Euclidean distance between two points (rounded to nearest integer).
         * The distance returned is round ( EuclideanDistance ( p1, p2 ) * 10 ).
         * \param p1 pair of doubles. Holds the Cartesian coordinates of the first point
         * \param p2 pair of doubles. Holds the Cartesian coordinates of the second point
         */
        int Euclid(std::pair<double,double> p1, std::pair<double,double> p2);

        /*!
         * Clears all data in the generator implying you can use the same generator object to generato several instances.
         */
        void clearGenerator ();
};

#endif // DATA_H_INCLUDED
