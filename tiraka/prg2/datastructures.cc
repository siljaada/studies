// Datastructures.cc
//
// Student name: Silja Kantanen
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::station_count()
{
    return stations.size();
}

void Datastructures::clear_all()
{
    stations.clear();
    distances.clear();
    alphabetically.clear();
    regions.clear();
    trains.clear();
    stations_to_regions.clear();
}

std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> stations_vector;
     stations_vector.reserve(stations.size());

     std::transform(stations.begin(), stations.end(), back_inserter(stations_vector),
             [](std::pair<const StationID, const Stationdata> const &p) {return p.first;});
     return stations_vector;
}

Distance calculate_distance_joo(Coord coord)
{
    Distance distance = sqrt(coord.x*coord.x + coord.y*coord.y);
    return distance;
}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    auto it = stations.find(id);
    if(it != stations.end()){
        return false;
    }
    else{
        Stationdata s;
        s.id = id;
        s.name = name;
        s.coords = xy;

        stations[id] = s;

        Distance distance = calculate_distance_joo(xy);
        std::pair<Distance, Stationdata*> a;
        a = {distance, &stations.at(id)};
        distances.push_back(a);

        alphabetically.insert({name, &stations.at(id)});
        return true;
    }
}

Name Datastructures::get_station_name(StationID id)
{
    auto it = stations.find(id);
    if (it != stations.end()){
        return it->second.name;
    }
    else{
        return NO_NAME;
     }
}

Coord Datastructures::get_station_coordinates(StationID id)
{
    auto it = stations.find(id);
    if(it != stations.end()) {
        return it->second.coords;
    }

    else{
        return NO_COORD;
    }
}

std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> alpabetically_order;
    alpabetically_order.reserve(alphabetically.size());
    std::transform(alphabetically.begin(), alphabetically.end(), back_inserter(alpabetically_order),
            [](std::pair<const Name, const Stationdata*> const &p) {return p.second->id;});


    return alpabetically_order;
}

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> all;
    all.reserve(distances.size());

    sort(distances.begin(), distances.end(),[=]( std::pair<Distance, Stationdata*> & a,  std::pair<Distance, Stationdata*> & b){
        if(a.first == b.first){
            int y_a = a.second->coords.y;
            int y_b = b.second->coords.y;
            if(!(y_a < y_b)){
                return a.second->name < b.second->name;
            }
            else{
                return y_a < y_b;
            }
        }
        else{
            return a.first < b.first;
        }
    });

    for(auto it: distances){
        all.push_back(it.second->id);
    }

    return all;
}

StationID Datastructures::find_station_with_coord(Coord xy)
{
    for(auto it : stations){
        if(it.second.coords == xy){
            return it.first;
        }
    }

    return NO_STATION;
}

bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    if(stations.find(id) != stations.end())
    {
        Distance distance_old = calculate_distance_joo(stations.at(id).coords);
        stations[id].coords = newcoord;
        std::pair<Distance, Stationdata*> a1;
        a1 = {distance_old, &stations[id]};
        Distance distance = calculate_distance_joo(newcoord);
        std::pair<Distance, Stationdata*> a2 = {distance, &stations[id]};
        distances.erase(std::find(distances.begin(), distances.end(),a1));
        distances.push_back(a2);
        return true;
    }
    else{
        return false;
    }
}

bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    std::vector<std::pair<Time, TrainID>> trains_vec;
    trains_vec.reserve(trains.size());
    std::pair<Time, TrainID> train_data = {time, trainid};
    trains_vec = trains[stationid];
    if(stations.find(stationid) == stations.end()){
        return false;
    }
    else{
        auto p = std::find(trains[stationid].begin(),trains[stationid].end(),
                           train_data);
        if(p != trains[stationid].end()){
            return false;
        }
        else{
            trains_vec.push_back({time, trainid});
            trains[stationid] = trains_vec;
            return true;
        }
    }
}

bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{

   std::unordered_map<StationID, std::vector<std::pair<Time, TrainID>>>::const_iterator a = trains.find(stationid);
    if(a != trains.end()){
        std::pair<Time, TrainID> train_data = {time, trainid};
        trains[stationid].erase(std::find(trains[stationid].begin(), trains[stationid].end(), train_data));
        return true;
    }
    else{
        return false;
    }
}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time, TrainID>> trains_after;
    trains_after.reserve(trains.size());
    auto beg = trains[stationid].begin();
    auto end = trains[stationid].end();
    if(trains.find(stationid) != trains.end()){
        for(auto it =beg; it < end; it++){
            if(it->first >=  time){
                trains_after.push_back(*it);
            }
        }
    }

    else
    {
        trains_after.push_back({NO_TIME, NO_TRAIN});
    }

    return trains_after;
}

bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    if(regions.find(id) == regions.end()){
        Node newregion;
        newregion.id_= id;
        newregion.name_ = name;
        newregion.coords_ = coords;
        newregion.parent = nullptr;
        newregion.children_ = {nullptr};
        regions.insert({id,newregion});
        return true;
    }
    else{
        return false;
    }
}

std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> allregions;
    allregions.reserve(regions.size());

    std::transform(regions.begin(), regions.end(), back_inserter(allregions),
            [](std::pair<const RegionID, const Node> const &p) {return p.first;});

    return allregions;
}

Name Datastructures::get_region_name(RegionID id)
{
    if(regions.find(id) != regions.end()){
        return regions[id].name_;
    }
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    if(regions.find(id) != regions.end()){
        return regions[id].coords_;
    }
    return {NO_COORD};

}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    if(regions.find(id) != regions.end() || regions.find(parentid) != regions.end()){
        Node* parent;
        parent = &regions[parentid];
        Node* child;
        child = &regions[id];
        parent->children_.push_back(child);
        child->parent = parent;
        return true;
    }
    else{
        return false;
    }

}


bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    if(stations.find(id) != stations.end()){
        Node *s;
        s = &regions[parentid];
        stations_to_regions.insert({id, s});
        return true;
    }
    else{
        return false;
    }
}

std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> belongs = { };
    if(stations_to_regions.find(id) != stations_to_regions.end()){


        Node child;
        child = *stations_to_regions[id];
        Node* parent;
        parent = child.parent;
        belongs.push_back(child.id_);
        while(parent != nullptr){
           belongs.push_back(parent->id_);
           child = *parent;
           parent = child.parent;

        }
    }
    if(stations.find(id) == stations.end()){
        belongs.push_back(NO_REGION);
    }
    return belongs;

}

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("all_subregions_of_region()");
}

std::vector<StationID> Datastructures::stations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("stations_closest_to()");
}

bool Datastructures::remove_station(StationID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_station()");
}

