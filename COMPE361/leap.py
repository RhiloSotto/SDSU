u = int(input("Leap Year? "))

if u % 4 == 0:
    if u % 100 == 0 and u % 400 != 0:
        print("not leap")
    else:
        print("leap")
else:
    print("not leap")
