u = {} #dict()
u["Mike"] = 159831231
u["John"] = 259831231
u["Jane"] = 552322231

r = u["Mike"]
print(r)

if "Mike" in u:
    print(u["Mike"])

if "Leon" in u:
    print(u["Leon"])

for j in u:
    print(j)
    print(u[j])


abcd = "LastName"
efgh = "Mike"
z = {
        abcd:"Tutberidze",
        "FirstName":efgh,
        "Phone":1122334455
    }
print(z)
