#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>

namespace fs = std::filesystem;

// 支持的图片扩展名（不区分大小写）
bool isImageFile(const fs::path& path) {
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || 
            ext == ".bmp" || ext == ".tif" || ext == ".tiff" || ext == ".webp");
}

int main() {
    // 获取当前工作目录（即程序所在目录）
    fs::path currentDir = fs::current_path();
    std::cout << "当前目录: " << currentDir << std::endl;

    // 收集所有图片文件
    std::vector<fs::path> imagePaths;
    for (const auto& entry : fs::directory_iterator(currentDir)) {
        if (entry.is_regular_file() && isImageFile(entry.path())) {
            imagePaths.push_back(entry.path());
        }
    }

    if (imagePaths.empty()) {
        std::cerr << "未找到任何图片文件！" << std::endl;
        return -1;
    }

    // 按文件名排序（可选）
    std::sort(imagePaths.begin(), imagePaths.end());

    std::cout << "找到 " << imagePaths.size() << " 张图片。" << std::endl;

    // 遍历并显示每张图片
    for (const auto& imgPath : imagePaths) {
        std::cout << "正在加载: " << imgPath.filename().string() << std::endl;
        cv::Mat img = cv::imread(imgPath.string(), cv::IMREAD_COLOR);
        if (img.empty()) {
            std::cerr << "警告: 无法读取图片 " << imgPath << std::endl;
            continue;
        }

        // 显示图片（窗口大小自适应）
        cv::namedWindow("图片查看器", cv::WINDOW_NORMAL);
        cv::imshow("图片查看器", img);

        // 等待按键：按任意键查看下一张，按 ESC 或 'q' 退出
        int key = cv::waitKey(0);
        if (key == 27 || key == 'q' || key == 'Q') {
            std::cout << "用户退出。" << std::endl;
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}