#include  "second_challenge/second_challenge.hpp"

SecondChallenge::SecondChallenge()
: Node("second_challenge")
{
    // --- パブリッシャーの作成 ---
    // トピック名: "cmd_vel"
    cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", hz_);
  
    // --- サブスクライバーの作成 ---
    // トピック名: "scan"
    // 実行する関数: scan_callback (この後、クラス内に定義する必要がある)
    scan_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
      "scan", hz_, std::bind(&SecondChallenge::scan_callback, this, std::placeholders::_1));

    // firstで使用したグローバル変数の定義にtimerを追加
    using namespace std::chrono_literals;
    timer_ = this->create_wall_timer(100ms, std::bind(&SecondChallenge::timer_callback, this)); 
}

void SecondChallenge::timer_callback()
{
    // 繰り返したい関数を定義
    // 距離を判定して、速度をセットしパブリッシュする
    if (can_move()) {
        set_cmd_vel();
    } else {
        RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 1000, "Waiting for scan data...");
    }
}

void SecondChallenge::scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    // ここにレーザースキャンのコールバックの処理を書く
    scan_ = *msg;  // 受け取ったメッセージをクラス内の変数に保存
}

bool SecondChallenge::can_move()
{
    return scan_.has_value();
}

bool SecondChallenge::is_goal()
{
    return calc_distance() <= goal_dist_;
}

double SecondChallenge::calc_distance()
{
    // 壁からの距離や壁に垂直な向きのセンサが何番目か調べる
    // ロボットの真正面にある一番近い障害物までの距離: ranges配列の中央要素
    float range_ahead = scan_.value().ranges[scan_.value().ranges.size() / 2];
    return range_ahead;
  
    /* 方向
    bearing = msg.angle_min + i * msg.angle_max / len(msg.ranges);*/

    /* スキャナで検出した一番近い障害物までの距離: ranges配列の最小値
    closest_range = min(msg.ranges) */

}

void SecondChallenge::run(float velocity, float omega)
{
    // ここにロボットを動かす処理を書く
    // 速度データを格納
    cmd_vel_.linear.x = velocity;
    cmd_vel_.angular.z = omega;

    // パブリッシュを実行
    cmd_vel_pub_->publish(cmd_vel_);
}

void SecondChallenge::set_cmd_vel()
{
    // ここにcmd_velをセットする処理を書く
    if(is_goal())
    {
    // 停止
        run(0.0, 0.0);
    }
    else
    {
    // 直進 (0.1m/s, 0.0rad/s)
        run(velocity_, 0.0);
    }
}