def f(x):
    if x > 10:
        g(x-2)
    print(x)


# calling f here does not work though
'''f(19)'''

def g(x):
    if x > 10:
        f(x-3)
    print(x)

# we can call functions no matter the order of definition
# f is defined before g, but f can still call g
# these are recursive

# invoking f must occur after the f function and all
# functions within that function (g) are declared
f(20)