RegionID Datastructures::common_parent_of_regions(RegionID /*id1*/, RegionID /*id2*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("common_parent_of_regions()");
}
/*
 @brief: junat lisätään tieto rakenteeseen. Myös viereiset asemat on lisätty sekä neigbours,
 että next-stations unorder_mappeihin.
 palauttaa true/false


 */
bool Datastructures::add_train(TrainID trainid, std::vector<std::pair<StationID, Time> > stationtimes)
{
    if(trains.find(trainid) != trains.end()){
        return false;
    }
    std::vector<std::pair<Time, TrainID>> trains_vec;
    auto begin = stationtimes.begin();
    auto end = stationtimes.end();
    auto next = begin+1;
    edge edge;
    for(auto pair = begin; pair < end; pair++){
        if(stations.find(pair->first) != stations.end()){
            stations[pair->first].trains.push_back({pair->second, trainid});

            if(next < end){
                trains[trainid].push_back({pair->second, pair->first});
                stations[pair->first].neighbours[&stations[next->first]] = calculate_distance(stations[pair->first].coords,
                        stations[next->first].coords);
                stations[pair->first].next_stations[trainid]=&stations[next->first];


            }

            else{
                edge.distance = NO_DISTANCE;
                edge.trainid = trainid;
                stations[pair->first].neighbours[nullptr] = NO_DISTANCE;

            }
            next++;

        }

        else{
            return false;
        }
    }
    return true;



}
/*
 @brief: haetaan aseman viereiset asemat jonne pääsee junilla
 palauttaa vectrin, jossa viereiset asemien id ovat.


 */
std::vector<StationID> Datastructures::next_stations_from(StationID id)
{
    std::vector<StationID> next_stations;
    next_stations.reserve(stations.size());
    if(stations.find(id) != stations.end()){
        auto beg = stations[id].next_stations.begin();
        auto end = stations[id].next_stations.end();

        for(auto it = beg; it != end; it++){
            if(it->second != nullptr){
               next_stations.push_back(it->second->id);
            }

        }
        return next_stations;
    }


    return {NO_STATION};
}
/*
 @brief: etsii asemalta lähtevän junan loppu asemat.
 palauttaa vektorin, jossa on junan reitin seuraavat asemat


 */
std::vector<StationID> Datastructures::train_stations_from(StationID stationid, TrainID trainid)
{
    if(stations.find(stationid) == stations.end() or trains.find(trainid) == trains.end()
            or stations[stationid].next_stations[trainid] == nullptr ){
        return {NO_STATION};
    }
    std::vector<StationID> stations_to;
    Stationdata* current = stations[stationid].next_stations[trainid];
    while(current != nullptr){
        stations_to.push_back(current->id);
        current = current->next_stations[trainid];
    }
    return stations_to;

}

void Datastructures::clear_trains()
{
    trains.clear();
    auto iter = stations.begin();
    while(iter != stations.end()){
        iter->second.neighbours.clear();
        iter->second.next_stations.clear();
        iter->second.trains.clear();
        delete iter->second.pii;
        iter++;

    }
}

/*
 @brief: etsii reitin annetujen asemien välillä.
 palauttaa asemine id sekä niiden väliset etäisyydet


 */

std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID fromid, StationID toid)
{
    std::vector<std::pair<StationID, Distance>> route;
    if(stations.find(fromid) == stations.end() or stations.find(toid) == stations.end()){
        route.push_back({NO_STATION, NO_DISTANCE});
        return route;
    }
    search_route(fromid, toid);
    Stationdata* ptr = &stations[toid];
    while(ptr->pii != nullptr){
        route.insert(route.begin(), {ptr->id,ptr->distance});
        if(ptr->pii->id == fromid){
            route.insert(route.begin(), {ptr->pii->id, ptr->pii->distance});
            return route;
        }
        ptr = ptr->pii;


    }
    return {};

}

std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID fromid, StationID toid)
{
    std::vector<std::pair<StationID, Distance>> route;
    if(stations.find(fromid) == stations.end() or stations.find(toid) == stations.end()){
        route.push_back({NO_STATION, NO_DISTANCE});
        return route;
    }
    search_route(fromid, toid);
    Stationdata* ptr = &stations[toid];
    while(ptr->pii != nullptr){
        route.insert(route.begin(), {ptr->id,ptr->distance});
        if(ptr->pii->id == fromid){
            route.insert(route.begin(), {ptr->pii->id, ptr->pii->distance});
            return route;
        }
        ptr = ptr->pii;


    }
    return {};
}

std::vector<StationID> Datastructures::route_with_cycle(StationID /*fromid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_with_cycle()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID /*fromid*/, StationID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_shortest_distance()");
}

std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival(StationID /*fromid*/, StationID /*toid*/, Time /*starttime*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_earliest_arrival()");
}

void Datastructures::search_route(StationID fromid, StationID toid)
{
    all_colors_white();
    std::vector<Stationdata* > queue;
    queue.push_back(&stations[fromid]);
    stations[fromid].distance =0;
    stations[fromid].color = "gray";
    while(!queue.empty()){
        auto now = queue[0];
        queue.erase(queue.begin());
        auto beg = now->neighbours.begin();
        auto end = now->neighbours.end();
        for(auto it = beg; it != end; it++){
            if(it->first != nullptr){
                if(it->first->color == "white"){
                    it->first->color = "gray";
                    it->first->distance = now->distance + it->second;
                    it->first->pii = now;

                    if(now->id != toid){
                        queue.push_back(it->first);;
                    }
            }
            now->color = "black";
        }
    }
    }
}

Distance Datastructures::calculate_distance(Coord from_xy, Coord to_xy)
{
    Distance distance;
    Distance sum_x;
    Distance sum_y;
    sum_x = (from_xy.x-to_xy.x)*(from_xy.x-to_xy.x);
    sum_y = (from_xy.y-to_xy.y)*(from_xy.y-to_xy.y);
    distance = sqrt(abs(sum_x+sum_y));
    return distance;

}

void Datastructures::all_colors_white()
{
    auto iter = stations.begin();
    while(iter!= stations.end()){
        iter->second.color = "white";
        iter++;
    }
}
