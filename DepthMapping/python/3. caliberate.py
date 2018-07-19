import glob
import os
import random
import sys

import numpy as np
import cv2

CHESSBOARD_SIZE = (7, 7)
CHESSBOARD_OPTIONS = (cv2.CALIB_CB_ADAPTIVE_THRESH |cv2.CALIB_CB_NORMALIZE_IMAGE | cv2.CALIB_CB_FAST_CHECK)
OBJECT_POINT_ZERO = np.zeros((CHESSBOARD_SIZE[0] * CHESSBOARD_SIZE[1], 3),np.float32)
OBJECT_POINT_ZERO[:, :2] = np.mgrid[0:CHESSBOARD_SIZE[0],0:CHESSBOARD_SIZE[1]].T.reshape(-1, 2)
OPTIMIZE_ALPHA = 0.25
TERMINATION_CRITERIA = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

width = 640
height = 360
leftImageDir = "capture/left"
rightImageDir = "capture/right"
outputFile = "outputFile"


def getMatchingObjectAndImagePoints(requestedFilenames, allFilenames, objectPoints, imagePoints):
    requestedFilenameSet = set(requestedFilenames)
    requestedObjectPoints = []
    requestedImagePoints = []

    for index, filename in enumerate(allFilenames):
        if filename in requestedFilenameSet:
            requestedObjectPoints.append(objectPoints[index])
            requestedImagePoints.append(imagePoints[index])

    return requestedObjectPoints, requestedImagePoints

def readImagesAndFindChessboards(imageDirectory):
    cacheFile = "{0}/chessboards.npz".format(imageDirectory)
    try:
        cache = np.load(cacheFile)
        print("Loading image data from cache file at {0}".format(cacheFile))
        return (list(cache["filenames"]), list(cache["objectPoints"]),list(cache["imagePoints"]), tuple(cache["imageSize"]))
    except IOError:
        print("Cache file at {0} not found".format(cacheFile))

    print("Reading images at {0}".format(imageDirectory))
    imagePaths = glob.glob("{0}/*.jpg".format(imageDirectory))

    filenames = []
    objectPoints = []
    imagePoints = []
    imageSize = None

    for imagePath in sorted(imagePaths):
        image = cv2.imread(imagePath)
        grayImage = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

        newSize = grayImage.shape[::-1]
        if imageSize != None and newSize != imageSize:
            raise ValueError("Calibration image at {0} is not the same size as the others".format(imagePath))
        imageSize = newSize

        hasCorners, corners = cv2.findChessboardCorners(grayImage, CHESSBOARD_SIZE, None)

        if hasCorners:
            filenames.append(os.path.basename(imagePath))
            objectPoints.append(OBJECT_POINT_ZERO)
            cv2.cornerSubPix(grayImage, corners, (11, 11), (-1, -1), TERMINATION_CRITERIA)
            imagePoints.append(corners)

            cv2.drawChessboardCorners(image, CHESSBOARD_SIZE, corners, hasCorners)
            cv2.imshow(imageDirectory, image)

        # Needed to draw the window
        cv2.waitKey(1)

    cv2.destroyWindow(imageDirectory)

    print("Found corners in {0} out of {1} images".format(len(imagePoints), len(imagePaths)))

    np.savez_compressed(cacheFile,filenames=filenames, objectPoints=objectPoints,imagePoints=imagePoints, imageSize=imageSize)
    return filenames, objectPoints, imagePoints, imageSize

(leftFilenames, leftObjectPoints, leftImagePoints, leftSize) = readImagesAndFindChessboards(leftImageDir)
(rightFilenames, rightObjectPoints, rightImagePoints, rightSize) = readImagesAndFindChessboards(rightImageDir)

imageSize = leftSize
filenames = list(set(leftFilenames) & set(rightFilenames))
filenames = sorted(filenames)

leftObjectPoints, leftImagePoints = getMatchingObjectAndImagePoints(filenames,leftFilenames, leftObjectPoints, leftImagePoints)
rightObjectPoints, rightImagePoints = getMatchingObjectAndImagePoints(filenames,rightFilenames, rightObjectPoints, rightImagePoints)

objectPoints = leftObjectPoints

print("Calibrating left camera...")
_, leftCameraMatrix, leftDistortionCoefficients, _, _ = cv2.calibrateCamera(objectPoints, leftImagePoints, imageSize, None, None)
newLeftCamMatrix, leftROI = cv2.getOptimalNewCameraMatrix(leftCameraMatrix, leftDistortionCoefficients, (width, height), 1, (width, height))

print("Calibrating right camera...")
_, rightCameraMatrix, rightDistortionCoefficients, _, _ = cv2.calibrateCamera(objectPoints, rightImagePoints, imageSize, None, None)
newRightCamMatrix, rightROI = cv2.getOptimalNewCameraMatrix(rightCameraMatrix, rightDistortionCoefficients, (width, height), 1, (width, height))

print("Saving calibration...")

np.savez_compressed(outputFile, imageSize = imageSize, leftCameraMatrix = leftCameraMatrix, leftDistortionCoefficients = leftDistortionCoefficients,
                    newLeftCamMatrix = newLeftCamMatrix, leftROI = leftROI, rightCameraMatrix = rightCameraMatrix, rightDistortionCoefficients = rightDistortionCoefficients,
                    newRightCamMatrix = newRightCamMatrix, rightROI = rightROI)

cv2.destroyAllWindows()
