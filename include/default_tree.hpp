#ifndef DEFAULT_TREE_HPP
#define DEFAULT_TREE_HPP

#include <string>

class GenusHeight {
  public:
    std::string genus;
    double totalHeight;
    int count;

    GenusHeight() : totalHeight(0), count(0) {}

    void addHeight(double height) {
        totalHeight += height;
        count++;
    }

    double averageHeight() const { return count > 0 ? totalHeight / count : 0; }
};

#endif