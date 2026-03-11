#ifndef SECOND_CHALLENGE_HPP
#define SECOND_CHALLENGE_HPP

#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>

#include <rclcpp/rclcpp.hpp>
#include <functional>  // bind & placeholders用
#include <memory>      // SharedPtr用
#include <optional>    // has_value()用
#include <chrono>      // 時間を扱うために追加

class SecondChallenge : public rclcpp::Node
{
    public:
        SecondChallenge();
        // void process();

        // コールバック関数
        void timer_callback();
        void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);

        // 関数
        bool can_move();                        // センサ情報（今回はodom）を取得できているかの確認用
        bool is_goal();                         // 終了判定
        double calc_distance();                 // 進んだ距離を計算
        void run(float velocity, float omega);  // roombaの制御入力を決定
        void set_cmd_vel();                     // 並進速度と旋回速度を計算

        // 変数
        int hz_ = 10;             // 制御周期
        double goal_dist_ = 1.0;  // ゴールまでの距離
        double velocity_ = 0.3;   // 直進時の並進速度
        std::optional<sensor_msgs::msg::LaserScan> scan_;  // optional型で定義することによりscanをsubできたかの判定も同時に行う
        geometry_msgs::msg::Twist cmd_vel_;

        // Pub & Sub
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;   // scan
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;  // 制御入力
        rclcpp::TimerBase::SharedPtr timer_;  // タイマー
};

#endif  // SECOND_CHALLENGE_HPP
