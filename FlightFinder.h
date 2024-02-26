#ifndef FLIGHT_FINDER_H
#define FLIGHT_FINDER_H

#include "HashTable.h"
#include "MultiGraph.h"

#define FLIGHT_TABLE_SIZE 29

struct HaltedFlight
{
    std::string airportFrom;
    std::string airportTo;
    std::string airline;
    float w0;
    float w1;
    HaltedFlight(const std::string &From,
                 const std::string &To,
                 const std::string &airlineName,
                 float first,
                 float second)
    {
        airportFrom = From;
        airportTo = To;
        airline = airlineName;
        w0 = first;
        w1 = second;
    }
};

class FlightFinder
{
private:
    HashTable<FLIGHT_TABLE_SIZE> lruTable;
    MultiGraph navigationMap;

    // Print Related
    // Use these
    static void PrintCanNotHalt(const std::string &airportFrom,
                                const std::string &airportTo,
                                const std::string &airlineName);
    static void PrintCanNotResumeFlight(const std::string &airportFrom,
                                        const std::string &airportTo,
                                        const std::string &airlineName);
    static void PrintFlightFoundInCache(const std::string &airportFrom,
                                        const std::string &airportTo,
                                        bool isCostWeighted);
    static void PrintFlightCalculated(const std::string &airportFrom,
                                      const std::string &airportTo,
                                      bool isCostWeighted);
    static void PrintPathDontExist(const std::string &airportFrom,
                                   const std::string &airportTo);

    //
    std::vector<HaltedFlight> haltedFlights;
    //

protected:
public:
    // Constructors & Destructor
    FlightFinder(const std::string &flightMapPath);

    // Members
    // Halting/Continuing flights

    // (Direct Function call)
    void HaltFlight(const std::string &airportFrom,
                    const std::string &airportTo,
                    const std::string &airlineName);

    // (Direct Function call)
    void ContinueFlight(const std::string &airportFrom,
                        const std::string &airportTo,
                        const std::string &airlineName);
    // Flight Finding
    // (Direct Function call)
    void FindFlight(const std::string &startAirportName,
                    const std::string &endAirportName,
                    float alpha);
    // (Direct Function call)
    void FindSpecificFlight(const std::string &startAirportName,
                            const std::string &endAirportName,
                            float alpha,
                            const std::vector<std::string> &unwantedAirlineNames) const;

    // (Direct Function call)
    int FurthestTransferViaAirline(const std::string &airportName,
                                   const std::string &airlineName) const;

    // Printing Functions
    void PrintMap();
    void PrintCache();
};

#endif // FLIGHT_FINDER_H