/*
 * Copyright (C) 2020  Christian Berger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROCESS_IMAGE_HPP
#define PROCESS_IMAGE_HPP

#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cmath>


std::vector<std::vector<cv::Point>> findContours(const cv::Mat &mask)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    return contours;
}

double processContours(const cv::Mat &result, const std::vector<std::vector<cv::Point>> &contours, double &angle, int yOffset, const cv::Scalar &color)
{
    for (const auto &contour : contours)
    {
        cv::Rect rectangleBorder = cv::boundingRect(contour);
        if (rectangleBorder.area() > 100)
        { // Ensuring that small noises are disregarded
            rectangleBorder.y += yOffset;
            cv::rectangle(result, rectangleBorder, color, 3); // Use the passed color
            int midpointX = rectangleBorder.x + rectangleBorder.width / 2;
            int midpointY = rectangleBorder.y + rectangleBorder.height / 2;
            angle = atan2(midpointY, midpointX);
        }
    }
    return angle;
}


cv::Mat processImage(const cv::Mat &result, double pPosition, double vZ, double fGSR)
{
    cv::Mat blueMask;
    cv::Mat yellowMask;

    int height = result.rows;
    int croppedTopHeight = height * 0.4;
    cv::Rect roi(0, croppedTopHeight, result.cols, height - croppedTopHeight);
    cv::Mat imgBottom60 = result(roi);

    auto convertToHSV = [](const cv::Mat &input)
    {
        cv::Mat HueSatVal;
        cv::cvtColor(input, HueSatVal, cv::COLOR_BGR2HSV);
        return HueSatVal;
    };

    auto createMask = [](const cv::Mat &HueSatVal, const cv::Scalar &minRange, const cv::Scalar &maxRange)
    {
        cv::Mat mask;
        cv::inRange(HueSatVal, minRange, maxRange, mask);
        return mask;
    };

    auto applyMorphologicalOpening = [](cv::Mat &mask)
    {
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    };

    auto applyGaussianBlur = [](cv::Mat &mask)
    {
        cv::GaussianBlur(mask, mask, cv::Size(3, 3), 1.0);
    };

    auto combineMasks = [](const cv::Mat &mask1, const cv::Mat &mask2)
    {
        cv::Mat combinedMask;
        cv::bitwise_or(mask1, mask2, combinedMask);
        return combinedMask;
    };

    // Apply lambda functions values
    cv::Mat hsvImage = convertToHSV(imgBottom60);

    blueMask = createMask(hsvImage, cv::Scalar(99, 118, 41), cv::Scalar(139, 255, 255));
    applyMorphologicalOpening(blueMask);
    applyGaussianBlur(blueMask);

    yellowMask = createMask(hsvImage, cv::Scalar(19, 101, 99), cv::Scalar(29, 255, 255));
    applyMorphologicalOpening(yellowMask);
    applyGaussianBlur(yellowMask);

    cv::Mat combinedMask = combineMasks(blueMask, yellowMask);
    std::vector<std::vector<cv::Point>> blueContours = findContours(blueMask);
    std::vector<std::vector<cv::Point>> yellowContours = findContours(yellowMask);

    double blueAngle = 0.0;
    double yellowAngle = 0.0;


    cv::Scalar blueColor(255, 0, 0);
    cv::Scalar yellowColor(0, 255, 255);

    int yOffset = height * 0.4;

    blueAngle = processContours(result, blueContours, blueAngle, yOffset, blueColor);
    yellowAngle = processContours(result, yellowContours, yellowAngle, yOffset, yellowColor);

    // Extract blue and yellow masks and contours

    // Calculate midpoint of the screen
    int midX = result.cols / 2;
    int midY = result.rows / 2;

    // Initialize distances from cones to midpoint
    double distBlue = std::numeric_limits<double>::max();
    double distYellow = std::numeric_limits<double>::max();

    // Initialize flags to indicate cone presence
    bool bluePresent = false;
    bool yellowPresent = false;

    // Initialize variables to track cone positions
    int blueX = 0, yellowX = 0;

    // Check if blue cone is visible and calculate its distance to midpoint
    if (!blueContours.empty() && !blueContours[0].empty())
    {
        cv::Point bluePoint = blueContours[0][0];
        distBlue = std::sqrt(std::pow(bluePoint.x - midX, 2) + std::pow(bluePoint.y - midY, 2));
        blueX = bluePoint.x;
        bluePresent = true;
    }

    // Check if yellow cone is visible and calculate its distance to midpoint
    if (!yellowContours.empty() && !yellowContours[0].empty())
    {
        cv::Point yellowPoint = yellowContours[0][0];
        distYellow = std::sqrt(std::pow(yellowPoint.x - midX, 2) + std::pow(yellowPoint.y - midY, 2));
        yellowX = yellowPoint.x;
        yellowPresent = true;
    }

    // Determine cone positions relative to the middle of the screen
    if (bluePresent && yellowPresent)
    {

        if (blueX > midX && yellowX < midX) // ccw
        {
            if (((blueX - midX) - (midX - yellowX)) > -200 && ((blueX - midX) - (midX - yellowX)) < 200)
            {
                // Blue cone is on the right side, and yellow cone is on the left side
                steeringAngle = 2.832e-6 * abs((midX - blueX) - (yellowX - midX)) * abs((midX - blueX) - (yellowX - midX));
            }
            else
            {
                steeringAngle = (std::abs(distBlue - distYellow) * 0.29) / midX;
            }
        }
        else if (blueX < midX && yellowX > midX)
        {
            if (((midX - blueX) - (yellowX - midX)) > -200 && ((midX - blueX) - (yellowX - midX)) < 200)
            {
                // Blue cone is on the left side, and yellow cone is on the right side
                steeringAngle = -2.832e-6 * abs((midX - blueX) - (yellowX - midX)) * abs((midX - blueX) - (yellowX - midX));
            }
            else
            {
                steeringAngle = (std::abs(distBlue - distYellow) * -0.29) / midX;
            }
        }
    }

    else if (bluePresent && !yellowPresent)
    {
        // Only blue cone is present
        if (blueX < midX)
        {
            // Blue cone is on the left side
            steeringAngle = -0.1;
        }
        else if (blueX > midX)
        {
            // Blue cone is on the right side
            steeringAngle = 0.1;
        }
    }
    else if (yellowPresent && !bluePresent)
    {
        // Only yellow cone is present
        if (yellowX < midX)
        {
            // Yellow cone is on the left side
            steeringAngle = 0.1;
        }
        else if (yellowX > midX)
        {
            // Yellow cone is on the right side
            steeringAngle = -0.1;
        }
    }

    return result;
}





#endif

int32_t main(int32_t argc, char **argv) {
    int32_t retCode{1};
    // Parse the command line parameters as we require the user to specify some mandatory information on startup.
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if ( (0 == commandlineArguments.count("cid")) ||
         (0 == commandlineArguments.count("name")) ||
         (0 == commandlineArguments.count("width")) ||
         (0 == commandlineArguments.count("height")) ) {
        std::cerr << argv[0] << " attaches to a shared memory area containing an ARGB image." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> --name=<name of shared memory area> [--verbose]" << std::endl;
        std::cerr << "         --cid:    CID of the OD4Session to send and receive messages" << std::endl;
        std::cerr << "         --name:   name of the shared memory area to attach" << std::endl;
        std::cerr << "         --width:  width of the frame" << std::endl;
        std::cerr << "         --height: height of the frame" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=253 --name=img --width=640 --height=480 --verbose" << std::endl;
    }
    else {
        // Extract the values from the command line parameters
        const std::string NAME{commandlineArguments["name"]};
        const uint32_t WIDTH{static_cast<uint32_t>(std::stoi(commandlineArguments["width"]))};
        const uint32_t HEIGHT{static_cast<uint32_t>(std::stoi(commandlineArguments["height"]))};
        const bool VERBOSE{commandlineArguments.count("verbose") != 0};

        // Attach to the shared memory.
        std::unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
        if (sharedMemory && sharedMemory->valid()) {
            std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;

            // Interface to a running OpenDaVINCI session where network messages are exchanged.
            // The instance od4 allows you to send and receive messages.
            cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};

            opendlv::proxy::GroundSteeringRequest gsr;
            std::mutex gsrMutex;
            auto onGroundSteeringRequest = [&gsr, &gsrMutex](cluon::data::Envelope &&env){
                // The envelope data structure provide further details, such as sampleTimePoint as shown in this test case:
                // https://github.com/chrberger/libcluon/blob/master/libcluon/testsuites/TestEnvelopeConverter.cpp#L31-L40
                std::lock_guard<std::mutex> lck(gsrMutex);
                gsr = cluon::extractMessage<opendlv::proxy::GroundSteeringRequest>(std::move(env));
                std::cout << "lambda: groundSteering = " << gsr.groundSteering() << std::endl;
            };

            od4.dataTrigger(opendlv::proxy::GroundSteeringRequest::ID(), onGroundSteeringRequest);

            // Endless loop; end the program by pressing Ctrl-C.
            while (od4.isRunning()) {
                // OpenCV data structure to hold an image.
                cv::Mat img;

                // Wait for a notification of a new frame.
                sharedMemory->wait();

                // Lock the shared memory.
                sharedMemory->lock();
                {
                    // Copy the pixels from the shared memory into our own data structure.
                    cv::Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
                    img = wrapped.clone();
                }
                // TODO: Here, you can add some code to check the sampleTimePoint when the current frame was captured.
                sharedMemory->unlock();

                // TODO: Do something with the frame.
                // Example: Draw a red rectangle and display image.
                cv::rectangle(img, cv::Point(50, 50), cv::Point(100, 100), cv::Scalar(0,0,255));

                // If you want to access the latest received ground steering, don't forget to lock the mutex:
                {
                    std::lock_guard<std::mutex> lck(gsrMutex);
                    std::cout << "main: groundSteering = " << gsr.groundSteering() << std::endl;
                }

                // Display image on your screen.
                if (VERBOSE) {
                    cv::imshow(sharedMemory->name().c_str(), img);
                    cv::waitKey(1);
                }
            }
        }
        retCode = 0;
    }
    return retCode;
}

