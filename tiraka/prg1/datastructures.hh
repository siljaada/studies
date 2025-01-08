// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <iterator>
// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate:
    //mapin koon palauttaminen on vakioaikasta
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: clear on lineaarinen n
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-loop on lineaarinen
    std::vector<StationID> all_stations();

    // Estimate of performance: O(1)
    // Short rationale for estimate: vakioaikainen kun  lisätään
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: unorderde_mapin palautus O(1)
    Name get_station_name(StationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: unordered_mapin palautus O(1)˝
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-loopilla käydään läpi
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-loopilla käydään läpi
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-loopilla käydään läpi
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: vaihdetaan vai tietty kordinaatti
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: etsitään paljon find ja käytetään []
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance:O(logn)
    // Short rationale for estimate: käytetään paljon algoritmia find
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: foor loop ja paljon etsimistä find
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(logn)
    // Short rationale for estimate: käytetään useita findeja
    // ja lisätään useisiin tietorakenteisiin
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: transform-metodi on lineaarinen
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(1)
    // Short rationale for estimate: etsitään mapista ja palutetaan
    Name get_region_name(RegionID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: käytetään findeja ja sijoituksia
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate:tutkitaan ja lisätään jolloin alussa hitaampaa
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(logn)
    // Short rationale for estimate:tutkiaan paljon aluksi hiitaampaa
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: while-loop, joka hidastuu taisesti datan kasvaessa
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_station(StationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    // Add stuff needed for your class implementation here
    
    //Stationeiden data
    struct Stationdata {
        StationID id;
        Name name;
        Coord coords;
        std::vector<std::pair<Time, TrainID>> trains;
    };
    //Sisältää kaikki asemat
    std::unordered_map < StationID, Stationdata > stations;
    
    std::vector< std::pair<Distance, Stationdata*>> distances;
    std::map < Name, Stationdata*> alphabetically;
    std::unordered_map < StationID, std::vector<std::pair<Time, TrainID>> > trains;
    struct Node{
        RegionID id_;
        Name name_;
        std::vector<Coord> coords_;
        std::vector<Node*> children_;
        Node* parent;
    };

    std::unordered_map<RegionID, Node> regions;
    std::unordered_map<StationID, Node*> stations_to_regions;
    void preorder(Node* node, std::vector<RegionID> regions);

};

#endif // DATASTRUCTURES_HH

