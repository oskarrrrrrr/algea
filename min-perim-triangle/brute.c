#include <stdio.h>
#include <inttypes.h>
#include <limits.h>
#include <unistd.h>

#define MAX_N 3500000

typedef int32_t i32;
typedef int64_t i64;

typedef struct { i32 x, y; } Point;

void printPoint(Point p) {
    printf("%" PRId32 " %" PRId32 "\n", p.x, p.y);
}

void printPoints(const Point *arr, size_t n) {
    for (i32 i = 0; i < n; i++) {
        printPoint(arr[i]);
    }
}

i64 distSq(Point p1, Point p2) {
    i64 x1 = (i64)p1.x;
    i64 y1 = (i64)p1.y;
    i64 x2 = (i64)p2.x;
    i64 y2 = (i64)p2.y;
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

i32 n;
Point points[MAX_N];

void readInput() {
    scanf("%" SCNd32, &n);
    for (i32 i = 0; i < n; i++) {
        scanf("%" SCNd32 " %" SCNd32, &points[i].x, &points[i].y);
    }
}

i64 brute(size_t l, size_t r, Point result[3]) {
    i64 maxCirc = LLONG_MAX;
    for (i32 i = l; i < r; i++) {
        for (i32 j = i+1; j < r; j++) {
            for (i32 k = j+1; k < r; k++) {
                i64 circ = (
                    distSq(points[i], points[j])
                    + distSq(points[j], points[k])
                    + distSq(points[k], points[i])
                );
                if (circ < maxCirc) {
                    maxCirc = circ;
                    result[0] = points[i];
                    result[1] = points[j];
                    result[2] = points[k];
                }
            }
        }
    }
    return maxCirc;
}

int main() {
    readInput();
    Point bruteResult[3];
    brute(0, n, bruteResult);
    printPoints(bruteResult, 3);
    return 0;
}
