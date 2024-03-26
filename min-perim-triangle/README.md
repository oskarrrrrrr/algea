# Min Perimeter Triangle

## Problem

Given a list of n distinct points on a plane, find three points that form a triangle with the smallest perimeter. Note that three points on the same line are considered a triangle.

### Input

On the first line a single number, n, followed by n lines of coordinates xi and yi.

Constraints:
3 <= n <= 3 500 000
-10^7 <= xi, yi <= 10^7
max memory usage: 128MB

### Output

Output three lines containing the coordinates of a triangle with the smallest perimeter.

## Solution

The solution is very similar to the one for the problem of finding a pair of closest points on a plane. We recursively split the plane in half (in terms of the number of points, not area) until there are only a few points (less than 6) left, which are then solved by checking all possible pairs. At each recursive step, we check if there are any triangles that have points in both halves. This is done by examining triangles within a close range (half of the smallest perimeter found up to that point) around the middle point between the left and right halves. The time complexity of this approach is O(nlogn).

This solution avoids dynamic memory allocation by primarily utilizing a single array for extra space and reusing it efficiently.

## Running

To generate tests with larger values of n, run: `python gen_test.py`
Run `./test.sh` to run the tests.
