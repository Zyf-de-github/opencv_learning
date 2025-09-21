#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>


//int main() {
//    // 读取图片
//    cv::Mat image = cv::imread("C:/Users/86138/Pictures/Screenshots/屏幕截图 2025-09-21 110540.png");
//    // 检查图片是否成功加载
//    if (image.empty()) {
//        std::cout << "无法加载图片！" << std::endl;
//        return -1;
//    }
//
//    // 显示图片
//    cv::imshow("Display Image", image);
//
//    // 等待按键
//    cv::waitKey(0);
//
//    return 0;
//}



int main() {
    // 创建一个带有网格的测试图像，这样透视效果更明显
    cv::Mat image(400, 600, CV_8UC3, cv::Scalar(255, 255, 255));

    // 绘制网格线
    for (int i = 0; i <= image.rows; i += 20) {
        cv::line(image, cv::Point(0, i), cv::Point(image.cols, i), cv::Scalar(200, 200, 200), 1);
    }
    for (int i = 0; i <= image.cols; i += 20) {
        cv::line(image, cv::Point(i, 0), cv::Point(i, image.rows), cv::Scalar(200, 200, 200), 1);
    }

    // 绘制中心十字和文字
    cv::line(image, cv::Point(image.cols / 2, 0), cv::Point(image.cols / 2, image.rows), cv::Scalar(0, 0, 255), 2);
    cv::line(image, cv::Point(0, image.rows / 2), cv::Point(image.cols, image.rows / 2), cv::Scalar(0, 0, 255), 2);
    cv::putText(image, "Original Image", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);

    // 定义原始图像的四个角点
    std::vector<cv::Point2f> srcPoints;
    srcPoints.push_back(cv::Point2f(0, 0));                      // 左上角
    srcPoints.push_back(cv::Point2f(image.cols - 1, 0));         // 右上角
    srcPoints.push_back(cv::Point2f(image.cols - 1, image.rows - 1)); // 右下角
    srcPoints.push_back(cv::Point2f(0, image.rows - 1));         // 左下角

    // 1. 仿射变换的目标点（保持平行性）
    std::vector<cv::Point2f> dstAffinePoints;
    dstAffinePoints.push_back(cv::Point2f(50, 50));              // 左上角
    dstAffinePoints.push_back(cv::Point2f(image.cols - 50, 80)); // 右上角
    dstAffinePoints.push_back(cv::Point2f(image.cols - 50, image.rows - 50)); // 右下角
    dstAffinePoints.push_back(cv::Point2f(50, image.rows - 80)); // 左下角

    // 2. 透视变换的目标点（产生强烈的透视效果）
    std::vector<cv::Point2f> dstPerspectivePoints;
    // 强烈的透视效果：左上和左下向右移动，右上和右下向左移动
    dstPerspectivePoints.push_back(cv::Point2f(200, 50));        // 左上角（向右移动）
    dstPerspectivePoints.push_back(cv::Point2f(image.cols - 200, 50)); // 右上角（向左移动）
    dstPerspectivePoints.push_back(cv::Point2f(image.cols - 100, image.rows - 50)); // 右下角（向左移动）
    dstPerspectivePoints.push_back(cv::Point2f(100, image.rows - 50)); // 左下角（向右移动）

    // 计算仿射变换矩阵
    cv::Mat affineMatrix = cv::getAffineTransform(
        std::vector<cv::Point2f>(srcPoints.begin(), srcPoints.begin() + 3),
        std::vector<cv::Point2f>(dstAffinePoints.begin(), dstAffinePoints.begin() + 3)
    );

    // 计算透视变换矩阵
    cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPerspectivePoints);

    // 应用变换
    cv::Mat affineTransformed;
    cv::warpAffine(image, affineTransformed, affineMatrix, image.size());

    cv::Mat perspectiveTransformed;
    cv::warpPerspective(image, perspectiveTransformed, perspectiveMatrix, image.size());

    // 绘制变换前后的点
    cv::Mat imageWithPoints = image.clone();
    cv::Mat affineWithPoints = affineTransformed.clone();
    cv::Mat perspectiveWithPoints = perspectiveTransformed.clone();

    // 绘制原始点
    for (const auto& pt : srcPoints) {
        cv::circle(imageWithPoints, pt, 8, cv::Scalar(0, 0, 255), -1); // 红色点
    }

    // 绘制仿射变换点
    for (int i = 0; i < 4; i++) {
        cv::circle(affineWithPoints, dstAffinePoints[i], 8, cv::Scalar(0, 255, 0), -1); // 绿色点
        cv::line(affineWithPoints, dstAffinePoints[i], dstAffinePoints[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
    }

    // 绘制透视变换点
    for (int i = 0; i < 4; i++) {
        cv::circle(perspectiveWithPoints, dstPerspectivePoints[i], 8, cv::Scalar(255, 0, 0), -1); // 蓝色点
        cv::line(perspectiveWithPoints, dstPerspectivePoints[i], dstPerspectivePoints[(i + 1) % 4], cv::Scalar(255, 0, 0), 2);
    }

    // 添加文字说明
    cv::putText(imageWithPoints, "Original", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);
    cv::putText(affineWithPoints, "Affine Transform", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);
    cv::putText(perspectiveWithPoints, "Perspective Transform", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);

    // 调整窗口大小并显示
    cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
    cv::namedWindow("Affine Transform", cv::WINDOW_NORMAL);
    cv::namedWindow("Perspective Transform", cv::WINDOW_NORMAL);

    cv::resizeWindow("Original Image", 600, 400);
    cv::resizeWindow("Affine Transform", 600, 400);
    cv::resizeWindow("Perspective Transform", 600, 400);

    cv::imshow("Original Image", imageWithPoints);
    cv::imshow("Affine Transform", affineWithPoints);
    cv::imshow("Perspective Transform", perspectiveWithPoints);

    // 打印说明
    std::cout << "=== 变换效果对比 ===" << std::endl;
    std::cout << "仿射变换: 保持平行性，网格线仍然平行" << std::endl;
    std::cout << "透视变换: 产生3D透视效果，平行线在远处相交" << std::endl;
    std::cout << "注意观察网格线的变化！" << std::endl;

    // 等待按键
    cv::waitKey(0);

    return 0;
}

//int main() {
//    // 1. 创建一个视频捕获对象，参数0表示打开默认摄像头
//    cv::VideoCapture cap(0);
//
//    // 检查摄像头是否成功打开
//    if (!cap.isOpened()) {
//        std::cout << "错误：无法打开摄像头！" << std::endl;
//        return -1;
//    }
//
//    // 创建两个窗口
//    cv::namedWindow("Original Video", cv::WINDOW_AUTOSIZE);
//    cv::namedWindow("Canny Edge Detection", cv::WINDOW_AUTOSIZE);
//
//    std::cout << "按 'ESC' 键退出程序..." << std::endl;
//
//    while (true) {
//        cv::Mat frame, edges, grayFrame;
//
//        // 2. 从摄像头捕获一帧图像
//        cap >> frame;
//
//        // 检查帧是否为空
//        if (frame.empty()) {
//            std::cout << "错误：捕获到的帧为空！" << std::endl;
//            break;
//        }
//
//        // 3. 将彩色图像转换为灰度图像（Canny处理需要单通道图像）
//        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
//
//        // 4. 使用Canny算法进行边缘检测
//        // 参数：输入图像，输出图像，阈值1，阈值2
//        cv::Canny(grayFrame, edges, 50, 150);
//
//        // 5. 在窗口中显示图像
//        cv::imshow("Original Video", frame);
//        cv::imshow("Canny Edge Detection", edges);
//
//        // 6. 等待30毫秒，并检查是否按下'ESC'键 (ASCII码为27)
//        if (cv::waitKey(30) == 27) {
//            std::cout << "程序已由用户终止。" << std::endl;
//            break;
//        }
//    }
//
//    // 7. 程序结束时，自动释放摄像头并销毁所有窗口
//    return 0;
//}

//int main() {
//    // 1. 加载人脸检测器（Haar级联分类器）
//    cv::CascadeClassifier faceCascade;
//    // 请确保这个路径指向正确的xml文件！
//    std::string cascadePath = "E:/study_tools/opencv/opencv/build/etc/haarcascades/haarcascade_frontalface_default.xml";
//
//    if (!faceCascade.load(cascadePath)) {
//        std::cout << "错误：无法加载人脸检测分类器文件！" << std::endl;
//        std::cout << "请确认文件路径: " << cascadePath << std::endl;
//        return -1;
//    }
//
//    // 2. 打开摄像头
//    cv::VideoCapture cap(0);
//    if (!cap.isOpened()) {
//        std::cout << "错误：无法打开摄像头！" << std::endl;
//        return -1;
//    }
//
//    std::cout << "人脸检测程序已启动..." << std::endl;
//    std::cout << "按 'ESC' 键退出程序" << std::endl;
//
//    cv::Mat frame, grayFrame;
//
//    while (true) {
//        // 3. 从摄像头读取一帧图像
//        cap >> frame;
//        if (frame.empty()) {
//            std::cout << "错误：捕获到的帧为空！" << std::endl;
//            break;
//        }
//
//        // 4. 转换为灰度图（人脸检测需要在灰度图上进行）
//        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
//        // 增强对比度，提高检测精度
//        cv::equalizeHist(grayFrame, grayFrame);
//
//        // 5. 检测人脸
//        std::vector<cv::Rect> faces;
//        faceCascade.detectMultiScale(
//            grayFrame,   // 输入灰度图像
//            faces,       // 输出检测到的人脸矩形列表
//            1.1,         // 缩放比例（1.1表示每次搜索窗口扩大10%）
//            3,           // 最小邻居数（越高检测越严格，漏检越多但误检越少）
//            0,           // 标志（通常为0）
//            cv::Size(30, 30) // 检测的最小人脸尺寸
//        );
//
//        // 6. 在原始彩色图像上框选出检测到的人脸
//        for (const auto& face : faces) {
//            // 绘制绿色矩形框，线宽为2
//            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
//
//            // 可选：在框上方显示文字
//            cv::putText(frame, "Face",
//                cv::Point(face.x, face.y - 10),
//                cv::FONT_HERSHEY_SIMPLEX, 0.7,
//                cv::Scalar(0, 255, 0), 2);
//        }
//
//        // 7. 在窗口标题中显示检测到的人脸数量
//        std::string windowTitle = "Face Detection - Found: " + std::to_string(faces.size());
//        cv::imshow(windowTitle, frame);
//
//        // 8. 检测按键，ESC键退出
//        if (cv::waitKey(30) == 27) { // ESC键的ASCII码是27
//            std::cout << "程序已退出。" << std::endl;
//            break;
//        }
//    }
//
//    // 9. 自动释放资源
//    return 0;
//}