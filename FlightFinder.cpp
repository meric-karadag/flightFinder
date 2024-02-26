#include "FlightFinder.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void FlightFinder::PrintCanNotHalt(const std::string &airportFrom,
                                   const std::string &airportTo,
                                   const std::string &airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void FlightFinder::PrintCanNotResumeFlight(const std::string &airportFrom,
                                           const std::string &airportTo,
                                           const std::string &airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void FlightFinder::PrintFlightFoundInCache(const std::string &airportFrom,
                                           const std::string &airportTo,
                                           bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              << " is found in cache." << std::endl;
}

void FlightFinder::PrintFlightCalculated(const std::string &airportFrom,
                                         const std::string &airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              << "." << std::endl;
}

void FlightFinder::PrintPathDontExist(const std::string &airportFrom,
                                      const std::string &airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo << "\"." << std::endl;
}

void FlightFinder::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void FlightFinder::PrintCache()
{
    lruTable.PrintTable();
}

FlightFinder::FlightFinder(const std::string &flightMapPath)
    : navigationMap(flightMapPath)
{
}

//=======================//
//          TODO         //
//=======================//
void FlightFinder::HaltFlight(const std::string &airportFrom,
                              const std::string &airportTo,
                              const std::string &airlineName)
{
    /* TODO */
    try
    {
        EdgeResult result = navigationMap.FindEdge(airlineName, airportFrom, airportTo);
        HaltedFlight halted = HaltedFlight(airportFrom, airportTo, airlineName, result.w0, result.w1);
        haltedFlights.push_back(halted);
        navigationMap.RemoveEdge(airlineName, airportFrom, airportTo);
    }
    catch (const VertexNotFoundException &e)
    {
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
    }
    catch (const EdgeNotFoundException &e)
    {
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
        ;
    }
}

// (Direct Function call)
void FlightFinder::ContinueFlight(const std::string &airportFrom,
                                  const std::string &airportTo,
                                  const std::string &airlineName)
{
    /* TODO */
    for (size_t i = 0; i < haltedFlights.size(); i++)
    {
        if (haltedFlights[i].airportFrom == airportFrom && haltedFlights[i].airportTo == airportTo && haltedFlights[i].airline == airlineName)
        {
            navigationMap.AddEdge(airlineName, airportFrom, airportTo, haltedFlights[i].w0, haltedFlights[i].w1);
            haltedFlights.erase(haltedFlights.begin() + i);
            return;
        }
    }
    PrintCanNotResumeFlight(airportFrom, airportTo, airlineName);
}

void FlightFinder::FindFlight(const std::string &startAirportName,
                              const std::string &endAirportName,
                              float alpha)
{
    /* TODO */
    // Check if alpha == 0 or 1 to decide whether to cache the path or not
    bool willCached = false;
    if (alpha == 0 || alpha == 1)
        willCached = true;
    try
    {
        // Create a vector to store the calculated path
        std::vector<int> ove;
        if (willCached)
        {
            int start, end;
            VertexResult vertices = navigationMap.FindVertex(startAirportName, endAirportName);
            start = vertices.start;
            end = vertices.end;

            bool isCostWeighted = (alpha == 1) ? false : true;
            // if the requested path is in the cache print from cache
            if (lruTable.Find(ove, start, end, isCostWeighted, true))
            {
                PrintFlightFoundInCache(startAirportName, endAirportName, isCostWeighted);
                navigationMap.PrintPath(ove, alpha, true);
                return;
            }
            // if the requested path is not in the cache calculate, print and cache the path
            else
            {
                navigationMap.HeuristicShortestPath(ove, startAirportName, endAirportName, alpha);
                // If does not catches an exception there exists a path
                PrintFlightCalculated(startAirportName, endAirportName, isCostWeighted);
                navigationMap.PrintPath(ove, alpha, true);
                // Try to insert the path into the hash table
                // if table is full, remove last LRU and insert
                try
                {
                    lruTable.Insert(ove, isCostWeighted);
                }
                catch (const TableCapFullException e)
                {
                    lruTable.RemoveLRU(1);
                    lruTable.Insert(ove, isCostWeighted);
                }
            }
        }
        // if  alpha != 0 and alpha != 1, calculate path but do not cache it
        else
        {
            navigationMap.HeuristicShortestPath(ove, startAirportName, endAirportName, alpha);
            // If does not catches an exception there exists a path
            navigationMap.PrintPath(ove, alpha, true);
        }
    }
    catch (const VertexNotFoundException &e)
    {
        PrintPathDontExist(startAirportName, endAirportName);
    }
    catch (const EdgeNotFoundException &e)
    {
        PrintPathDontExist(startAirportName, endAirportName);
    }
}

void FlightFinder::FindSpecificFlight(const std::string &startAirportName,
                                      const std::string &endAirportName,
                                      float alpha,
                                      const std::vector<std::string> &unwantedAirlineNames) const
{
    try
    {
        // Create a vector to store the calculated path
        std::vector<int> ove;
        int start, end;

        bool isCostWeighted = (alpha == 1) ? false : true;
        // Calculate the requested  path
        navigationMap.FilteredShortestPath(ove, startAirportName, endAirportName, alpha, unwantedAirlineNames);
        navigationMap.PrintPath(ove, alpha, true);
    }
    catch (const VertexNotFoundException &e)
    {
        PrintPathDontExist(startAirportName, endAirportName);
    }
    catch (const EdgeNotFoundException &e)
    {
        PrintPathDontExist(startAirportName, endAirportName);
    }
}

int FlightFinder::FurthestTransferViaAirline(const std::string &airportName,
                                             const std::string &airlineName) const
{
    try
    {
        int result;
        result = navigationMap.MaxDepthViaEdgeName(airportName, airlineName);
        return result;
    }
    catch (const VertexNotFoundException &e)
    {
        return -1;
    }
}