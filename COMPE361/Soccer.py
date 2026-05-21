p = input("Please enter the number of goals by first team: ")
q = input("Please enter the number of goals by second team: ")

p = int(p)
q = int(q)


r = "First team won" if p > q else "Second team won" if p < q else "Draw"
print(r)
