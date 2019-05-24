/**
 * @Author: PALARD Nicolas <nclsp>
 * @Date:   2019-04-11T09:59:39+02:00
 * @Email:  palard@rea.lity.tech
 * @Project: Natar.io
 * @Last modified by:   nclsp
 * @Last modified time: 2019-04-11T11:10:34+02:00
 * @Copyright: RealityTech 2018-2019
 */

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <RedisImageHelper.hpp>

int main(int argc, char** argv) {
    std::string host = "127.0.0.1";
    int port = 6379;

    RedisImageHelperSync clientSync(host, port, "");
    if (!clientSync.connect()) {
        std::cerr << "Cannot connect to redis server. Please ensure that a redis server is up and running." << std::endl;
        return EXIT_FAILURE;
    }

    if (argc != 2) {
        std::cerr << "Missed arguments: ./natar-image-getter redis key" << std::endl;
        return EXIT_FAILURE;
    }

    std::string imageKey = argv[1];
    int width = clientSync.getInt(imageKey + ":width");
    int height = clientSync.getInt(imageKey + ":height");
    int channels = clientSync.getInt(imageKey + ":channels");
    std::cout <<"#Info" << std::endl
              << width << "x" << height << "x" << channels << std::endl;

    Image* image = clientSync.getImage(width, height, channels, imageKey);
    if (image == NULL) {
        std::cerr << "Failed to retrieve image from redis" << std::endl;
        return EXIT_FAILURE;
    }

    cv::Mat rgbImage, bgrImage;
    switch (channels) {
        case 3:
            rgbImage = cv::Mat(height, width, CV_8UC3, image->data());
            cv::cvtColor(rgbImage, bgrImage, cv::COLOR_RGB2BGR);
        break;
        case 4:
            rgbImage = cv::Mat(height, width, CV_8UC4, image->data());
            cv::cvtColor(rgbImage, bgrImage, cv::COLOR_RGBA2BGRA);
        break;
    }
    cv::imshow("Get image", bgrImage);
    cv::waitKey();

    delete image;
}
