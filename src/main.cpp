#include<iostream>
#include<vector>
#include "vector2.hpp"
#include "material_point.hpp"
#include<raylib.h>
#include<thread>
#include<chrono>
#include<map>
using namespace std;
struct screen_settings{
    const float SCALE;
    const int screenWidth;
    const int screenHeight;
    screen_settings(const int screenWighti = 800, const int screenHeighti = 600, const float SCALEi = 50)
    : screenWidth(screenWighti), screenHeight(screenHeighti),  SCALE(SCALEi){}
};
class simple_engine{
private:
    const double gravity_const = 6.6743 * 10e-11; 
public:
    bool collisions = true;
    bool gravity= true;
    vector<material_point>points;

    size_t add_point(material_point&point ){
        points.push_back(point);
        return points.size() - 1;
    }
    
    void update_points_states(const double &delta_time){//v0, a, x0, y0 S=V0t + at^2/2
        for(int i = 0; i < points.size(); i++){
            material_point &point = points[i];
            //cout<<"index: "<<i<<" x: "<<point.x<<" y: "<<point.y<<endl;
            double Sx = point.speed.x * delta_time +  point.get_accel().x * vector2::pw(delta_time) / double(2);
            double Sy = point.speed.y * delta_time + point.get_accel().y * vector2::pw(delta_time) / double(2);
            point.x += Sx;
            point.y += Sy;
            point.speed = point.speed + point.get_accel() * delta_time;
            if (collisions or gravity){
                for(int j = 0; j < i; j++){
                    material_point &cand_point = points[j];
                        
                    
                    double dist = material_point::dist(point, cand_point);

                    if(gravity){
                        double gravity_power = gravity_const * (point.mass * cand_point.mass) / (dist * dist);
                        vector2 vect(cand_point.x - point.x, cand_point.y - point.y);
                        vect.normalize();
                        vect = vect * gravity_power;
                        string name1 = "gravity_to_" + to_string(j);
                        string name2 = "gravity_to_" + to_string(i);
                        point.delete_force_by_name(name1);
                        point.apply_force(force_data(vect, name1));
                        vect = vect * -1;
                        cand_point.delete_force_by_name(name2);
                        cand_point.apply_force(force_data(vect, name2));
                    }

                    if(collisions){
                        vector2 n = vector2(cand_point.x - point.x, cand_point.y - point.y);   // от point к cand    
                        double min_dist = point.radius + cand_point.radius;

                        
                        if (dist >= min_dist) continue;

                        
                        if (dist < 1e-9) {
                        
                            n = vector2(1, 0);
                            dist = 1e-9;
                        }

                        vector2 n_hat = n / dist;

                    
                        double overlap = min_dist - dist;
                        double total_mass = point.mass + cand_point.mass;

            
                        double push_a = overlap * (cand_point.mass / total_mass);
                        double push_b = overlap * (point.mass / total_mass);

                        point.x -= n_hat.x * push_a;
                        point.y -= n_hat.y * push_a;
                        cand_point.x += n_hat.x * push_b;
                        cand_point.y += n_hat.y * push_b;


                        vector2 rel = point.speed - cand_point.speed;
                        double rel_n = rel * n_hat;

            
                        if (rel_n <= 0) continue;

                        double factor = 2.0 * rel_n / total_mass;

                        point.speed = point.speed - n_hat * (factor * cand_point.mass);
                        cand_point.speed = cand_point.speed + n_hat * (factor * point.mass);
                    }
                }     
            }
        }
    }
    static screen_settings get_presset(const string &presset_name){
        map<string, screen_settings>mp = {
            {"simple", screen_settings(800, 600, 50)}, 
            {"default", screen_settings(1024, 768, 50)},
            {"mega", screen_settings(1920, 1080, 3.34 / 1e9)}
        };

        if(mp.count(presset_name)){
            return mp[presset_name];
        }
        return mp["simple"];
    }
    static Color get_color_by_color_name(const string &color_name){
        map<string, Color>mp = {
            {"blue", Color({137, 207, 240, 255})},
            {"yellow", Color({255, 250, 0, 255})},
            {"white", Color({255, 255, 255, 255})}, 
            {"orange", Color({255, 127, 39, 255})}
        };
        if(mp.count(color_name)){
            return mp[color_name];
        }
        return mp["white"];
    }
    void start_screen(const screen_settings &settings){
        InitWindow(settings.screenWidth, settings.screenHeight, "Simple Engine Visualization");
        SetTargetFPS(60);
        this_thread::sleep_for(chrono::milliseconds(100));
        while (!WindowShouldClose()) {
        
        float deltaTime = GetFrameTime();
        (*this).update_points_states(deltaTime);

        BeginDrawing();
        ClearBackground(BLACK);

        for (const material_point &point : this->points) {
            screen_data data = point.get_pixel_coords(settings.screenHeight, settings.screenWidth, settings.SCALE);
            DrawCircleV({ data.x, data.y }, data.radius, get_color_by_color_name(point.color));
        }

        EndDrawing();
    }

    CloseWindow();
    }
};

int main(void){

    simple_engine engine;
    engine.gravity = false;
    material_point sun(1.98892 * 1e30, 0, 0, 4.49 *1e9, "yellow");
    material_point earth(5.972 * 1e24, 0, 149.6 * 1e9, 1.5 * 1e9, "blue");

    material_point mars(6.417 * 1e23, 0, 152.6 * 1e9, 1 * 1e9, "orange");
    earth.apply_impulse(vector2(1e35, 0));
    mars.apply_impulse(vector2(1e34 * 1.5, 0));
    engine.gravity = true;
    engine.add_point(sun);
    engine.add_point(earth);
    engine.add_point(mars);

    engine.start_screen(simple_engine::get_presset("mega"));

    return 0;
}
// F = m1 * m2 / r * 2 * G