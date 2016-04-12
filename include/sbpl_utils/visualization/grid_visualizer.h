#pragma once

#include <opencv2/core/core.hpp>  
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

struct GridItem {
  int x;
  int y;
  int red;
  int green;
  int blue;
  GridItem(int x, int y, int red, int green, int blue) {
    this->x = x;
    this->y = y;
    this->red = red;
    this->green = green;
    this->blue = blue;
  }
};

class GridVisualizer {
  public:
    GridVisualizer();
    void SetGrid(const std::vector<std::vector<char>> &grid_data);
    void SetGrid(const unsigned char *const * const grid_data, int height, int width);
    void VisualizeState(int x_coord, int y_coord, int red = 127, int green = 127, int blue = 127);
    void AddSpecialState(int x_coord, int y_coord, int red = 0, int green = 0, int blue = 0);
    void ClearStates();
    
  private:
    cv::Mat grid_;
    cv::Mat colored_grid_;
    cv::Mat stateful_grid_;
    int height_;
    int width_;
    std::vector<GridItem> special_states_;

    void DrawSpecialStates();
};
