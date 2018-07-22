#!/usr/bin/env python

"""
This program takes the file containing the prime numbers and parses them
to generate the file numbers.h, which contains a definition of a vector with primes.
"""

in_filename = "primes-to-100k.txt"
out_filename = "numbers.h"
# the maximum number to find
num_limit = 255

# output string to generate the source code
out = "int primes[] = {"

with open(in_filename) as f:
    line = f.readline()
    while line:
        number = int(line)
        if (number > num_limit):
            break
        print (line)
        out = out + line.rstrip() + ", " + "\n"
        line = f.readline()

# remove last comma and "enter" chars from out, and add the closing brackets
out = out[:-3]
out = out + "};"

# write out to the output file, overwrite mode
fw = open(out_filename, "w")
fw.write(out)
print ("Output saved to " + out_filename)
