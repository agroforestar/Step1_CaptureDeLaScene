#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace cv;
using namespace std;

/**
@brief Recognize de shape from the number of side
@param size : number of borders
@return the name of the shape
**/
string detect( int size ) {
   // initialize the shape nameand approximate the contour
    string shape = "unidentified";

// if the shape is a triangle, it will have 3 vertices
    if (size == 3)
        shape = "triangle";
    //if the shape has 4 vertices, it is either a square or
            // a rectangle
    else if (size == 4)
    {
        shape = "rectangle";
    }
    else if (size == 5)
        shape = "pentagon";
    else if (size == 6)
        shape = "hexagon";
    else if (size == 7)
        shape = "fleche";
    else
        shape = "circle";
        // return the name of the shape
    return shape;
}

/**
@brief Write in the image, the name of the geometrical shape
@param form : name of the shape
@param x : X coord of the shape
@param y : Y coord of the shape
@param src : image in which we write
**/
void writeFormName(string form, int x, int y, Mat src) {
    Size box = getTextSize(form, FONT_HERSHEY_SIMPLEX, 0.5, 2, 0);
    putText(src, form, Point(int(x - box.height / 2), int(y + box.width / 2)), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
}

/**
@brief Write in the file "data.txt" the objects and their coordinates
@param x : X coord of the shape
@param y : Y coord of the shape
@param form : name of the shape
@param color : color (BGR format) of the shape
**/
void writeInOutputFile(double x, double y, string form, Scalar color) {
    ofstream myfile;
    myfile.open("D:/Mes_Documents/code/OpenCVc++/data.txt", ios::app);
    myfile << form ;
    myfile << ";";
    myfile << color;
    myfile << ";";
    myfile << x;
    myfile << ";";
    myfile << y ;
    myfile << "\n";
    myfile.close();
}
    
/**
@brief Open a window with a webcam frame. 
@details The program waits the escap key to close the window. Espace key take a screenshot
@param[in] path : path to save the screenchottaken by the camera
**/
void useCamera(char* path) {
    try {
        VideoCapture cam = VideoCapture(0);  // 0->index of camera
        int img_counter = 0;
        bool out = false;
        while (not out) {
            Mat frame;
            bool ret = cam.read(frame);
            if (!ret) {
                std::cout << "failed to grab frame" << std::endl;
            }

            cv::imshow("test", frame);
            int k = waitKey(1);
            if (k % 256 == 27) { //ESC pressed
                std::cout << "Escape hit, closing..." << std::endl;
                out = true;
            }
            else if (k % 256 == 32) { //Barre espace
                string img_name(path);
                imwrite(img_name, frame);
                img_counter += 1;
            }
            
        
        }
        cam.release();
        destroyAllWindows();
        
    }
        catch (Exception e) {
            std::cout << e.what() << std::endl;
    }

}
    

int main(int argc, char* argv[])
{

    useCamera(argv[1]);

    Mat src = imread(argv[1], cv::IMREAD_COLOR); //Rename with test file or with "screenshot.png" to use the image takes by the webcam D: / Mes_Documents / code / OpenCVc++ / test2.png
    Mat thr, gray;
    blur(src, src, Size(3, 3));  
    cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    Canny(gray, thr, 50, 190, 3, false);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(thr.clone(), contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<Point>> contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f>center(contours.size());
    vector<float>radius(contours.size());
    vector<vector<Point> >hull(contours.size());
    Mat labels = cv::Mat::zeros(src.size(), CV_8UC1);
    std::vector<float> cont_avgs(contours.size(), 0.f);
    for (int i = 0; i < contours.size(); i++) // for each shape
    {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 2, true); // modifier le epsilon pour d??tecter de plus petit objects
        if (contours_poly[i].size() > 2) {
            boundRect[i] = boundingRect(Mat(contours_poly[i]));
            minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
            convexHull(Mat(contours[i]), hull[i], false);

            string form = detect(contours_poly[i].size());

            Moments M = moments(contours_poly[i]);
            int cX = int((M.m10 / M.m00));
            int cY = int((M.m01 / M.m00));

            
            drawContours(src, contours_poly, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());
            writeFormName(form, cX, cY, src);

            drawContours(labels, contours_poly, i, Scalar(i), FILLED);
            cv::Scalar color = cv::mean(src(boundRect[i]), labels(boundRect[i]));

            writeInOutputFile(cX, cY, form, color);
        }
        
    }

    imshow("src", src);
    imshow("Canny", thr);
    waitKey();
}
