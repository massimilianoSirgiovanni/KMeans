import numpy as np
import time

from joblib import Parallel, delayed  # use loky backend
from joblib import Memory

hello_sleep = 0.5


def hello(i: int):
    time.sleep(hello_sleep)
    print(f'Hello {i}')


hello_threads = 10
start_time = time.time()
Parallel(n_jobs=2)(delayed(hello)(i) for i in range(hello_threads))
end_time = time.time()
print(
    f'Elapsed time for loop parallel processing: {end_time - start_time:.3f} s instead of {hello_sleep * hello_threads} s')

start_time = time.time()
accumulator = 0.
n_iter = 0
while accumulator < 1000:
    results = Parallel(n_jobs=2)(delayed(np.sqrt)(accumulator + i ** 2) for i in range(5))
    accumulator += sum(results)  # synchronization barrier
    n_iter += 1
end_time = time.time()
print(f'Elapsed time for loop parallel processing: {end_time - start_time:.2f} s')

start_time = time.time()
with Parallel(n_jobs=2) as parallel:
    accumulator = 0.
    n_iter = 0
    while accumulator < 1000:
        results = parallel(delayed(np.sqrt)(accumulator + i ** 2) for i in range(5))
        accumulator += sum(results)  # synchronization barrier
        n_iter += 1
end_time = time.time()
print(f'Elapsed time for loop parallel processing pool reuse (with): {end_time - start_time:.2f} s')

rng = np.random.RandomState(42)
data = rng.randn(int(1e4), 4)


def costly_compute(data, column):
    """Emulate a costly function by sleeping and returning a column."""
    time.sleep(2)
    return data[column]


def data_processing_mean(data, column):
    """Compute the mean of a column."""
    return costly_compute(data, column).mean()


start_time = time.time()
results = [data_processing_mean(data, col) for col in range(data.shape[1])]
end_time = time.time()
print(f'Elapsed time for the sequential processing: {end_time - start_time:.2f} s')

location = '/tmp/joblib_example_parallel'
memory = Memory(location, verbose=0)
costly_compute_cached = memory.cache(costly_compute)

# Clean-up the cache folder
# XXX be careful you may destroy results that may be useful for your experiments !!!
memory.clear(warn=False)


def data_processing_mean_using_cache(data, column):
    """Compute the mean of a column."""
    return costly_compute_cached(data, column).mean()


start_time = time.time()
results = Parallel(n_jobs=4)(
    delayed(data_processing_mean_using_cache)(data, col)
    for col in range(data.shape[1]))
end_time = time.time()
print(f'Elapsed time for the parallel processing: {end_time - start_time:.2f} s')

start_time = time.time()
results = Parallel(n_jobs=4)(
    delayed(data_processing_mean_using_cache)(data, col)
    for col in range(data.shape[1]))
end_time = time.time()
print(f'Elapsed time for the cached parallel processing: {end_time - start_time:.2f} s')

# shared data structure
shared_set = set()


def collect(x):
    shared_set.add(x)  # shared data structure access


# shared memory access: require='sharedmem'
Parallel(n_jobs=2, require='sharedmem')(delayed(collect)(i) for i in range(5))
print(shared_set)
