#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <LSLDevice.hpp>

namespace rosneuro {

class LSLDeviceTest : public ::testing::Test {
    public:
        LSLDeviceTest(){}
        ~LSLDeviceTest(){}
        void SetUp(void) {
            lslDev = new LSLDevice();
            lslDevFrame = new LSLDevice(new NeuroFrame());
        }
        void TearDown(void) {
            delete lslDev;
            delete lslDevFrame;
        }
        LSLDevice* lslDev = new LSLDevice();
        LSLDevice* lslDevFrame = new LSLDevice(new NeuroFrame());
};

TEST_F(LSLDeviceTest, Constructor) {
    EXPECT_EQ(lslDev->name_, "lsldev");
    EXPECT_EQ(lslDev->stream_, nullptr);
    EXPECT_EQ(lslDev->info_, nullptr);
    EXPECT_EQ(lslDev->stream_name_, "unknown");
    EXPECT_EQ(lslDev->stream_type_, "unknown");
    EXPECT_EQ(lslDevFrame->name_, "lsldev");
    EXPECT_EQ(lslDevFrame->stream_, nullptr);
    EXPECT_EQ(lslDevFrame->info_, nullptr);
    EXPECT_EQ(lslDevFrame->stream_name_, "unknown");
    EXPECT_EQ(lslDevFrame->stream_type_, "unknown");
}

TEST_F(LSLDeviceTest, Configure) {
    NeuroFrame* frame = new NeuroFrame();
    unsigned int framerate = 1;

    ros::param::set("~stream_type", "unknown");
    ros::param::set("~stream_name", "unknown");

    EXPECT_EQ(lslDev->Configure(frame, framerate), true);
    EXPECT_EQ(lslDevFrame->Configure(frame, framerate), true);

    ros::param::del("~stream_name");
    EXPECT_FALSE(lslDev->Configure(frame, framerate));
    ros::param::del("~stream_type");
    EXPECT_FALSE(lslDev->Configure(frame, framerate));
}

TEST_F(LSLDeviceTest, Open) {
    ros::param::set("~stream_type", "EEG");
    ros::param::set("~stream_name", "obci_eeg1");

    lslDev->Configure(new NeuroFrame(), 1);
    lslDev->Open();

    ros::param::del("~stream_name");
    ros::param::del("~stream_type");
}


}

int main(int argc, char **argv) {
    ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Fatal);
    ros::init(argc, argv, "test_lsl_device");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}