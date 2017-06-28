# Graphs-Djikstras-Algorithm
Implementation of a graph and adjacency matrix used to compute the shortest path between a list of restaurants (using Djikstra's Algorithm). The final path taken is the one where the most possible nodes are visited, without visiting any one node twice.

1. XML file is read in, which contains a list of restaurants.
2. XML file is parsed to put each restaurant's data into a vector of strings.
3. String vector is parsed into a vector of Restaurant objects.
4. Adjacency list is built as an unordered_map, based on project specifications.
5. The distance between each restaurant is calculated with the Haversine formula, using the restaurant's longitude and latitude.
6. Distance is inserted into the adjacency matrix.
7. Using Djikstra's Algorithm, the shortest path for all restaurants is calculated and stored in a vector.
8. For this project, the max number of total restaurants visited is selected as the result. The data for each restaurant in this path is printed to the console (in colors).

Sample output:

```Opening input file: Restaurants.XML

Parsing XML file...
Building restaurant list...
Building adjacency list...
 0: 3 5 7 10 12 13 14
 1: 2 7 11
 2: 1 5 6 8 13
 3: 0 10 12 14
 4: 7 11 12
 5: 0 2 6 13 14
 6: 2 8 13
 7: 0 1 3 4 11 12
 8: 2 6 9 12 13
 9: 8 13
10: 0 3 12 13 14
11: 1 4 7 12 13 14
12: 0 3 7 10 14
13: 0 2 5 6 10 14
14: 0 3 5 10 12 13
15: 6 8 13 16 17 18
16: 8 9 15 17 18
17: 8 9 15 16 18
18: 8 9 15 16 17
Building adjacency matrix...
Printing Matrix:
     0     0     0  2.36     0  4.72     0  5.79     0     0  2.65     0  3.85  4.87  2.36     0     0     0     0
     0     0  5.34     0     0     0     0  4.84     0     0     0  2.15     0     0     0     0     0     0     0
     0  5.34     0     0     0  4.19  2.91     0  4.87     0     0     0     0  5.69     0     0     0     0     0
  2.36     0     0     0     0     0     0     0     0     0  1.33     0  3.15     0  2.99     0     0     0     0
     0     0     0     0     0     0     0  4.13     0     0     0  5.13   6.3     0     0     0     0     0     0
  4.72     0  4.19     0     0     0  2.36     0     0     0     0     0     0  1.83  5.69     0     0     0     0
     0     0  2.91     0     0     0     0     0  4.72     0     0     0     0  3.14     0     0     0     0     0
  5.79  4.84     0  6.26  4.13     0     0     0     0     0     0  4.19  3.46     0     0     0     0     0     0
     0     0  4.87     0     0     0  4.72     0     0  7.46     0     0  14.4  7.51     0     0     0     0     0
     0     0     0     0     0     0     0     0  7.46     0     0     0     0  9.74     0     0     0     0     0
  2.65     0     0  1.33     0     0     0     0     0     0     0     0  4.47  6.83  2.07     0     0     0     0
     0  2.15     0     0  5.13     0     0  4.19     0     0     0     0  7.64  10.5  11.5     0     0     0     0
  3.85     0     0  3.15     0     0     0  3.46     0     0  4.47     0     0     0  5.73     0     0     0     0
  4.87     0  5.69     0     0  1.83  3.14     0     0     0  6.83     0     0     0  5.02     0     0     0     0
  2.36     0     0  2.99     0  5.69     0     0     0     0  2.07     0  5.73  5.02     0     0     0     0     0
     0     0     0     0     0     0   316     0   313     0     0     0     0   316     0     0  24.5  11.2  14.9
     0     0     0     0     0     0     0     0   321   316     0     0     0     0     0  24.5     0    18  9.78
     0     0     0     0     0     0     0     0   323   317     0     0     0     0     0  11.2    18     0  9.07
     0     0     0     0     0     0     0     0   319   313     0     0     0     0     0  14.9  9.78  9.07     0
Calculating shortest path...

The shortest path to the most restaurants is the path traveled to restaurant 9:
0 3 14 10 12 5 13 7 6 2 4 11 1 8 9

Restaurant 0:
2235 Wyoming NE
Albuquerque, NM
(505)292-3865
35.1 -106.55
Traveling to restaurant 3, 2.3613 miles away.
Total distance traveled: 0

Restaurant 3:
425 Eubank NE. Albuquerque
Albuquerque, NM
(505)323-5552
35.07 -106.53
Traveling to restaurant 14, 4.7221 miles away.
Total distance traveled: 2.3613

Restaurant 14:
2809 Juan Tabo Blvd. NE
Albuquerque, NM
(505)296-1575
35.11 -106.51
Traveling to restaurant 10, 5.7912 miles away.
Total distance traveled: 7.0834

Restaurant 10:
740 Juan Tabo Blvd. NE
Albuquerque, NM
(505)237-1611
35.08 -106.51
Traveling to restaurant 12, 2.6504 miles away.
Total distance traveled: 12.875

Restaurant 12:
5210 Gibson SE
Albuquerque, NM
(505)232-6866
35.05 -106.58
Traveling to restaurant 5, 3.8489 miles away.
Total distance traveled: 15.525

Restaurant 5:
7600 Jefferson NE
Albuquerque, NM
(505)821-6184
35.16 -106.59
Traveling to restaurant 13, 4.8697 miles away.
Total distance traveled: 19.374

Restaurant 13:
8111 Wyoming NE
Albuquerque, NM
(505)856-2162
35.17 -106.56
Traveling to restaurant 7, 2.3644 miles away.
Total distance traveled: 24.244

Restaurant 7:
2435 Southern Blvd
Rio Rancho, NM
(505)892-4121
35.06 -106.64
Traveling to restaurant 6, 4.1884 miles away.
Total distance traveled: 26.608

Restaurant 6:
2301 Menaul Blvd. NE
Albuquerque, NM
(505)884-1828
35.19 -106.61
Traveling to restaurant 2, 2.3607 miles away.
Total distance traveled: 30.796

Restaurant 2:
9358 Eagle Ranch Rd. NW
Albuquerque, NM
(505)890-5229
35.18 -106.66
Traveling to restaurant 4, 4.8421 miles away.
Total distance traveled: 33.157

Restaurant 4:
4257 Isleta Blvd. SW
Albuquerque, NM
(505)877-2727
35.01 -106.68
Traveling to restaurant 11, 4.1301 miles away.
Total distance traveled: 37.999

Restaurant 11:
301 Coors Blvd. NW
Albuquerque, NM
(505)839-2999
35.08 -106.71
Traveling to restaurant 1, 4.193 miles away.
Total distance traveled: 42.129

Restaurant 1:
3240 Coors NW
Albuquerque, NM
(505)839-6966
35.11 -106.7
Traveling to restaurant 8, 4.7209 miles away.
Total distance traveled: 46.322

Restaurant 8:
4101 Ridge Rock Rd. SE
Rio Rancho, NM
(505)892-6384
35.25 -106.65
Traveling to restaurant 9, 7.4624 miles away.
Total distance traveled: 51.043

Restaurant 9:
130 Hwy. 550
East Bernalillo, NM
(505)867-9110
35.31 -106.54
Total distance traveled: 58.506
```
