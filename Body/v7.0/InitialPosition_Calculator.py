import math
import numpy

def derivative(f1, f2, f3, q1, q2, q3, x, y, z, h):
    return numpy.matrix([[((f1(q1+h, q2, q3, x) - f1(q1, q2, q3, x)) / h), ((f1(q1, q2+h, q3, x) - f1(q1, q2, q3, x)) / h) , ((f1(q1, q2, q3+h, x) - f1(q1, q2, q3, x)) / h)],[((f2(q1+h, q2, q3, x) - f2(q1, q2, q3, x)) / h), ((f2(q1, q2+h, q3, x) - f2(q1, q2, q3, x)) / h) , ((f2(q1, q2, q3+h, x) - f2(q1, q2, q3, x)) / h)], [((f3(q1+h, q2, q3, x) - f3(q1, q2, q3, x)) / h), ((f3(q1, q2+h, q3, x) - f3(q1, q2, q3, x)) / h) , ((f3(q1, q2, q3+h, x) - f3(q1, q2, q3, x)) / h)]])

def eqnX(q1, q2, q3, x):
    return -0.305*math.sin(q1)*math.cos(q2-q3)-0.255*math.sin(q1)*math.cos(q2)-0.140*math.sin(q1)-x

def eqnY(q1, q2, q3, y):
    return -0.305*math.sin(q2-q3)-0.255*math.sin(q2)-y

def eqnZ(q1, q2, q3, z):
    return -0.305*math.cos(q1)*math.cos(q2-q3)-0.255*math.cos(q1)*math.cos(q2)-0.140*math.cos(q1)-z

def solve(funcX, funcY, funcZ, q1, q2, q3, x, y, z, h):
    lastQ = numpy.matrix([[q1], [q2], [q3]])
    nextQ = lastQ + 10* h

    error = abs(nextQ - lastQ)
    
    while (error.item(0) > h and error.item(1)>h and error.item(2)>h ):
        
        newEQN = numpy.matrix([[funcX(lastQ.item(0), lastQ.item(1), lastQ.item(2), x)], [funcY(lastQ.item(0), lastQ.item(1), lastQ.item(2), y)], [funcZ(lastQ.item(0), lastQ.item(1), lastQ.item(2), z)]])
        
        deriva = derivative(funcX, funcY, funcZ, lastQ.item(0), lastQ.item(1), lastQ.item(1), x, y, z, h)
        
        nextQ = lastQ - deriva.getI()*newEQN

        error = abs(nextQ - lastQ)

        lastQ = nextQ
    else:
        return nextQ.item(0), nextQ.item(1), nextQ.item(2)

def invKinematic(x, y, z, q1, q2, q3):
    newQ1, newQ2, newQ3 = solve(eqnX, eqnY, eqnZ, q1, q2, q3, x, y, z, 0.01)
    return newQ1, newQ2, newQ3


x = float(raw_input("x : "))
y = float(raw_input("y : "))
z = float(raw_input("z : "))
q1 = float(raw_input("q1 : "))
q2 = float(raw_input("q2 : "))
q3 = float(raw_input("q3 : "))

print invKinematic(x, y, z, q1, q2, q3)

