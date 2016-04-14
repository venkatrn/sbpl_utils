#include <sbpl_utils/visualization/grid_visualizer.h>

namespace {
  constexpr int kDisplayImageWidth = 300;
  constexpr int kDisplayImageHeight = 300;
}

GridVisualizer::GridVisualizer() : height_(0), width_(0), scaling_factor_x_(1.0), scaling_factor_y_(1.0) {

}

void GridVisualizer::SetGrid(const unsigned char *const *const grid_data, int height, int width) {
  grid_.create(height, width, CV_8UC1);
	for (int ii = 0; ii < height; ++ii) {
		for (int jj = 0; jj < width; ++jj) {
			// SBPL grids are in column-major order.
			grid_.at<unsigned char>(ii, jj) = grid_data[jj][ii];
		}
	}
  cv::normalize(grid_, grid_, 0, 255, cv::NORM_MINMAX, CV_8UC1);
  cv::applyColorMap(grid_, colored_grid_, cv::COLORMAP_JET);
  stateful_grid_ = colored_grid_.clone();
}

void GridVisualizer::SetGrid(const char *const *const grid_data, int height, int width) {
  grid_.create(height, width, CV_8UC1);
	for (int ii = 0; ii < height; ++ii) {
		for (int jj = 0; jj < width; ++jj) {
			// SBPL grids are in column-major order.
			grid_.at<unsigned char>(ii, jj) = static_cast<unsigned char>(grid_data[jj][ii]);
		}
	}
  cv::normalize(grid_, grid_, 0, 255, cv::NORM_MINMAX, CV_8UC1);
  cv::applyColorMap(grid_, colored_grid_, cv::COLORMAP_JET);
  stateful_grid_ = colored_grid_.clone();
  if (height < 200 || width < 200) {
    scaling_factor_x_ = kDisplayImageHeight / height;
    scaling_factor_y_ = kDisplayImageWidth / width;
  }
}

void GridVisualizer::VisualizeState(int x_coord, int y_coord, int red, int green, int blue) {
  // BGR order
  stateful_grid_.at<cv::Vec3b>(y_coord, x_coord)[0] = blue; 
  stateful_grid_.at<cv::Vec3b>(y_coord, x_coord)[1] = green; 
  stateful_grid_.at<cv::Vec3b>(y_coord, x_coord)[2] = red; 
}

void GridVisualizer::VisualizeLine(int x1, int y1, int x2, int y2, int red, int green, int blue) {
  cv::Point p1(x1, y1);
  cv::Point p2(x2, y2);
  cv::Scalar color(blue, green, red);
  cv::line(stateful_grid_, p1, p2, color, 2);
}

void GridVisualizer::AddSpecialState(int x_coord, int y_coord, int red, int green, int blue) {
  special_states_.emplace_back(x_coord, y_coord, red, green, blue);
}

void GridVisualizer::DrawSpecialStates() {
  for (const auto &grid_item : special_states_) {
    const int x_coord = grid_item.x;
    const int y_coord = grid_item.y;
    const int red = grid_item.red;
    const int green = grid_item.green;
    const int blue = grid_item.blue;
    cv::Point center(x_coord, y_coord);
    // OpenCV has BGR order.
    cv::Scalar color(blue, green, red);
    cv::circle(stateful_grid_, center, 5, color, -1);
    // stateful_grid_.at<cv::Vec3b>(y_coord, x_coord)[0] = red; 
    // stateful_grid_.at<cv::Vec3b>(y_coord, x_coord)[1] = green; 
    // stateful_grid_.at<cv::Vec3b>(y_coord, x_coord)[2] = blue; 
  }
}

void GridVisualizer::Display(int delay) {
  static cv::Mat scaled_grid;
  DrawSpecialStates();
  cv::resize(stateful_grid_, scaled_grid, cv::Size(), scaling_factor_x_, scaling_factor_y_, cv::INTER_LANCZOS4);
  cv::imshow("expansions", scaled_grid);
  cv::waitKey(delay);
}

void GridVisualizer::ClearStates() {
  stateful_grid_.setTo(cv::Vec3b(255, 255, 255));
}
