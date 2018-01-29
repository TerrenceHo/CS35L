After learning how multi-threaded should be used, I split the ray-tracing code
from the main function over to it's separate function, then generated threads
for that function.

However, these threads would make N copies of the image being traced and not
share the work.  So I had to modify the function to be aware of threads and
divide up work by the number of threads.  I accomplished this by giving each
thread it's own row to work with, then incrementing by the number of threads, so
that work is divided evenly among threads.

While this would form an image and run, it did not print correctly.  Because
threads are not sequential, the threads would write to disk in a random order
and destroy the image.  We needed to store the image values in a array of size
[height][width][3], where 3 is the number of color channels.  Once the threads
stored the color values into the array, we printed it out.  So we were only able
to parallelize the image calculations, rather than the actual image generation
itself.

As for the performance of srt, threading improves it's performance greatly.
Doubling the number of threads nearly halves the time take to run the ray
tracing code.  However, this means that adding more threads adds less
performance gain per thread, so after a number of threads the performance gain
is no longer worth with respect to the thread switching needed, once you get to
a very high number of threads.
