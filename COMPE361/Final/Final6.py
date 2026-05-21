import threading,time

def f():
    global n
    locker.acquire()
    k = n
    time.sleep(5)
    n = k + 1
    locker.release()

def g():
    global n
    locker.acquire()
    k = n
    time.sleep(5)
    n = k + 1
    locker.release()


n = 0
locker = threading.Lock()
th1 = threading.Thread(target = f)
th2 = threading.Thread(target = g)

th1.start()
th2.start()

th1.join()
th2.join()

print(n)
    
