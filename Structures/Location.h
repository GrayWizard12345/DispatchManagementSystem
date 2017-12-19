//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define pi 3.14159265358979323846
double Location_CalculateDistanceTo(void *from, void *to);

struct Location {

    double latitude;
    double longitude;
    double (*calculateDistanceTo)(struct Location *, struct Location *);

} locationInit() {
    printf("Location constructor called!\n");

    struct Location* location = malloc(sizeof(struct Location));

    location->calculateDistanceTo = Location_CalculateDistanceTo ;
    return *location;
};

double deg2rad(double);
double rad2deg(double);

double deg2rad(double deg) {
    return (deg * pi / 180);
}

double rad2deg(double rad) {
    return (rad * 180 / pi);
}

double Location_CalculateDistanceTo(void *from, void *to) {
    struct Location *a = from;
    struct Location *b = to;
    double lat1 = a->latitude;
    double lon1 = a->longitude;
    double lat2 = b->latitude;
    double lon2 = b->longitude;

    double theta, dist;
    theta = lon1 - lon2;
    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
    dist = acos(dist);
    dist = rad2deg(dist);
    dist = dist * 60 * 1.1515 * 1.609344;
    return (dist);
}