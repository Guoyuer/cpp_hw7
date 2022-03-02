# cpp_hw7

## Resubmission Notes:

I think I solved all problems mentioned in early feedback.

#### P1:

1. result_of_t has been replaced with invoke_result_t
2. use auto for get_future()'s return type
3. use thread.detach(), instead of join()

#### P2

1. If the function throws an exception, the promise is set with the exception

#### P3

1. added future's wait() and exception handling.

## P4

My original answer:

```
I don't have much idea on this question. I guess when the type is simple, like an int, const T and T const don't matter.

When the type is complex, like int *a[5], the position of const may bring much ambiguity. Then we need to care about
east or west const.
```

Now I got it from the rubric:
```
The C++ Core Guidelines (NL.26) recommend it for “Conventional notation is more familiar to more programmers” and for
“Consistency in large code bases”
```

It's totally ok not to give me additional 1 point.