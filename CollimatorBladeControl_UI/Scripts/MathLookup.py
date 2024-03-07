from math import *

sin_lookup_table = []
cos_lookup_table = []

for i in range(256):
    curr_radian = (i / 256) * 2 * pi
    sin_lookup_table.append(sin(curr_radian))
    cos_lookup_table.append(cos(curr_radian))

print(sin_lookup_table)

