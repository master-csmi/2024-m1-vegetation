#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

int main()
{
    std::vector<Tree> treeLibrary = createLibraryFromJsonFile("query_result.json");

    // Create a point cloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    // Add points to the cloud for each tree
    for (const auto &tree : treeLibrary)
    {
        // Add a point for the trunk
        cloud->push_back(pcl::PointXYZ(tree.lat, tree.lon, tree.height / 2));

        // Add a point for the crown
        cloud->push_back(pcl::PointXYZ(tree.lat, tree.lon, tree.height + tree.diameter_crown / 2));
    }

    // TODO: Use a surface reconstruction algorithm to create a mesh from the point cloud

    return 0;
}