#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>


//int main() {
//    // ��ȡͼƬ
//    cv::Mat image = cv::imread("C:/Users/86138/Pictures/Screenshots/��Ļ��ͼ 2025-09-21 110540.png");
//    // ���ͼƬ�Ƿ�ɹ�����
//    if (image.empty()) {
//        std::cout << "�޷�����ͼƬ��" << std::endl;
//        return -1;
//    }
//
//    // ��ʾͼƬ
//    cv::imshow("Display Image", image);
//
//    // �ȴ�����
//    cv::waitKey(0);
//
//    return 0;
//}



int main() {
    // ����һ����������Ĳ���ͼ������͸��Ч��������
    cv::Mat image(400, 600, CV_8UC3, cv::Scalar(255, 255, 255));

    // ����������
    for (int i = 0; i <= image.rows; i += 20) {
        cv::line(image, cv::Point(0, i), cv::Point(image.cols, i), cv::Scalar(200, 200, 200), 1);
    }
    for (int i = 0; i <= image.cols; i += 20) {
        cv::line(image, cv::Point(i, 0), cv::Point(i, image.rows), cv::Scalar(200, 200, 200), 1);
    }

    // ��������ʮ�ֺ�����
    cv::line(image, cv::Point(image.cols / 2, 0), cv::Point(image.cols / 2, image.rows), cv::Scalar(0, 0, 255), 2);
    cv::line(image, cv::Point(0, image.rows / 2), cv::Point(image.cols, image.rows / 2), cv::Scalar(0, 0, 255), 2);
    cv::putText(image, "Original Image", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);

    // ����ԭʼͼ����ĸ��ǵ�
    std::vector<cv::Point2f> srcPoints;
    srcPoints.push_back(cv::Point2f(0, 0));                      // ���Ͻ�
    srcPoints.push_back(cv::Point2f(image.cols - 1, 0));         // ���Ͻ�
    srcPoints.push_back(cv::Point2f(image.cols - 1, image.rows - 1)); // ���½�
    srcPoints.push_back(cv::Point2f(0, image.rows - 1));         // ���½�

    // 1. ����任��Ŀ��㣨����ƽ���ԣ�
    std::vector<cv::Point2f> dstAffinePoints;
    dstAffinePoints.push_back(cv::Point2f(50, 50));              // ���Ͻ�
    dstAffinePoints.push_back(cv::Point2f(image.cols - 50, 80)); // ���Ͻ�
    dstAffinePoints.push_back(cv::Point2f(image.cols - 50, image.rows - 50)); // ���½�
    dstAffinePoints.push_back(cv::Point2f(50, image.rows - 80)); // ���½�

    // 2. ͸�ӱ任��Ŀ��㣨����ǿ�ҵ�͸��Ч����
    std::vector<cv::Point2f> dstPerspectivePoints;
    // ǿ�ҵ�͸��Ч�������Ϻ����������ƶ������Ϻ����������ƶ�
    dstPerspectivePoints.push_back(cv::Point2f(200, 50));        // ���Ͻǣ������ƶ���
    dstPerspectivePoints.push_back(cv::Point2f(image.cols - 200, 50)); // ���Ͻǣ������ƶ���
    dstPerspectivePoints.push_back(cv::Point2f(image.cols - 100, image.rows - 50)); // ���½ǣ������ƶ���
    dstPerspectivePoints.push_back(cv::Point2f(100, image.rows - 50)); // ���½ǣ������ƶ���

    // �������任����
    cv::Mat affineMatrix = cv::getAffineTransform(
        std::vector<cv::Point2f>(srcPoints.begin(), srcPoints.begin() + 3),
        std::vector<cv::Point2f>(dstAffinePoints.begin(), dstAffinePoints.begin() + 3)
    );

    // ����͸�ӱ任����
    cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(srcPoints, dstPerspectivePoints);

    // Ӧ�ñ任
    cv::Mat affineTransformed;
    cv::warpAffine(image, affineTransformed, affineMatrix, image.size());

    cv::Mat perspectiveTransformed;
    cv::warpPerspective(image, perspectiveTransformed, perspectiveMatrix, image.size());

    // ���Ʊ任ǰ��ĵ�
    cv::Mat imageWithPoints = image.clone();
    cv::Mat affineWithPoints = affineTransformed.clone();
    cv::Mat perspectiveWithPoints = perspectiveTransformed.clone();

    // ����ԭʼ��
    for (const auto& pt : srcPoints) {
        cv::circle(imageWithPoints, pt, 8, cv::Scalar(0, 0, 255), -1); // ��ɫ��
    }

    // ���Ʒ���任��
    for (int i = 0; i < 4; i++) {
        cv::circle(affineWithPoints, dstAffinePoints[i], 8, cv::Scalar(0, 255, 0), -1); // ��ɫ��
        cv::line(affineWithPoints, dstAffinePoints[i], dstAffinePoints[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
    }

    // ����͸�ӱ任��
    for (int i = 0; i < 4; i++) {
        cv::circle(perspectiveWithPoints, dstPerspectivePoints[i], 8, cv::Scalar(255, 0, 0), -1); // ��ɫ��
        cv::line(perspectiveWithPoints, dstPerspectivePoints[i], dstPerspectivePoints[(i + 1) % 4], cv::Scalar(255, 0, 0), 2);
    }

    // �������˵��
    cv::putText(imageWithPoints, "Original", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);
    cv::putText(affineWithPoints, "Affine Transform", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);
    cv::putText(perspectiveWithPoints, "Perspective Transform", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);

    // �������ڴ�С����ʾ
    cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
    cv::namedWindow("Affine Transform", cv::WINDOW_NORMAL);
    cv::namedWindow("Perspective Transform", cv::WINDOW_NORMAL);

    cv::resizeWindow("Original Image", 600, 400);
    cv::resizeWindow("Affine Transform", 600, 400);
    cv::resizeWindow("Perspective Transform", 600, 400);

    cv::imshow("Original Image", imageWithPoints);
    cv::imshow("Affine Transform", affineWithPoints);
    cv::imshow("Perspective Transform", perspectiveWithPoints);

    // ��ӡ˵��
    std::cout << "=== �任Ч���Ա� ===" << std::endl;
    std::cout << "����任: ����ƽ���ԣ���������Ȼƽ��" << std::endl;
    std::cout << "͸�ӱ任: ����3D͸��Ч����ƽ������Զ���ཻ" << std::endl;
    std::cout << "ע��۲������ߵı仯��" << std::endl;

    // �ȴ�����
    cv::waitKey(0);

    return 0;
}

//int main() {
//    // 1. ����һ����Ƶ������󣬲���0��ʾ��Ĭ������ͷ
//    cv::VideoCapture cap(0);
//
//    // �������ͷ�Ƿ�ɹ���
//    if (!cap.isOpened()) {
//        std::cout << "�����޷�������ͷ��" << std::endl;
//        return -1;
//    }
//
//    // ������������
//    cv::namedWindow("Original Video", cv::WINDOW_AUTOSIZE);
//    cv::namedWindow("Canny Edge Detection", cv::WINDOW_AUTOSIZE);
//
//    std::cout << "�� 'ESC' ���˳�����..." << std::endl;
//
//    while (true) {
//        cv::Mat frame, edges, grayFrame;
//
//        // 2. ������ͷ����һ֡ͼ��
//        cap >> frame;
//
//        // ���֡�Ƿ�Ϊ��
//        if (frame.empty()) {
//            std::cout << "���󣺲��񵽵�֡Ϊ�գ�" << std::endl;
//            break;
//        }
//
//        // 3. ����ɫͼ��ת��Ϊ�Ҷ�ͼ��Canny������Ҫ��ͨ��ͼ��
//        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
//
//        // 4. ʹ��Canny�㷨���б�Ե���
//        // ����������ͼ�����ͼ����ֵ1����ֵ2
//        cv::Canny(grayFrame, edges, 50, 150);
//
//        // 5. �ڴ�������ʾͼ��
//        cv::imshow("Original Video", frame);
//        cv::imshow("Canny Edge Detection", edges);
//
//        // 6. �ȴ�30���룬������Ƿ���'ESC'�� (ASCII��Ϊ27)
//        if (cv::waitKey(30) == 27) {
//            std::cout << "���������û���ֹ��" << std::endl;
//            break;
//        }
//    }
//
//    // 7. �������ʱ���Զ��ͷ�����ͷ���������д���
//    return 0;
//}

//int main() {
//    // 1. ���������������Haar������������
//    cv::CascadeClassifier faceCascade;
//    // ��ȷ�����·��ָ����ȷ��xml�ļ���
//    std::string cascadePath = "E:/study_tools/opencv/opencv/build/etc/haarcascades/haarcascade_frontalface_default.xml";
//
//    if (!faceCascade.load(cascadePath)) {
//        std::cout << "�����޷������������������ļ���" << std::endl;
//        std::cout << "��ȷ���ļ�·��: " << cascadePath << std::endl;
//        return -1;
//    }
//
//    // 2. ������ͷ
//    cv::VideoCapture cap(0);
//    if (!cap.isOpened()) {
//        std::cout << "�����޷�������ͷ��" << std::endl;
//        return -1;
//    }
//
//    std::cout << "����������������..." << std::endl;
//    std::cout << "�� 'ESC' ���˳�����" << std::endl;
//
//    cv::Mat frame, grayFrame;
//
//    while (true) {
//        // 3. ������ͷ��ȡһ֡ͼ��
//        cap >> frame;
//        if (frame.empty()) {
//            std::cout << "���󣺲��񵽵�֡Ϊ�գ�" << std::endl;
//            break;
//        }
//
//        // 4. ת��Ϊ�Ҷ�ͼ�����������Ҫ�ڻҶ�ͼ�Ͻ��У�
//        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
//        // ��ǿ�Աȶȣ���߼�⾫��
//        cv::equalizeHist(grayFrame, grayFrame);
//
//        // 5. �������
//        std::vector<cv::Rect> faces;
//        faceCascade.detectMultiScale(
//            grayFrame,   // ����Ҷ�ͼ��
//            faces,       // �����⵽�����������б�
//            1.1,         // ���ű�����1.1��ʾÿ��������������10%��
//            3,           // ��С�ھ�����Խ�߼��Խ�ϸ�©��Խ�൫���Խ�٣�
//            0,           // ��־��ͨ��Ϊ0��
//            cv::Size(30, 30) // ������С�����ߴ�
//        );
//
//        // 6. ��ԭʼ��ɫͼ���Ͽ�ѡ����⵽������
//        for (const auto& face : faces) {
//            // ������ɫ���ο��߿�Ϊ2
//            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
//
//            // ��ѡ���ڿ��Ϸ���ʾ����
//            cv::putText(frame, "Face",
//                cv::Point(face.x, face.y - 10),
//                cv::FONT_HERSHEY_SIMPLEX, 0.7,
//                cv::Scalar(0, 255, 0), 2);
//        }
//
//        // 7. �ڴ��ڱ�������ʾ��⵽����������
//        std::string windowTitle = "Face Detection - Found: " + std::to_string(faces.size());
//        cv::imshow(windowTitle, frame);
//
//        // 8. ��ⰴ����ESC���˳�
//        if (cv::waitKey(30) == 27) { // ESC����ASCII����27
//            std::cout << "�������˳���" << std::endl;
//            break;
//        }
//    }
//
//    // 9. �Զ��ͷ���Դ
//    return 0;
//}