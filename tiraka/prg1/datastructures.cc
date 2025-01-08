// Datastructures.cc
//
// Student name: Silja Kantane
// Student email: silja.kantanen@tuni.fi
// Student number: 50343681

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

Distance calculate_distance(Coord coord)
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

        Distance distance = calculate_distance(xy);
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
        Distance distance_old = calculate_distance(stations.at(id).coords);
        stations[id].coords = newcoord;
        std::pair<Distance, Stationdata*> a1;
        a1 = {distance_old, &stations[id]};
        Distance distance = calculate_distance(newcoord);
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

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    std::vector<RegionID>children;
    if(regions.find(id) != regions.end()){
        Node *vara = nullptr;
        vara = &regions[id];
        return children;
    }
    else{
        return {NO_REGION};
    }
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

/*void Datastructures::preorder(Node* node, std::vector<RegionID> regions)
{
    if(node != NULL){

        for(auto child : node->children_){
            preorder(child,regions);
            regions.push_back(node->id_);
        }
    }
}*/
