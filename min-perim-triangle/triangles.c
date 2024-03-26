#include <math.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_N 3500000

typedef int32_t i32;
typedef int64_t i64;
typedef double f64; 

typedef struct { i32 x, y; } Point;

void printPoint(Point p) {
    printf("%" PRId32 " %" PRId32 "\n", p.x, p.y);
}

void printPoints(Point *arr, size_t n) {
    for (i32 i = 0; i < n; i++) {
        printPoint(arr[i]);
    }
}

void readInput(i32 *n, Point *X) {
    scanf("%" SCNd32, n);
    for (i32 i = 0; i < *n; i++) {
        scanf("%" SCNd32 " %" SCNd32, &X[i].x, &X[i].y);
    }
}

i64 distSq(Point p1, Point p2) {
    i64 x1 = (i64)p1.x;
    i64 y1 = (i64)p1.y;
    i64 x2 = (i64)p2.x;
    i64 y2 = (i64)p2.y;
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

f64 dist(i64 distSq) {
    return sqrt((f64)distSq);
}

int compPointsByX(const void *e1, const void *e2) {
    int res = ((*(Point*)e1).x - (*(Point*)e2).x); 
    if (res != 0) { 
        return res;
    } else {
        return (*(Point*)e1).y - (*(Point*)e2).y; 
    }
}

int compPointsByY(const void *e1, const void *e2) {
    int res = (*(Point*)e1).y - (*(Point*)e2).y;
    if (res != 0) {
        return res;
    } else {
        return ((*(Point*)e1).x - (*(Point*)e2).x);
    }
}

typedef struct {
    i64 minCircSq;
    double minCirc;
    Point points[3];
} Result;

void checkTriangle(Point p1, Point p2, Point p3, Result *result) {
    i64 d1 = distSq(p1, p2);
    i64 d2 = distSq(p2, p3);
    i64 d3 = distSq(p3, p1);
    i64 circSq = d1 + d2 + d3;
    if (circSq < result->minCircSq) {
        result->minCircSq = circSq;
        result->minCirc = dist(d1) + dist(d2) + dist(d3);
        result->points[0] = p1;
        result->points[1] = p2;
        result->points[2] = p3;
    }
}

void _findMinCircTriangle(Point *X, Point *Y, i32 n, i32 yEndOffset, Result *result) {
    if (n < 6) {
        for (i32 i = 0; i < n; i++) {
            for (i32 j = i + 1; j < n; j++) {
                for (i32 k = j + 1; k < n; k++) {
                    checkTriangle(X[i], X[j], X[k], result);
                }
            }
        }
    } else {
        i32 leftCount = n / 2;
        i32 rightCount = n - leftCount;
        i32 leftIdx = yEndOffset;
        i32 rightIdx = yEndOffset+leftCount;
        for (i32 i = 0; i < n; i++) {
            if (leftIdx < yEndOffset+leftCount && Y[i].x <= X[leftCount-1].x) {
                Y[leftIdx].x = Y[i].x;
                Y[leftIdx].y = Y[i].y;
                leftIdx++;
            } else {
                Y[rightIdx].x = Y[i].x;
                Y[rightIdx].y = Y[i].y;
                rightIdx++;
            }
        }

        _findMinCircTriangle(X, Y+yEndOffset, leftCount, n, result);
        _findMinCircTriangle(X+leftCount, Y+yEndOffset+leftCount, rightCount, rightCount, result);

        f64 x = X[leftCount-1].x + ((f64)(X[leftCount].x - X[leftCount-1].x) / 2);
        i32 leftBound = floor(x - (result->minCirc / 2));
        i32 rightBound = ceil(x + (result->minCirc / 2));

        i32 stripIdx = yEndOffset;
        for (i32 i = 0; i < n; i++) {
            if (leftBound <= Y[i].x && Y[i].x <= rightBound) {
                Y[stripIdx].x = Y[i].x;
                Y[stripIdx].y = Y[i].y;
                stripIdx++;
            }
        }

        for (i32 i = yEndOffset; i < stripIdx; i++) {
            // not sure what lookahead is correct; I found 16 in a StackOverflow
            // response. It shouldn't matter much in terms of performance if we
            // check a few triangles extra. Would be nice to know the real number
            // and a test to validate it.
            for (i32 j = i + 1; j < stripIdx && j < i + 16; j++) {
                for (i32 k = j + 1; k < stripIdx && k < i + 16; k++) {
                    checkTriangle(Y[i], Y[j], Y[k], result);
                }
            } 
        }
    }
}

Result findMinCircTriangle(Point *X, Point *Y, i32 n) {
    qsort(X, n, sizeof(Point), compPointsByX);
    memcpy(Y, X, n*sizeof(Point));
    qsort(Y, n, sizeof(Point), compPointsByY);
    i64 minCirc = LLONG_MAX;
    Result result = { .minCircSq = LLONG_MAX };
    _findMinCircTriangle(X, Y, n, n, &result);
    return result;
}

int main() {
    i32 n;
    static Point X[MAX_N];
    static Point Y[3*MAX_N];
    readInput(&n, X);
    Result result = findMinCircTriangle(X, Y, n);
    printPoints(result.points, 3);
    return 0;
}
