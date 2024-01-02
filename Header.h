#pragma once
#define FIELDPOLY_HPP

#include <iostream>
#include <vector>
#include <array>
#include <bitset>
#include <sstream>
#include <chrono>


#define array_size 179


class FieldGF {
    
private:   
    std::array<int, array_size> data;

public:
    FieldGF();
    FieldGF(const std::array<int, array_size>& coefficients);


    FieldGF operator+(const FieldGF& other) const;
    FieldGF operator*(const FieldGF& other) const;
    FieldGF toSquare(const FieldGF& A) const;
    FieldGF pow(const FieldGF& power) const;
    FieldGF inverse() const; 
    FieldGF trace() const;

    FieldGF zero() const;
    FieldGF one() const;


    std::string toString() const;
    static FieldGF fromString(const std::string& str);
};


