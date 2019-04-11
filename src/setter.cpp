/**
 * @Author: PALARD Nicolas <nclsp>
 * @Date:   2019-04-11T09:53:44+02:00
 * @Email:  palard@rea.lity.tech
 * @Project: Natar.io
 * @Last modified by:   nclsp
 * @Last modified time: 2019-04-11T11:10:38+02:00
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

    if (argc != 3) {
        std::cerr << "Missed arguments: ./natar-image-setter image-file redis-key" << std::endl;
        return EXIT_FAILURE;
    }

    cv::Mat image = cv::imread(argv[1]);
    cv::Mat rgbImage;
    cv::cvtColor(image, rgbImage, cv::COLOR_BGR2RGB);
    Image* redisImage = new Image(rgbImage.cols, rgbImage.rows, rgbImage.channels(), rgbImage.data);

    clientSync.setImage(redisImage, argv[2]);

    delete redisImage;
}
