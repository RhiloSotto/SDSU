
import sqlite3

con = sqlite3.Connection("C:\\SWSetup\\MyUniversity.db")

cur = con.cursor()

cur.execute("SELECT * FROM Grades")

r = cur.fetchone()

while r:
    if r[3] > 91:
        print(r)
    r = cur.fetchone()

cur.close()

con.close()
    
