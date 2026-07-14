# Green Efficiency Analysis

## Measurement Methodology

I compiled all three programs with the flags from the project:
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 -Wno-long-long

For the timing I used clock() from time.h. clock() does not give seconds, it
gives processor ticks. So I divided the result by CLOCKS_PER_SEC to change it
into seconds. I ran baseline_loop and comparison_algorithms three times each.
I ran them one after the other in the same terminal, so the machine was the
same for all runs.

The instrumentation lab was the only file I changed. I took four clock()
readings: before build, after build, after process, and after reduce. Each
phase time is the difference between two readings next to each other. I chose
this way because one reading works as the end of one phase and the start of the
next one. This means no time is lost between phases, and TOTAL always covers
all of them.

## Observed Performance Differences

baseline_loop was very stable. The three runs were 0.228669 s, 0.228164 s and
0.226706 s for 100 million iterations. The difference between the runs is less
than 2 ms, so I think the timing method is reliable.

The comparison gave the biggest difference. The naive version took 2.759290 s,
2.761769 s and 2.755006 s. The average is about 2.758688 s. The single-pass
version took 0.000106 s in all three runs. This means the naive version is
about 26,000 times slower on the same data.

The two versions also print different counts (625,025,000 and 25,000). The
naive one counts the same even numbers again in every nested pass. So it does
a lot of extra work and it does not even give the correct count directly.

In the instrumentation lab I got these times: TOTAL 0.000737 s, BUILD_DATA
0.000286 s, PROCESS 0.000316 s, REDUCE 0.000135 s. PROCESS was the slowest
phase and REDUCE was the fastest. The three phases add up to TOTAL, so my
measurement boundaries were correct.

## Relation Between Runtime and Energy Consumption

Runtime shows how long the CPU is busy. A busy CPU uses more power than an
idle CPU. The naive algorithm runs for 2.758688 s and the single-pass runs for
0.000106 s. So the naive one keeps the CPU working about 26,000 times longer,
and the result is not better.

The reason is the complexity. The naive version has nested loops. For 50,000
elements it does about n^2 / 2 = 1.25 billion operations, so it is O(n^2). The
single-pass version does only 50,000 operations, so it is O(n). If the data
becomes 10 times bigger, the O(n) version does 10 times more work, but the
O(n^2) version does about 100 times more. The waste grows much faster than
the data.

## Limitations of the Experiment

The main limitation is that I did not measure energy. I measured time and used
it as a substitute. To measure real watts I would need special tools. So what
I say about energy is a conclusion, not something I saw.

There are other limitations too. clock() measures CPU time, and other processes
on the sandbox can affect it. Three runs is a small sample. I also tested only
one input size, so I saw the difference at n = 50,000 but I did not test how it
changes with other sizes.

## Practical Engineering Takeaway

The most important thing I learned is that the algorithm is more important than
small changes in the code. The difference here was very large, and cleaning up
the naive version would not fix it.

The second thing is to measure before optimising. I did not know PROCESS was the
slowest phase until I measured it. Guessing can waste time on the wrong part.

Also, efficiency is not only about speed. CPU time is connected to energy, so
writing efficient code is also good for the environment. At a big scale this
becomes important.
