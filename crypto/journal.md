14 4 13 1 2 15 11 8 3 10 6 12 5 9 0 7
0 15 7 4 14 2 13 1 10 6 12 11 9 5 3 8
4 1 14 8 13 6 2 11 15 12 9 7 3 10 5 0
15 12 8 2 4 9 1 7 5 11 3 14 10 0 6 13

15 1 8 14 6 11 3 4 9 7 2 13 12 0 5 10
3 13 4 7 15 2 8 14 12 0 1 10 6 9 11 5
0 14 7 11 10 4 13 1 5 8 12 6 9 3 2 15
13 8 10 1 3 15 4 2 11 6 7 12 0 5 14 9

Mar 29

I have understood how mask generation works. The website gave me an intuitive understanding of butterly circuits. The book hinted at an algorithm for general bit permutations, but there was no connection. I couldn't understand how to incorperate them until I read the code. Deciphering the code was the missing puzzle piece. Now I can work on implementing my own version of mask g generation which will likely be slower but surely more comprehensible. 

Mar 30

I have now implemented three version of general bit permutation algorithms. One 2 line version in python, one simple version in C, and finally, the fast mask-permutation C algorithm. I have benchmarked all these codes. The benchmark computes the same 1000 random permutations on the same 10000 randomly generated inputs across all three functions. The results are as follows with -O3 optimization:

Python Results:
---------------
Total seconds:	 55.606396436691284
Seconds / Call:	 5.560639643669129e-06
Sanity Check:	 0xa24ea601b8dd66e9

Dumb C Results:
-----------------
Ticks elapsed: 	104646835474
Total seconds: 	29.068298950037402
Seconds / Call: 2.9068298950037401e-06
Sanity Check: 	0xa24ea601b8dd66e9

Fast C Results:
-----------------
Ticks elapsed: 	495474170
Total seconds: 	0.13763045227640969
Seconds / Call: 1.3763045227640968e-08
Sanity Check: 	0xa24ea601b8dd66e9


Speed-up Factor Results
-----------------------
Dumb C vs Dumb Python	 1.91
Fast C vs Dumb C	     211.21
Fast C vs Dumb Python	 404.03

Apr 7

I have finally found the time to finish implmeneted DES. Now I just need to test it. 
