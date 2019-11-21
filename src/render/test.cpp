
// #include <opencv2/opencv.hpp>

// int main(int argc, char ** argv) {
//     cv::Mat img(480, 640, CV_32FC1);
//     cv::Mat norm_img(480, 640, CV_32FC1);
//     // img.at<float>(480-1-i/SCR_WIDTH, i%SCR_WIDTH, 0) = depth/6;
//     FILE *depth_file = fopen(argv[1], "rb");
//     if (depth_file == NULL) {
//         printf("Error opening file!\n");
//         exit(1);
//     }
//     int width, height;
//     float x, y, depth;
//     int result;
//     result = fread(&width, sizeof(int), 1, depth_file);
//     result = fread(&height, sizeof(int), 1, depth_file);
//     printf("%d %d\n", width, height);
//     float max = -1000, min = 1000;
//     for (int i = 0; i < height; i++) {
//         for (int j = 0; j < width; j++) {
//             result = fread(&depth, sizeof(float), 1, depth_file);
//             result = fread(&depth, sizeof(float), 1, depth_file);
//             img.at<float>(height-1-i, j, 0) = depth;
//             max = std::max(max, depth);
//             min = std::min(min, depth);
//             result = fread(&depth, sizeof(float), 1, depth_file);
//             cv::normalize(img, norm_img, 1.0, 0.0, cv::NORM_INF);
//         }
//     }
//     printf("max %f min %f\n", max, min);
//     cv::imshow("test", norm_img);
//     cv::waitKey(0);

//     return 0;

// }