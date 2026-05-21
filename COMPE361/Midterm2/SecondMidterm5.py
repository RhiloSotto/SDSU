import sqlite3

con = sqlite3.Connection("C:\\SWSetup\\Shop.db")

cur = con.cursor()
cur.execute("SELECT * FROM Sales")

row = cur.fetchone()
while row:
    cost = row[2] * row[3]
    print (row[1] + ' ' + row[4] + ' ' + str(cost))
    row = cur.fetchone()

con.close()
