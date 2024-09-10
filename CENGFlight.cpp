#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}

//=======================//
//          TODO         //
//=======================//
void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{
    /* TODO */
    std::vector<GraphVertex> vertices= navigationMap.getVertexList();
    int airportFromIndex = -1, airportToIndex = -1;
    for (int i = 0; i < (int)vertices.size(); ++i)
    {
        if (vertices[i].name == airportFrom)
            airportFromIndex = i;
        if (vertices[i].name == airportTo)
            airportToIndex = i;
    }


    if (airportFromIndex == -1 or airportToIndex == -1)
    {
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
        return;
    }
    bool flag = false;
    
    for(int i=0; i<(int)vertices[airportFromIndex].edges.size(); i++){
        if((vertices[airportFromIndex].edges[i].endVertexIndex==airportToIndex and vertices[airportFromIndex].edges[i].name==airlineName) ){
           
            flag=true;
        }
    }
    if(flag==false){
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
        return;
    }
    
    
    for (int i = 0; i < (int)vertices[airportFromIndex].edges.size(); i++)
    {
        if (vertices[airportFromIndex].edges[i].endVertexIndex == airportToIndex && vertices[airportFromIndex].edges[i].name == airlineName)
        {
            HaltedFlight haltedFlight;
            haltedFlight.airportFrom = airportFrom;
            haltedFlight.airportTo = airportTo;
            haltedFlight.airline = airlineName;
            haltedFlight.w0 = vertices[airportFromIndex].edges[i].weight[0];
            haltedFlight.w1 = vertices[airportFromIndex].edges[i].weight[1];

            

            navigationMap.RemoveEdge(airlineName,airportFrom,airportFrom);
            haltedFlights.push_back(haltedFlight);
            return;
        }
    }

    
}

    // (Direct Function call)
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    /* TODO */
    
    for(int i=0;i<(int)haltedFlights.size();i++){
        if(haltedFlights[i].airportFrom==airportFrom and haltedFlights[i].airportTo==airportTo and haltedFlights[i].airline==airlineName){
            navigationMap.AddEdge(airlineName,airportFrom,airportTo,haltedFlights[i].w0,haltedFlights[i].w1);
            haltedFlights.erase(haltedFlights.begin()+i);
            return;
        }
    }
    PrintCanNotResumeFlight(airportFrom,airportTo,airlineName);
}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    /* TODO */
    std::vector<int> path;
    std::vector<GraphVertex> vertices= navigationMap.getVertexList();
    int airportFromIndex = -1, airportToIndex = -1;
    for (int i = 0; i < (int)vertices.size(); ++i)
    {
        if (vertices[i].name == startAirportName)
            airportFromIndex = i;
        if (vertices[i].name == endAirportName)
            airportToIndex = i;
    }
    bool isCostWeighted=false;
    if(alpha == 0 ){
        isCostWeighted = true;
    }
    if(alpha==0 or alpha==1){
        
        if(lruTable.Find(path,airportFromIndex,airportToIndex,isCostWeighted,true)){
            PrintFlightFoundInCache(startAirportName,endAirportName,isCostWeighted);
            navigationMap.PrintPath(path,alpha,true);
        }
        else if(navigationMap.HeuristicShortestPath(path,startAirportName,endAirportName,alpha)){
            PrintFlightCalculated(startAirportName,endAirportName,isCostWeighted);
            lruTable.Insert(path,isCostWeighted);
            navigationMap.PrintPath(path,alpha,true);
        }else{
            PrintPathDontExist(startAirportName,endAirportName);
        }
        
    }else{
        if(navigationMap.HeuristicShortestPath(path,startAirportName,endAirportName,alpha)){
            navigationMap.PrintPath(path,alpha,true);
        }else{
                PrintPathDontExist(startAirportName,endAirportName);
        }
    }
}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{
    /* TODO */
    std::vector<int> path;
    navigationMap.FilteredShortestPath(path,startAirportName,endAirportName,alpha,unwantedAirlineNames);
    navigationMap.PrintPath(path,alpha,true);
}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    /* TODO */
}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    /* TODO */
    try{
        int result = navigationMap.MaxDepthViaEdgeName(airportName,airlineName);
        return result;
    }
    catch(const VertexNotFoundException& ex){
        return -1;
    }
}
