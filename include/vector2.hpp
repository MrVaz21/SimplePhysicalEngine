#pragma once
#include<cmath>
class vector2{
public:
    double x;
    double y;
    vector2(double xi = 0, double yi = 0)
    : x(xi), y(yi)
    {

    }

    friend vector2 operator +(const vector2&a, const vector2&b){
        return vector2(a.x + b.x, a.y + b.y);
    }
    static double pw(double a){
        return a * a;
    }
    friend vector2 operator /(const vector2 &a, const double &n){
        return vector2(a.x / n, a.y / n);
    }
    friend vector2 operator *(const vector2 &a, const double &n){
        return vector2(a.x * n, a.y * n);
    }

    friend double operator *(const vector2 &a, const vector2&b){
        return a.x * b.x + a.y * b.y;
    }

    friend vector2 operator -(const vector2 &a, const vector2&b){
        return vector2(a.x - b.x, a.y - b.y);
    }
    double get_sz(){
        return sqrtl(pw(this->x) + pw(this->y));
    }
    void normalize(){
        if (get_sz() <=1e-5){
            return;
        }
        (*this) =  (*this) / get_sz();
    }


};
