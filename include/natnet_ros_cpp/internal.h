#ifndef INTERNAL_H
#define INTERNAL_H

#include <NatNetCAPI.h>
#include <NatNetClient.h>

#include <map>
#include <vector>

#include <ros/publisher.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Point32.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <sensor_msgs/PointCloud.h>

#include "nn_filter.h"

class Internal
{
public:
    
    // Keeping count on the assets
    // NOTE: It will be only counted at the start. It is not advised to add rigid body after starting the node 
    std::map<int32_t,std::string> ListRigidBodies; 
    std::map<double,std::string> ListSkeletons;
    std::map<double,std::string> ListForcePlates;
    std::map<double,std::string> ListDevices;

    //Initializing the publishers for the ros
    std::map<std::string, ros::Publisher> RigidbodyPub;
    std::map<std::string, ros::Publisher> RigidbodyMarkerPub;
    std::map<std::string, ros::Publisher> IndividualMarkerPub;
    std::map<std::string, ros::Publisher> SkeletonPub;
    std::map<std::string, ros::Publisher> ForcePlatePub;
    std::map<std::string, ros::Publisher> DevicePub;
    ros::Publisher PointcloudPub;
    sensor_msgs::PointCloud msgPointcloud; // point cloud msg
    ros::Time timeStampLatestFrame; // timestamp of the latest frame in local time (sec)
    int markerCount = 0; // number of individual markers (unlabled + labled)
    int UnlabeledCount = 0;
    sServerDescription g_serverDescription;
    SetParam rosparam;
    std::vector<sRigidBodyData> last_data;

    void Pass(){}; //Void function to do nothing
    void Init(ros::NodeHandle &n); //take care of ros params

    void InitLastData(int, Internal&);
    bool CompareLastData(sRigidBodyData, sRigidBodyData);

    // Establish a NatNet Client connection
    int ConnectClient(NatNetClient* g_pClient, sNatNetClientConnectParams &g_connectParams);

    // MessageHandler receives NatNet error/debug messages
    static void MessageHandler( Verbosity msgType, const char* msg );

    // Function to get some useful information from the motion capture system
    // Enable log_internal parameter to have a look at it. 
    void Info(NatNetClient* g_pClient, ros::NodeHandle &n);

    //Provides information on the latencies of the different systems
    void LatenciInfo(sFrameOfMocapData* data, void* pUserData, Internal &internal);
    
    // Initialize the header of the ROS message 
    void initROSmsgHeader(std_msgs::Header &msgHeader, Internal &internal, std::string &base_frame);

    // Handles the data from the frame and publish it as ROS topics
    void DataHandler(sFrameOfMocapData* data, void* pUserData, Internal &internal);

    // Publishes the data of rigidbodies
    void PubRigidbodyPose(sRigidBodyData &data, Internal &internal);

    // Publish markers of the rigidbodies
    void PubRigidbodyMarker(sMarker &data, Internal &internal);

    // Publish Single marker as the Rigidbody and TF
    void PubMarkerPoseWithTrack(sMarker &data, Internal &internal);

    // Publish Point cloud from the marker
    void AppendToPointCloud(sMarker &data, Internal &internal);

    void fillTFMessage(sRigidBodyData &data, Internal &internal, geometry_msgs::TransformStamped &msgTFRigidBodies);
    void fillTFMessage(sMarker &data, Internal &internal, geometry_msgs::TransformStamped &msgTFMarker, int update);
};



#endif
