#pragma once
#include<vector2.hpp>
#include <vector>
#include<string>
using namespace std;

struct force_data{
vector2 vect;
string name;

force_data(const vector2 &vecti, const string &namei)
: vect(vecti), name(namei){}

};

struct screen_data{
    float x;
    float y;
    float radius;
  

    screen_data(float xi, float yi, float ri)
    : x(xi), y(yi), radius(ri){}
};
class material_point{
private://все значения в системе CИ
 
public:
vector2 speed;
double x;
double y;
double radius;
double mass;
string color;
vector<force_data>forces;
material_point(const double &massi, const double &xi, const double &yi, const double &radius_i, const string& color_i = "blue")
:mass(massi), x(xi), y(yi), radius(radius_i), color(color_i){}

size_t get_force_data_by_name(const string &name){
    for(int ind = 0; ind < forces.size(); ind++){
        if(forces[ind].name == name){
            return ind;
        }
    }
    return -1;
}
void apply_force(const force_data &force){
    if(get_force_data_by_name(force.name) != -1) return;
    forces.push_back(force);
}

void delete_force_by_name(const string &name){
    for(auto iter = forces.begin(); iter != forces.end(); iter++){
        if((*iter).name == name){
            forces.erase(iter);
            return;
        }
    }
}
static double dist(const material_point &point1, const material_point &point2){
    return sqrtl((point1.x - point2.x) *(point1.x - point2.x) + (point2.y - point1.y) * (point2.y - point1.y));  
}
void apply_impulse(const vector2 &p){ 
    vector2 v = p / this->mass;
    this->speed = this->speed + v;
}
screen_data get_pixel_coords(const float screenH, const float screenW, const float SCALE) const {

    return screen_data(screenW  / 2 + float(this->x) * SCALE, screenH / 2 - float(this->y) * SCALE, float(this->radius) * SCALE);
}
vector2 get_accel(){
    vector2 all_force;
    for(auto &force : forces){
        all_force = all_force + force.vect;
    }

    vector2 accel = all_force / mass;
    return accel;
    
}
//по сути дело за движком, точка сообщает ускорение, умеет хранить скорость и силы которые на нее действуют 
};