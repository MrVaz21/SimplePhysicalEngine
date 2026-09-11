#include<iostream>
#include<vector>
#include "vector2.hpp"
#include "material_point.hpp"
#include<raylib.h>
#include<thread>
#include<chrono>
using namespace std;

class simple_engine{
private:
    const double gravity_const = 6.6743 * 10e-11; 
public:
    bool collisions = true;
    bool gravity= true;
    vector<material_point>points;

    size_t add_point(material_point&point){
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
};

int main(void){
    const float SCALE = 50;
    const int screenWidth = 800;
    const int screenHeight = 600;
   
    InitWindow(screenWidth, screenHeight, "Simple Engine Visualization");
    SetTargetFPS(60);

    simple_engine engine;

    material_point point1(50000000, 0, 0, 1);
    material_point point3(500000000, 2, 2, 0.2);
    
 //  point1.apply_impulse(vector2(10, 0));
    
    material_point point2(5, 1, 0, 0.1);

    point2.apply_impulse(vector2(0, 1));
    engine.add_point(point1);
    engine.add_point(point2);
    engine.add_point(point3);
    this_thread::sleep_for(chrono::milliseconds(100));
    
    while (!WindowShouldClose()) {
        
        float deltaTime = GetFrameTime();
        engine.update_points_states(deltaTime);

    
        BeginDrawing();
        ClearBackground(BLACK);

        for (const material_point &point : engine.points) {
            screen_data data = point.get_pixel_coords(screenHeight, screenWidth, SCALE);
            DrawCircleV({ data.x, data.y }, data.radius, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
// F = m1 * m2 / r * 2 * G