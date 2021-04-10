#!/usr/bin/python3
import time
def sum(num):
    _sum = 0
    for i in range(0,num):
        _sum = _sum + i
    return _sum

def fib(x):
    if x == 0:
        return 0
    if x == 1:
        return 1
    if x == 2:
        return 1
    return fib(x-1) + fib(x-2)

if __name__ == '__main__':
    start=time.time()
    print(fib(32))
    end=time.time()
    print('Running time: %s Seconds'%(end-start))