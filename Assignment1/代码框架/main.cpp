#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <Eigen/Eigen>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdexcept>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // 绕 Z 轴旋转，输入角度的单位是度。
    model << cos(rotation_angle / 180 * MY_PI), -sin(rotation_angle / 180 * MY_PI), 0, 0,
        sin(rotation_angle / 180 * MY_PI), cos(rotation_angle / 180 * MY_PI), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // 先用正距离确定近平面的边界，再转换成课件使用的负 Z 坐标。
    float half_fov = eye_fov / 2;
    float tan_half_fov = tan(half_fov / 180 * MY_PI);
    float top = zNear * tan_half_fov;
    float right = top * aspect_ratio;
    float left = -right;
    float bottom = -top;
    float n = -zNear;
    float f = -zFar;

    Eigen::Matrix4f ortho = Eigen::Matrix4f::Identity();
    ortho << 2 / (right - left), 0, 0, -(right + left) / (right - left),
        0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
        0, 0, 2 / (n - f), -(n + f) / (n - f),
        0, 0, 0, 1;

    Eigen::Matrix4f persp2ortho = Eigen::Matrix4f::Identity();
    persp2ortho << n, 0, 0, 0,
        0, n, 0, 0,
        0, 0, n + f, -n * f,
        0, 0, 1, 0;

    projection = ortho * persp2ortho;

    return projection;
}

// axis 表示过原点的旋转轴方向；angle 的单位是度，正方向遵循右手定则。
Eigen::Matrix4f get_rotation(Eigen::Vector3f axis, float angle)
{
    // 公式要求单位轴；零向量无法定义旋转轴。
    float axis_length = axis.norm();
    if (axis_length == 0.0f)
    {
        throw std::invalid_argument("Rotation axis must be non-zero");
    }
    axis /= axis_length;

    float radians = angle * MY_PI / 180.0f;
    float c = std::cos(radians);
    float s = std::sin(radians);

    // K * v = axis.cross(v)，将叉乘写成矩阵乘法。
    Eigen::Matrix3f K;
    K << 0, -axis.z(), axis.y(),
        axis.z(), 0, -axis.x(),
        -axis.y(), axis.x(), 0;

    // 罗德里格斯公式：R = cos(theta) I + (1-cos(theta)) nn^T + sin(theta) K。
    Eigen::Matrix3f R = c * Eigen::Matrix3f::Identity() + (1.0f - c) * axis * axis.transpose() + s * K;

    Eigen::Matrix4f rotation = Eigen::Matrix4f::Identity();
    rotation.block<3, 3>(0, 0) = R;
    return rotation;
}

int main(int argc, const char **argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3)
    {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4)
        {
            filename = std::string(argv[3]);
        }
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a')
        {
            angle += 10;
        }
        else if (key == 'd')
        {
            angle -= 10;
        }
    }

    return 0;
}
