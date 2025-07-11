import matplotlib.pyplot as plt

# signal generation 
pattern = [1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0]
reuslt = []
offset = -1*4*4

class Pad:
    def __init__(self, offset, value):
        self.offset = offset
        self.value = value

#description of the exitation pads
ecectrodeSpaceing = 0.2
ecectrodeWidth = 0.8
numberOfElectrodes = 8 # we would not want to change this 
electrodes = []      
electrodes.append(Pad(0, 0)) #A
electrodes.append(Pad(1, 0)) #B
electrodes.append(Pad(2, 0)) #C
electrodes.append(Pad(3, 0)) #D
electrodes.append(Pad(4, 0)) #A`
electrodes.append(Pad(5, 0)) #B`
electrodes.append(Pad(6, 0)) #C`
electrodes.append(Pad(7, 0)) #D`

#description of the sensor pads
sensorMovement = 4.01
sensorWidth = 3.6
sensors = []
sensors.append(Pad(0, 0))
sensors.append(Pad(0, 0))
sensors.append(Pad(0, 0))
sensors.append(Pad(0, 0))
sensors.append(Pad(0, 0))
sensors.append(Pad(0, 0))
sensors.append(Pad(0, 0))

# these are for the graph 
Ax = []
Ay = []
Bx = []
By = []
Cx = []
Cy = []
Dx = []
Dy = []
nAx = []
nAy = []
nBx = []
nBy = []
nCx = []
nCy = []
nDx = []
nDy = []

patternLen = len(pattern)

for index, value in enumerate(pattern):
    AVal = 0
    BVal = 0
    CVal = 0
    Dval = 0
    nAVal = 0
    nBVal = 0
    nCVal = 0
    nDval = 0
    
    # ------- A ------- #
    Ax.append(index)
    Ax.append(index+1)
    AVal = value
    Ay.append(AVal)
    Ay.append(AVal)
    electrodes[0].value = AVal

    nAx.append(index)
    nAx.append(index+1)
    nAVal = (1 if value == 0 else 0)
    nAy.append(nAVal - 6)
    nAy.append(nAVal - 6)
    electrodes[4].value = nAVal

    # ------- B ------- #
    Bx.append(index)
    Bx.append(index+1)
    idx = index+(offset)
    if idx >= patternLen :
        idx = idx - patternLen
    BVal = pattern[idx]
    By.append(BVal - 1.5)
    By.append(BVal - 1.5)
    electrodes[1].value = BVal

    nBx.append(index)
    nBx.append(index+1)
    nBVal = (1 if BVal == 0 else 0)
    nBy.append(nBVal - 7.5)
    nBy.append(nBVal - 7.5)
    electrodes[5].value = nBVal
    
    # ------- C ------- #
    Cx.append(index)
    Cx.append(index+1)
    idx = index+(offset*2)
    if idx >= patternLen :
        idx = idx - patternLen
    CVal = pattern[idx]
    Cy.append(CVal - 3)
    Cy.append(CVal - 3)
    electrodes[2].value = CVal

    nCx.append(index)
    nCx.append(index+1)
    nCVal = (1 if CVal == 0 else 0)
    nCy.append(nCVal - 9)
    nCy.append(nCVal - 9)
    electrodes[6].value = nCVal
    
    # ------- D ------- #
    Dx.append(index)
    Dx.append(index+1)
    idx = index+(offset*3)
    if idx >= patternLen :
        idx = idx - patternLen
    DVal = pattern[idx]
    Dy.append(DVal - 4.5)
    Dy.append(DVal - 4.5)
    electrodes[3].value = DVal

    nDx.append(index)
    nDx.append(index+1)
    nDVal = (1 if DVal == 0 else 0)
    nDy.append(nDVal - 10.5)
    nDy.append(nDVal - 10.5)
    electrodes[7].value = nDVal

    # go through all of the sensor pads and calculate where each start and ends, also zero out all the values
    for sensor in sensors:
        startSensor = sensor.offset + sensorMovement
        endSensor = startSensor + sensorWidth
        sensor.value = 0

        # calculate the the start and end positions of the exitation pads
        for padIndex, pad in enumerate(electrodes):            
            startPad = pad.offset * (ecectrodeSpaceing + ecectrodeWidth)
            endPad = startPad + ecectrodeWidth

            # calculate the overlap between exitation pads and sensor pads
            coefficient = 0
            
            if startPad < startSensor and endPad > startSensor:
                coefficient = (endPad - startSensor) / ecectrodeWidth
                
            if startPad >= startSensor and endPad < endSensor:
                coefficient = 1

            if startPad > startSensor and endPad <= endSensor:
                coefficient = 1

            if startPad < endSensor and endPad > endSensor:
                coefficient = (endSensor - startPad) / ecectrodeWidth

            sensor.value = sensor.value + (pad.value * coefficient)

    # sum all of the values from the sensor pads 
    tmpVal = 0
    
    for sensor in sensors:
        tmpVal = tmpVal + sensor.value
        
    reuslt.append(tmpVal)

# draw the result of the calculation            
Rx = []
Ry = []

for index, value in enumerate(reuslt):
    Rx.append(index)
    Rx.append(index+1)
    Ry.append(value + 5)
    Ry.append(value + 5)
    

# Create a line chart
# plt.plot(x, y, label='result', marker='o')

plt.figure(figsize=(24, 6))

plt.plot(Ax, Ay, label='A', marker='')
plt.plot(Bx, By, label='B', marker='')
plt.plot(Cx, Cy, label='C', marker='')
plt.plot(Dx, Dy, label='D', marker='')
plt.plot(nAx, nAy, label='nA', marker='')
plt.plot(nBx, nBy, label='nB', marker='')
plt.plot(nCx, nCy, label='nC', marker='')
plt.plot(nDx, nDy, label='nD', marker='')
plt.plot(Rx, Ry, label='result', marker='')

# Add titles and labels
plt.title('Resulting signal Chart')
plt.xlabel('Time')
plt.ylabel('Voltage')

# Show legend
plt.legend()

# Display the chart
plt.show()
