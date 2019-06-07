import numpy as np
import cv2

REMAP_INTERPOLATION = cv2.INTER_LINEAR

DEPTH_VISUALIZATION_SCALE = 1

calibration = np.load("outputFile2.npz", allow_pickle=False)
imageSize = calibration["imageSize"]
leftMapX = calibration["leftMapX"]
leftMapY = calibration["leftMapY"]
rightMapX = calibration["rightMapX"]
rightMapY = calibration["rightMapY"]
Q = calibration["disparityToDepthMap"]

CAMERA_WIDTH = 640
CAMERA_HEIGHT = 360
FLOOR_THRESHOLD = 30.0

left = cv2.VideoCapture(0)
right = cv2.VideoCapture(1)

left.set(cv2.CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH)
left.set(cv2.CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT)
right.set(cv2.CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH)
right.set(cv2.CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT)

window_size = 3
left_matcher = cv2.StereoBM_create(16, 15)

right_matcher = cv2.ximgproc.createRightMatcher(left_matcher)

# FILTER Parameters
lmbda = 80000
sigma = 1.2
visual_multiplier = 1.0

wls_filter = cv2.ximgproc.createDisparityWLSFilter(matcher_left=left_matcher)
wls_filter.setLambda(lmbda)
wls_filter.setSigmaColor(sigma)

def clusterRate(disparityMap, threshold):
      mask = disparityMap>threshold
      value = np.count_nonzero(disparityMap[mask])
      return float(value/disparityMap.size)

def averageDisparity(disparityMap):
      return np.average(disparityMap)
    

def decision(disparityMap, rate, threshold):
      rows, cols = disparityMap.shape
      mapLeft = disparityMap[:, :int(cols*0.3)]
      mapMiddle = disparityMap[:, int(cols*0.3):int(cols*0.7)]
      mapRight = disparityMap[:, int(cols*0.7):int(cols*1)]
      
      avgLeft = averageDisparity(mapLeft)
      rateMiddle = clusterRate(mapMiddle, threshold)
      avgRight = averageDisparity(mapRight)
      
      if (rateMiddle<=rate):
            print "move forward"
      elif (avgLeft>avgRight):
            print "turn Right"
      else:
            print "turn Left"

            
while(True):
    if not left.grab() or not right.grab():
        print("No more frames")
        break

    _, leftFrame = left.retrieve()   
    _, rightFrame = right.retrieve()

    fixedLeft = cv2.remap(leftFrame, leftMapX, leftMapY, cv2.INTER_LANCZOS4)
    fixedRight = cv2.remap(rightFrame, rightMapX, rightMapY, cv2.INTER_LANCZOS4)

    grayLeft = cv2.cvtColor(fixedLeft, cv2.COLOR_BGR2GRAY)
    grayRight = cv2.cvtColor(fixedRight, cv2.COLOR_BGR2GRAY)
    
    displ = left_matcher.compute(grayLeft, grayRight)  #.astype(np.float32)/16
    displ = np.int16(displ)
    dispr = right_matcher.compute(grayRight, grayLeft) # .astype(np.float32)/16
    dispr = np.int16(dispr)
    
    filteredImg = wls_filter.filter(displ, fixedLeft, None, dispr)  # important to put "imgL" here!!!
    filteredImg = cv2.normalize(src=filteredImg, dst=filteredImg, beta=0, alpha=255, norm_type=cv2.NORM_MINMAX)
    filteredImg = np.uint8(filteredImg)

    decision(filteredImg, 0.20, 120)
    
    filteredImg=cv2.applyColorMap(filteredImg, cv2.COLORMAP_BONE)
                    
    
    cv2.imshow('leftFrame', fixedLeft)
    cv2.imshow('rightFrame', fixedRight)
    cv2.imshow('Disparity Map', filteredImg)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

left.release()
right.release()
cv2.destroyAllWindows()
