//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define pi 3.14159265358979323846

typedef struct Location Location;
double location_calculateDistanceTo(Location *from, Location *to);
void location_setLongitude(Location* location, double longitude);
void location_setLatitude(Location* location, double latitude);

struct Location {
    double latitude;
    double longitude;
    double (*calculateDistanceTo)(Location* from, Location* to);
    void (*setLongitude)(Location* location, double longitude);
    void (*setLatitude)(Location* location, double latitude);

} locationInit() {
    //printf("Location constructor called!\n");

    Location* location = malloc(sizeof(Location));
    location->calculateDistanceTo = location_calculateDistanceTo;

    Location location1 = *location;
    return location1;
};

// Calculating the distance between 2 locations
// Returns distance in km
double deg2rad(double);
double rad2deg(double);

double deg2rad(double deg) {
    return (deg * pi / 180);
}

double rad2deg(double rad) {
    return (rad * 180 / pi);
}

double location_calculateDistanceTo(Location *from, Location *to) {
    Location *a = from;
    Location *b = to;
    double lat1 = a->latitude;
    double lon1 = a->longitude;
    double lat2 = b->latitude;
    double lon2 = b->longitude;

    double theta, dist;
    theta = lon1 - lon2;
    /*dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
    dist = acos(dist);
    dist = rad2deg(dist);
    dist = dist * 60 * 1.1515 * 1.609344;*/
    return (dist);
}
// End of locations distance calculation

void location_setLongitude(Location* location, double longitude) {
    location->longitude = longitude;
}
void location_setLatitude(Location* location, double latitude) {
    location->longitude = latitude;
}
