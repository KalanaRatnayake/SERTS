import cv2
import numpy as np

REMAP_INTERPOLATION = cv2.INTER_LINEAR

DEPTH_VISUALIZATION_SCALE = 1

calibration = np.load("outputFile.npz", allow_pickle=False)
leftCameraMatrix = calibration["leftCameraMatrix"]
leftDistortionCoefficients = calibration["leftDistortionCoefficients"]
newLeftCamMatrix = calibration["newLeftCamMatrix"]
leftROI = tuple(calibration["leftROI"])
rightCameraMatrix = calibration["rightCameraMatrix"]
rightDistortionCoefficients = calibration["rightDistortionCoefficients"]
newRightCamMatrix = calibration["newRightCamMatrix"]
rightROI = tuple(calibration["rightROI"])

CAMERA_WIDTH = 640
CAMERA_HEIGHT = 360

left = cv2.VideoCapture(1)
right = cv2.VideoCapture(0)

left.set(cv2.CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH)
left.set(cv2.CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT)
right.set(cv2.CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH)
right.set(cv2.CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT)


# SGBM Parameters -----------------
window_size = 3                 
left_matcher = cv2.StereoSGBM_create(
    minDisparity=8,
    numDisparities=48,
    blockSize=5,
    P1=8 * 3 * window_size ** 2,
    P2=32 * 3 * window_size ** 2,
    disp12MaxDiff=1,
    uniquenessRatio=15,
    speckleWindowSize=0,
    speckleRange=2,
    preFilterCap=63,
    mode=cv2.STEREO_SGBM_MODE_SGBM_3WAY
)

right_matcher = cv2.ximgproc.createRightMatcher(left_matcher)

# FILTER Parameters
lmbda = 80000
sigma = 1.2
visual_multiplier = 1.0

wls_filter = cv2.ximgproc.createDisparityWLSFilter(matcher_left=left_matcher)
wls_filter.setLambda(lmbda)
wls_filter.setSigmaColor(sigma)


while(True):
    if not (left.grab() and right.grab()):
        print("No more frames")
        break
    _, leftFrame = left.retrieve()
    _, rightFrame = right.retrieve()

    fixedLeft = cv2.undistort(leftFrame, leftCameraMatrix, leftDistortionCoefficients, None, newLeftCamMatrix)
    fixedRight = cv2.undistort(rightFrame, rightCameraMatrix, rightDistortionCoefficients, None, newRightCamMatrix)
    
    displ = left_matcher.compute(fixedLeft, fixedRight)  #.astype(np.float32)/16
    dispr = right_matcher.compute(fixedRight, fixedLeft) # .astype(np.float32)/16
    displ = np.int16(displ)
    dispr = np.int16(dispr)
    filteredImg = wls_filter.filter(displ, fixedLeft, None, dispr)  # important to put "imgL" here!!!

    filteredImg = cv2.normalize(src=filteredImg, dst=filteredImg, beta=0, alpha=255, norm_type=cv2.NORM_MINMAX);
    filteredImg = np.uint8(filteredImg)
    filteredImg=cv2.applyColorMap(filteredImg, cv2.COLORMAP_JET)
    cv2.imshow('Disparity Map', filteredImg)
    cv2.imshow('leftFrame', fixedLeft)
    cv2.imshow('rightFrame', fixedRight)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cv2.destroyAllWindows()
