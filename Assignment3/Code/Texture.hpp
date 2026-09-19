//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture
{
private:
    cv::Mat image_data;

public:
    Texture(const std::string &name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        // 1. UV 转图片坐标，限制在有效像素范围内
        float x = std::clamp(u * width, 0.0f, float(width - 1));
        float y = std::clamp((1.0f - v) * height,
                             0.0f, float(height - 1));

        // 2. 找到周围四个像素的坐标
        int x0 = static_cast<int>(std::floor(x));
        int y0 = static_cast<int>(std::floor(y));
        int x1 = std::min(x0 + 1, width - 1);
        int y1 = std::min(y0 + 1, height - 1);

        // 3. 采样点在这四个像素之间的相对位置
        float s = x - x0;
        float t = y - y0;

        // OpenCV 使用 (行, 列)，即 (y, x)
        auto read_color = [&](int px, int py) -> Eigen::Vector3f
        {
            auto color = image_data.at<cv::Vec3b>(py, px);
            return Eigen::Vector3f(color[0], color[1], color[2]);
        };

        Eigen::Vector3f c00 = read_color(x0, y0);
        Eigen::Vector3f c10 = read_color(x1, y0);
        Eigen::Vector3f c01 = read_color(x0, y1);
        Eigen::Vector3f c11 = read_color(x1, y1);

        // 4. 先横向插值，再纵向插值
        // 横向插值
        Eigen::Vector3f top = (1.0f - s) * c00 + s * c10;
        Eigen::Vector3f bottom = (1.0f - s) * c01 + s * c11;

        return (1.0f - t) * top + t * bottom; // 纵向插值
    }
};
#endif // RASTERIZER_TEXTURE_H
