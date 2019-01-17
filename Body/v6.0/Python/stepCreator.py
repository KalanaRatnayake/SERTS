position = input("enter the position arry : ")
print position
time = input("Enter the time array : ")
print time
sampletime = input("Enter Frequency : ")
print sampletime

newPositionArray = []
newPositionArray.append(position[0])
for i in range(0,len(position)-1):
    distanceGap = position[i+1] - position[i]        
    timeGap = time[i+1] - time[i]
    
    noOfPositions = timeGap/sampletime
    
    if (distanceGap==0):
        newPositionArray.extend([position[i]]*int(noOfPositions))
        continue
        
    gain = distanceGap/noOfPositions
    
    newValue = position[i] + gain

    if (distanceGap>0):
        while (newValue<=position[i+1]):
            newPositionArray.append(newValue)
            newValue = newValue + gain
    else:
        while (newValue>=position[i+1]):
            newPositionArray.append(newValue)
            newValue = newValue + gain

else:
    newPositionArray.append(position[-1])
    print newPositionArray, len(newPositionArray)
