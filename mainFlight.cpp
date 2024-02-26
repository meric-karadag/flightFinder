#include "FlightFinder.h"
#include "Exceptions.h"
#include <iostream>

void PrintArray(const std::vector<std::string> &arr)
{
    std::cout << "{";
    for (const std::string &a : arr)
    {
        std::cout << a << ", ";
    }
    std::cout << "}" << std::endl;
}

int main()
{
    FlightFinder flightSystem("turkey_flight.map");
    std::cout << "Loading Map" << std::endl;

    flightSystem.PrintMap();

    std::vector<std::string> result;

    int result = 0;
    std::cout << "Furthest Transfer From Istanbul(IST) via Pegasus\n";
    std::cout << flightSystem.FurthestTransferViaAirline("Istanbul(IST)", "Pegasus");

    // result = flightSystem.FurthestTransferViaAirline("Istanbul(IST)", "BoraJet");
    // std::cout << result;

    /* catch (SameNamedEdgeException &e)
    {
        std::cout << e.ToString() << std::endl;
        return 0;
    }
    catch (DuplicateVertexException &e)
    {
        std::cout << e.ToString() << std::endl;
        return 0;
    }
    catch (VertexNotFoundException &e)
    {
        std::cout << e.ToString() << std::endl;
        return 0;
    }
    */
    return 0;
}