#include<opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

using namespace std;
using namespace cv;

class SiftAndEdge {

private:
    Mat modified_frame;
    std::mutex frame_mutex;

public:

    void ApplySift(Mat src_gray) {
        cv::Ptr<cv::SIFT> detector = cv::SIFT::create();
        std::vector<cv::KeyPoint> keypoints;
        detector->detect(src_gray, keypoints);
        frame_mutex.lock();
        drawKeypoints(modified_frame, keypoints, modified_frame);
        frame_mutex.unlock();
    }

    void DetectEdges(Mat src_gray) {
        Mat detected_edges;

        int threshold = 100;
        int ratio = 3;
        int kernel_size = 3;

        blur(src_gray, detected_edges, Size(3,3));
        Canny(detected_edges, detected_edges, threshold, threshold*ratio, kernel_size);
        cvtColor(detected_edges, detected_edges, COLOR_GRAY2BGR);
        frame_mutex.lock();
        modified_frame += detected_edges;
        frame_mutex.unlock();
    }

    int ProduceVideo(string video_reference) {
        int frame_num = -1;
        VideoCapture video_capture(video_reference);

        if (!video_capture.isOpened()) {
            cout << "Could not open video" << endl;
            return -1;
        }

        //Write To video
        int ex = static_cast<int>(video_capture.get(CAP_PROP_FOURCC));
        cout << "fourcc: " << cv::VideoWriter::fourcc('M','J','P','G') << endl;
        cout << "fps: " << video_capture.get(CAP_PROP_FPS) << endl;
        VideoWriter output_video;
        output_video.open("output.avi", cv::VideoWriter::fourcc('M','J','P','G'), video_capture.get(CAP_PROP_FPS), Size(640, 360), true);

        if (!output_video.isOpened())
        {
            cout << "output_video failed to open" << endl;
            return -1;
        }

        while (video_capture.isOpened())
        {
            Mat frame;
            frame_num++;
            bool isSuccess = video_capture.read(frame);

            if(!isSuccess) {
                cout << "Video camera is disconnected" << endl;
                break;
            }

            //Apply crop
            Mat resized;
            resize(frame, resized, Size(640, 360), INTER_LINEAR);
            modified_frame = resized;
            //Apply Gray scale
            Mat src_gray;
            cvtColor(resized, src_gray, COLOR_BGR2GRAY);

            //Apply Image Processing
            std::thread t1(&SiftAndEdge::DetectEdges, this, src_gray);
            std::thread t2(&SiftAndEdge::ApplySift, this, src_gray);
            t1.join();
            t2.join();
            output_video.write(modified_frame);
            imwrite("frame.png", modified_frame);


        }
        video_capture.release();
        output_video.release();
        destroyAllWindows();
    }
};

static void help() {
        cout
        << "------------------------------------------------------------------------------" << endl
        << "This program resizes a video to 640 x 360 and applies Edge detection"
        << " and SIFT Feature detections to the video output.avi"                                       << endl
        << "Usage:"                                                                         << endl
        << "./challenge <referenceVideo>" << endl
        << "--------------------------------------------------------------------------"     << endl
        << endl;
}





int main(int argc, char *argv[]) {
    help();
    if (argc != 2) {
        cout << "Missing parameters" << endl;
        return -1;
    }

    const string video_reference = argv[1];

    SiftAndEdge sift_and_edge;
    sift_and_edge.ProduceVideo(video_reference);


    return 0;

}
