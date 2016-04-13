#include <sbpl_utils/visualization/grid_visualizer.h>

GridVisualizer::GridVisualizer() : height_(0), width_(0) {

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

void GridVisualizer::VisualizeState(int x_coord, int y_coord, int red, int green, int blue) {
  stateful_grid_.at<cv::Vec3b>(y_coord, x_coord)[0] = red; 
  stateful_grid_.at<cv::Vec3b>(y_coord, x_coord)[1] = green; 
  stateful_grid_.at<cv::Vec3b>(y_coord, x_coord)[2] = blue; 
  DrawSpecialStates();
  cv::imshow("expansions", stateful_grid_);
  cv::waitKey(1);
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

void GridVisualizer::ClearStates() {
  stateful_grid_.setTo(cv::Vec3b(255, 255, 255));
}
