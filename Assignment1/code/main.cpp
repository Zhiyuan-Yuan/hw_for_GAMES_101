#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

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

Eigen::Matrix4f get_rotation(Vector3f axis, float angle)
{
    Eigen::Matrix4f Rotation = Eigen::Matrix4f::Identity();
    axis.normalize();
    angle = angle * M_PI / 180;
    //求axis的反对称矩阵
    Matrix3f n;
    n << 0, -axis(2), axis(1),
        axis(2), 0, -axis(0),
        -axis(1), axis(0), 0;
    //由罗德里格斯公式求绕任意轴旋转任意角的变换矩阵
    Rotation.block(0, 0, 3, 3) = cos(angle) * Eigen::Matrix3f::Identity() + (1 - cos(angle)) * axis * axis.transpose() + sin(angle) * n;
    return Rotation;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    float degree2rad = MY_PI / (float)180;
    model.block(0, 0, 2, 2) << cos(rotation_angle * degree2rad), -sin(rotation_angle * degree2rad),
        sin(rotation_angle * degree2rad), cos(rotation_angle * degree2rad);

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    Matrix4f M_persp_to_ortho;
    M_persp_to_ortho << zNear, 0, 0, 0,
        0, zNear, 0, 0,
        0, 0, zNear + zFar, -zNear * zFar,
        0, 0, 1, 0;


    Matrix4f M_ortho;
    float height = -zNear * tan(eye_fov / 2 / 180 * MY_PI) * 2;
    float width = height * aspect_ratio;
    M_ortho << 2 / width, 0, 0, 0,
        0, 2 / height, 0, 0,
        0, 0, 2 / (zNear - zFar), -(zNear + zFar) / 2,
        0, 0, 0, 1;

    projection = M_ortho * M_persp_to_ortho * projection;

    return projection;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, -0.1, -50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        //r.set_model(get_model_matrix(angle));
        //绕任意轴旋转
        Vector3f rotate_axis;
        rotate_axis << 1, 0, 0;
        r.set_model(get_rotation(rotate_axis, angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, -0.1, -50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::namedWindow("image", 0);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
