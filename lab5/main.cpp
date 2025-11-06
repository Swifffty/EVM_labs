#include <iostream>
#include <iomanip>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Can't open camera" << std::endl;
        return -1;
    }
    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);
    double freq = cv::getTickFrequency();
    double fps = 0;
    bool flag_fps = false;
    cv::Mat frame;
    bool blur_mod = false;
    bool reverse_mod = false;
    bool bitwise_not_mod = false;

    while (true) {
        double t_start = cv::getTickCount();
        cap >> frame;
        double t_read = cv::getTickCount();
        if (frame.empty()) {
            std::cerr << "Can't load frame" << std::endl;
            break;
        }
        cv::Mat image = frame;

        if (blur_mod) {
            cv::blur(frame, image, cv::Size(3, 3));
            frame = image;
        }

        if (reverse_mod) {
            cv::rotate(frame, image, cv::ROTATE_90_CLOCKWISE);
            frame = image;
        }

        if (bitwise_not_mod) {
            cv::bitwise_not(frame, image);
        }

        if (flag_fps) {
            std::string text = "FPS " + std::to_string(fps);
            cv::putText(image, text, cv::Point(0,24), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
        }
        double t_convert = cv::getTickCount();
        cv::imshow("Camera", image);
        double t_out = cv::getTickCount();
        char c = (char) cv::waitKey(33);

        if (c == 27) {
            break;
        }

        switch (c) {
            case 49:
                flag_fps = !flag_fps;
                break;
            case 50:
                blur_mod = !blur_mod;
                break;
            case 51:
                reverse_mod = !reverse_mod;
                break;
            case 52:
                bitwise_not_mod = !bitwise_not_mod;
                break;
            case 53:
                blur_mod = false;
                reverse_mod = false;
                bitwise_not_mod = false;
                break;
        }

        double t_end_frame = cv::getTickCount();
        fps = 1000.0 / (((t_end_frame - t_start) / freq) * 1000);

        double time_frame = cv::getTickCount() - t_start;
        double time_read = 100 * (t_read - t_start) / time_frame;
        double time_convert = 100 * (t_convert - t_read) / time_frame;
        double time_out = 100 * (t_out - t_convert) / time_frame;
        std::cout << "read " << std::fixed << std::setprecision(2) << time_read << " convert " << std::fixed << std::setprecision(2) << time_convert << " out "<< std::fixed << std::setprecision(2) << time_out << std::endl;
    }
    cv::destroyAllWindows();
    return 0;
}
